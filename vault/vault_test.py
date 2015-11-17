import inspect
import nose
import os
import os.path
import shutil
import unittest

from . import vault

def setup_module():
	shutil.rmtree("test_data/", ignore_errors=True)
	os.mkdir("test_data/")

def get_dir():
	dir = os.path.join("test_data", inspect.stack()[2][3])
	os.makedirs(dir, exist_ok=True)
	return dir

def get_vault():
	return os.path.join(get_dir(), "vault.vlt")

def get_mount():
	dir = os.path.join(get_dir(), "vault")
	os.makedirs(dir, exist_ok=True)
	return dir

def test_create_close():
	v = vault.Vault(get_vault())
	v.create(get_mount(), "10m",
		kdf_iter_time_ms=1,
		use_urandom=True,
		password="test")
	v.close()

def test_resize():
	v = vault.Vault(get_vault())
	v.create(get_mount(), "10m",
		kdf_iter_time_ms=1,
		use_urandom=True,
		password="test")
	v.close()

	v.resize("20m")
