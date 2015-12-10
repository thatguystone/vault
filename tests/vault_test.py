import glob
import inspect
from nose.tools import *
import os
import os.path
import random
import shutil
import unittest

from vault import crypt, fs, loopdev, util, Vault, noexcept

@unittest.skipIf(os.getenv('SCHROOT_SESSION_ID'),
	'these tests fail when running from a chrooted/unionfs env')
class TestVault(unittest.TestCase):
	_multiprocess_can_split_ = True

	VAULT = "vault_test.vlt"
	PASS = "test"
	DEFARGS = {
		"iter_time": 1,
		"use_urandom": True,
		"password": PASS,
	}

	def setUp(self):
		# Cleanup after any failures from previous runs
		noexcept(fs.Unmount(self.path()), important=False)
		noexcept(crypt.Close(self.path()), important=False)
		noexcept(loopdev.Close(self.path()), important=False)

		shutil.rmtree(self.dir())
		self.v = Vault(self.path())

	def tearDown(self):
		self.v.close()

	def id(self):
		id = super().id()
		return "_".join(id.split('.')[-2:])

	def dir(self):
		dir = os.path.join("test_data", self.id())
		os.makedirs(dir, exist_ok=True)
		return dir

	def path(self):
		return os.path.join(self.dir(), self.VAULT)

	def mount(self):
		dir = os.path.join(self.dir(), "vault")
		return dir

	def open(self):
		self.v.open(self.mount(), password=self.PASS)

	def create(self, randomize=True, **kwargs):
		args = dict(**self.DEFARGS)
		args.update(**kwargs)

		self.v.create(self.mount(), "10m",
			randomize=randomize,
			**args)

class TestBasic(TestVault):
	def test_create_close(self):
		self.create()
		self.v.close()

class TestResize(TestVault):
	def _resize(self,
		how_much, fn, randomize=True,
		close=False, shrink=False):

		rand = "{}".format(random.random())
		path = os.path.join(self.mount(), "test_file")

		with open(path, "x") as f:
			f.write(rand)

		if close:
			self.v.close()

		size = os.path.getsize(self.path())
		if shrink:
			size -= util.human_size(how_much)
		else:
			size += util.human_size(how_much)

		fn(how_much, password=self.PASS)
		self.v.close()
		self.open()

		with open(path, "r") as f:
			assert_equal(rand, f.read())

		assert_equal(os.path.getsize(self.path()), size)

class TestGrow(TestResize):
	def _grow(self, how_much, randomize=True, close=False):
		self._resize(
			how_much, self.v.grow,
			randomize=True, close=close)

	def test_closed(self):
		self.create()
		self._grow("10m", close=True)

	def test_open(self):
		self.create()
		self._grow("10m")

	def test_closed_no_randomize(self):
		self.create(randomize=False)
		self._grow("10m", randomize=False, close=True)

	def test_open_no_randomize(self):
		self.create(randomize=False)
		self._grow("10m", randomize=False)

class TestShrink(TestResize):
	def _shrink(self, how_much, close=False):
		self._resize(how_much, self.v.shrink, close, shrink=True)

	def test_closed(self):
		self.create()
		self._shrink("1m", close=True)

	def test_open(self):
		self.create()
		self._shrink("1m")

class TestFailures(TestVault):
	def test_crypt_args(self):
		with assert_raises(RuntimeError):
			self.create(cipher_mode="cbc", hash="blah")

		# Should delete file on failure
		assert not os.path.exists(self.path())

class TestCallbacks(TestVault):
	def test_open_close(self):
		self.create()

		with open(os.path.join(self.mount(), "vault_open"), "w") as f:
			os.chmod(f.name, 0o755)
			f.write("#! /bin/bash\ntouch open_hit")

		with open(os.path.join(self.mount(), "vault_close"), "w") as f:
			os.chmod(f.name, 0o755)
			f.write("#! /bin/bash\ntouch close_hit")

		self.v.close()
		self.open()

		assert os.path.exists(os.path.join(self.mount(), "open_hit"))
		assert os.path.exists(os.path.join(self.mount(), "close_hit"))
