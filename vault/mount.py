import os.path

from . import crypt, util

class Format(object):
	def __init__(self, fs, crypt_dev):
		self.fs = fs
		self.crypt_dev = crypt_dev

	def run(self):
		util.run("sudo", "mkfs", "-t", self.fs, self.crypt_dev)

	def undo(self):
		pass

class Open(object):
	def __init__(self, crypt_dev, mount_dir):
		self.crypt_dev = crypt_dev
		self.mount_dir = mount_dir

	def run(self):
		util.run("sudo", "mount", self.crypt_dev, self.mount_dir)

	def undo(self):
		Close(self.mount_dir).run()

class Close(object):
	def __init__(self, file):
		self.file = file

	def run(self):
		cryptdev = crypt.mapper(crypt.dev_name_for(self.file))

		if os.path.exists(cryptdev):
			util.run("sudo", "umount", cryptdev)
		else:
			util.run("sudo", "umount", self.file)

class Resize(object):
	def __init__(self, dev, size):
		self.dev = dev
		self.size = human_size(size)

	def run(self):
		util.run("sudo", "resize2fs", self.dev, "%dK" % (self.size / 1024))

def is_mounted(file):
	cryptdev = crypt.mapper(crypt.dev_name_for(self.file))

	try:
		util.run("sudo", "mount", "--fake", cryptdev)
		return True
	except:
		return False
