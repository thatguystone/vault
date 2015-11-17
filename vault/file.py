import os

from . import util

class Create(object):
	def __init__(self, file, size, randomize):
		self.file = file
		self.size = util.human_size(size)
		self.randomize = randomize

	def run(self):
		with open(self.file, "xb") as f:
			f.truncate(self.size)

	def undo(self):
		os.remove(self.file)

class Randomize(object):
	def __init__(self, file, size, randomize):
		self.file = file
		self.size = util.human_size(size)
		self.randomize = randomize

	def run(self):
		if not self.randomize:
			return

		with open(self.file, "wb") as f, open("/dev/urandom", "rb") as u:
			size = self.size
			while size > 0:
				s = min(1024*1024, size)
				f.write(u.read(s))
				size -= s

	def undo(self):
		pass
