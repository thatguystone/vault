#! /usr/bin/env python

import argparse
import logging
import os.path
import sys

from . import crypt, file, loopdev, mount

log = logging.getLogger(__name__)

class Vault(object):
	def __init__(self, vault):
		self.vault = vault

	def create(self, mount_dir, size,
		randomize=True,	fs="ext4",
		cipher_mode="xts-plain64",
		hash=None, kdf_iter_time_ms=None,
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
		    Currently (as of late 2015), only cbc-essiv:<sha1,sha256,sha512>
		    and xts-plain64 are supported.

		:param hash: Which hash to use for cryptsetup's PBKDF2 function. sha1
		    is typically considered sufficient. If None is given, uses
		    cryptsetup's default.

		:param kdf_iter_time_ms: How long to iterate on the password, in ms.
		    If None is given, uses cryptsetup's default.

		:param use_urandom: Use /dev/urandom for key generation.

		:param password: INSECURELY provide a password. This should only be
		    used for testing.
		"""

		with Transaction() as t:
			t.add(file.Create(self.vault, size, randomize))
			t.add(file.Randomize(self.vault, size, randomize))
			lodev = t.add(loopdev.Open(self.vault))

			t.add(crypt.Format(
				lodev,
				cipher_mode,
				hash, kdf_iter_time_ms,
				use_urandom, password))

			cryptdev = t.add(crypt.Open(self.vault, lodev, password))

			t.add(mount.Format(fs, cryptdev))
			t.add(mount.Open(cryptdev, mount_dir))

	def open(self, mount_dir, password=None):
		if self.is_mounted():
			return

		with Transaction() as t:
			lodev = t.add(loopdev.Open(self.vault))
			cryptdev = t.add(crypt.Open(self.vault, lodev, password))
			t.add(mount.Open(cryptdev, mount_dir))

	def close(self):
		noexcept(mount.Close(self.vault))
		noexcept(crypt.Close(self.vault))
		noexcept(loopdev.Close(self.vault))

	def is_mounted(self):
		opened = False

		dev = loopdev.find(self.vault)
		if dev:
			opened = crypt.is_luks(dev) and mount.is_mounted(self.vault)

		return opened

	def grow(self, mount_dir, how_much, randomize=True, password=None):
		pass

	def shrink(self, mount_dir, how_much, password=None):
		pass

		# size = util.human_size(size)
		# curr_size = self._size()

		# try:
		# 	if curr_size == size:
		# 		return

		# 	self.open(mount_dir, password=password)
		# 	mount.Close(self.vault).run()

		# 	if curr_size > size:
		# 		file.Resize(self.vault,
		# 			curr_size, size,
		# 			randomize=randomize).run()
		# 		crypt.Resize(self.vault)
		# 		mount.Resize(self.vault, size).run()
		# 		return

		# 	mount.Resize(self.vault, size).run()
		# 	crypt.Close(self.vault)
		# 	file.Resize(self.vault, curr_size, size).run()
		# finally:
		# 	self.close()

	def _size(self):
		return os.path.getsize(self.vault)

class Transaction(object):
	def __enter__(self):
		self._undos = []
		return self

	def __exit__(self, exc_type, exc_val, exc_tb):
		if exc_tb:
			for undoer in self._undos:
				try:
					undoer.undo()
				except Exception as e:
					log.warn("undoing %s.%s failed: %s",
						undoer.__module__,
						undoer.__class__.__name__,
						e)

	def add(self, op):
		ret = op.run()
		self._undos.insert(0, op)
		return ret

def noexcept(r):
	try:
		r.run()
	except Exception as e:
		log.warn("%s failed: %s",
			r.__class__.__name__,
			e)
