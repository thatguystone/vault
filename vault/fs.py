import csv
import os.path
import time

from . import crypt, util

class Format(object):
	def __init__(self, fs, crypt_dev):
		self.fs = fs
		self.crypt_dev = crypt_dev

	def run(self):
		util.run("mkfs", "-t", self.fs, self.crypt_dev)

	def undo(self):
		pass

class Mount(object):
	def __init__(self, crypt_dev, mount_dir):
		self.crypt_dev = crypt_dev
		self.mount_dir = mount_dir

	def run(self):
		util.run("mount", self.crypt_dev, self.mount_dir)

	def undo(self):
		Close(self.mount_dir).run()

class Unmount(object):
	def __init__(self, file):
		self.file = file

	def run(self):
		dev = crypt.mapper(crypt.dev_name_for(self.file))

		# Maybe given mount-point instead of file?
		if not os.path.exists(dev) and os.path.exists(self.file):
			dev = os.path.abspath(self.file)

		if not os.path.exists(dev):
			raise Exception("mount path not found for {}".format(self.file))

		if _is_dev_mounted(dev):
			# If umount fails (someone is still accessing the files, or something
			# like that), try again and hope it clears up. This can happen during
			# testing when mounting and unmounting quickly since some daemons
			# might try to inspect the newly mounted stuffs.
			for i in range(5):
				try:
					util.run("umount", dev)
					break
				except Exception as e_:
					e = e_
					time.sleep(.1)
			else: # If all retries failed...
				raise e

		return dev

	def undo(self):
		pass

class Resize(object):
	def __init__(self, dev, size=None):
		self.dev = dev
		self.size = None if not size else util.human_size(size)

	def run(self):
		# util.run("e2fsck", "-fy", self.dev)
		args = [
			"resize2fs",
			self.dev]

		actual_resize = None
		if self.size:
			size = (self.size / 1024)
			args += ["%dK" % size]
			actual_resize = size * 1024

		util.run(*args)

		return actual_resize

	def undo(self):
		pass

def size(fs):
	out = util.run("dumpe2fs", "-h", fs)
	rows = csv.reader(
		out.strip().split('\n')[1:],
		delimiter=":",
		skipinitialspace=True)

	md = {}
	for row in rows:
		md[row[0].lower()] = row[1]

	return int(md["block count"]) * int(md["block size"])

def is_mounted(file):
	cryptdev = crypt.mapper(crypt.dev_name_for(file))
	return _is_dev_mounted(cryptdev)

def _is_dev_mounted(cryptdev):
	return cryptdev in open("/proc/self/mounts").read()
