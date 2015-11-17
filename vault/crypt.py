import hashlib
import os

from . import util

class Format(object):
	def __init__(self,
		lodev, cipher_mode,
		hash, kdf_iter_time_ms,
		use_urandom, password):

		self.lodev = lodev
		self.cipher_mode = cipher_mode
		self.hash = hash
		self.kdf_iter_time_ms = kdf_iter_time_ms
		self.use_urandom = use_urandom
		self.password = password

	def run(self):
		args = ["sudo",
			"cryptsetup",
			"--batch-mode",
			"--verify-passphrase",
			"--cipher", "aes-%s" % self.cipher_mode]

		if self.hash:
			args += ["--hash", self.hash]

		if self.kdf_iter_time_ms:
			args += ["--iter-time", str(self.kdf_iter_time_ms)]

		if "xts" in self.cipher_mode:
			args += ["--key-size", "512"]
		else:
			args += ["--key-size", "256"]

		if self.use_urandom:
			args += ["--use-urandom"]
		else:
			args += ["--use-random"]

		if self.password:
			args += ["--key-file=-"]

		args += ["luksFormat", self.lodev]

		util.run(*args, stdin=self.password)

	def undo(self):
		pass

class Open(object):
	def __init__(self, file, lodev, password=None):
		self.name = dev_name_for(file)
		self.lodev = lodev
		self.password = password

	def run(self):
		args = ["sudo",
			"cryptsetup",
			"--batch-mode"]

		if self.password:
			args += ["--key-file=-"]

		args += ["open", "--type", "luks",
			self.lodev, self.name]

		util.run(*args, stdin=self.password)

		return mapper(self.name)

	def undo(self):
		_close(self.name)

class Close(object):
	def __init__(self, file):
		self.name = dev_name_for(file)

	def run(self):
		_close(self.name)

def is_luks(dev):
	try:
		util.run("sudo",
			"cryptsetup",
			"isLuks", dev)
		return True
	except:
		return False

def mapper(name):
	return "/dev/mapper/%s" % name

def dev_name_for(file):
	base = os.path.basename(file)
	id = hashlib.md5(util.to_bytes(os.path.abspath(file))).hexdigest()
	return "%s-%s" % (base, id)

def _close(name):
	util.run("sudo",
		"cryptsetup",
		"close", name)
