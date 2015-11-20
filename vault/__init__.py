import argparse
import logging
import os.path
import sys
import time

from . import crypt, file, fs, loopdev, util

log = logging.getLogger(__name__)

class Vault(object):
	def __init__(self, vault):
		self.vault = vault

	def create(self, mount_dir, size,
		randomize=True,	fs_type="ext4",
		cipher_mode="xts-plain64",
		hash=None, iter_time=None,
		use_urandom=False,
		password=None):
		"""
		Create a new vault.

		:param mount_dir: Where to mount the newly-created vault

		:param size: How large the vault should be. May be human readable
			values such as "15m", "1GB", or a raw number for number of bytes.

		:param randomize: If the vault file should be randomized on creation.

		:param fs: Which filesystem to put into the vault

		:param cipher_mode: Cipher mode to pass through to cryptsetup.
		    Currently (as of late 2015), only cbc-essiv:<sha256,sha512>
		    and xts-plain64 are supported.

		:param hash: Which hash to use for cryptsetup's PBKDF2 function. sha1
		    is typically considered sufficient. If None is given, uses
		    cryptsetup's default.

		:param iter_time: How long to iterate on the password, in ms. If None
		    is given, uses cryptsetup's default.

		:param use_urandom: Use /dev/urandom for key generation.

		:param password: INSECURELY provide a password. This should only be
		    used for testing.
		"""

		with Transaction() as t:
			t.add(file.Create(self.vault, size, randomize))
			lodev = t.add(loopdev.Open(self.vault))

			t.add(crypt.Format(
				lodev,
				cipher_mode,
				hash, iter_time,
				use_urandom, password))

			cryptdev = t.add(crypt.Open(self.vault, lodev, password))

			t.add(fs.Format(fs_type, cryptdev))
			t.add(fs.Mount(cryptdev, mount_dir))

	def open(self, mount_dir, password=None):
		if self.is_mounted():
			return

		with Transaction() as t:
			lodev = t.add(loopdev.Open(self.vault))
			cryptdev = t.add(crypt.Open(self.vault, lodev, password))
			t.add(fs.Mount(cryptdev, mount_dir))

	def close(self, important=True):
		noexcept(fs.Unmount(self.vault), important=important)
		noexcept(crypt.Close(self.vault), important=important)
		noexcept(loopdev.Close(self.vault), important=important)

	def is_mounted(self):
		opened = False

		dev = loopdev.find(self.vault)
		if dev:
			opened = crypt.is_luks(dev) and fs.is_mounted(self.vault)

		return opened

	def grow(self, how_much, randomize=True, password=None):
		# 0) close everything
		# 1) resize file
		# 2) open without mounting
		# 3) resize FS
		# 4) close everything again so that next open works right

		self.close(important=False)

		with Transaction() as t:
			t.add(file.Resize(self.vault, how_much, randomize))
			lodev = t.add(loopdev.Open(self.vault))
			cryptdev = t.add(crypt.Open(self.vault, lodev, password))
			t.add(fs.Resize(cryptdev))
			self.close(important=False)

	def shrink(self, how_much, password=None):
		# 0) open without mounting
		# 1) resize FS
		# 2) close everything again
		# 3) truncate file

		with Transaction() as t:
			if self.is_mounted():
				cryptdev = t.add(fs.Unmount(self.vault))
			else:
				lodev = t.add(loopdev.Open(self.vault))
				cryptdev = t.add(crypt.Open(self.vault, lodev, password))

			curr_size = fs.size(cryptdev)
			size = curr_size - util.human_size(how_much)
			if size < 0:
				raise AssertionError("cannot shrink more than total size: {} > {}".format(
					util.human_size(how_much),
					curr_size))

			actual_resize = t.add(fs.Resize(cryptdev, size))

			self.close(important=False)
			t.add(file.Resize(self.vault, -(curr_size - actual_resize)))

class Transaction(object):
	def __enter__(self):
		self._undos = []
		return self

	def _get_name(self, obj):
		return "{}.{}".format(
			obj.__module__,
			obj.__class__.__name__)

	def __exit__(self, exc_type, exc_val, exc_tb):
		if exc_tb:
			for undoer in self._undos:
				try:
					undoer.undo()
				except Exception as e:
					log.warn("undoing %s failed: %s",
						self._get_name(undoer),
						e)

	def add(self, op):
		start = time.monotonic()
		try:
			ret = op.run()
		finally:
			log.debug("add (took %fs): %s",
				time.monotonic() - start,
				self._get_name(op))

		self._undos.insert(0, op)
		return ret

def noexcept(r, important=True):
	try:
		r.run()
	except Exception as e:
		l = log.warn if important else log.debug
		l("%s failed: %s", r.__class__.__name__, e)
