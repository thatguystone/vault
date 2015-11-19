import glob
import inspect
from nose.tools import *
import os
import os.path
import random
import shutil
import unittest

from .. import crypt, fs, loopdev, util, vault

class TestVault(unittest.TestCase):
	_multiprocess_can_split_ = True

	VAULT = "vault_test.vlt"
	PASS = "test"
	DEFARGS = {
		"kdf_iter_time_ms": 1,
		"use_urandom": True,
		"password": PASS,
	}

	def setUp(self):
		# Cleanup after any failures from previous runs
		vault.noexcept(fs.Unmount(self.path()), important=False)
		vault.noexcept(crypt.Close(self.path()), important=False)
		vault.noexcept(loopdev.Close(self.path()), important=False)

		shutil.rmtree(self.dir())
		self.v = vault.Vault(self.path())

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
		os.makedirs(dir, exist_ok=True)
		return dir

	def create(self):
		self.v.create(self.mount(), "10m", **self.DEFARGS)

class TestBasic(TestVault):
	def test_create_close(self):
		self.create()
		self.v.close()

class TestResize(TestVault):
	def _resize(self, how_much, fn, close=False):
		rand = "{}".format(random.random())
		path = os.path.join(self.mount(), "test_file")

		with open(path, "x") as f:
			f.write(rand)

		if close:
			self.v.close()

		fn(how_much, password=self.PASS)
		self.v.close()
		self.v.open(self.mount(), password=self.PASS)

		with open(path, "r") as f:
			assert_equal(rand, f.read())

class TestGrow(TestResize):
	def _grow(self, how_much, close=False):
		self._resize(how_much, self.v.grow, close)

	def test_closed(self):
		self.create()
		self._grow("10m", close=True)

	def test_open(self):
		self.create()
		self._grow("10m")

class TestShrink(TestResize):
	def _shrink(self, how_much, close=False):
		self._resize(how_much, self.v.shrink, close)

	def test_closed(self):
		self.create()
		self._shrink("1m", close=True)

	def test_open(self):
		self.create()
		self._shrink("1m")
