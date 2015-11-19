import os

from . import util

class Create(object):
	def __init__(self, file, size, randomize):
		self.file = file
		self.size = util.human_size(size)
		self.randomize = randomize

	def run(self):
		with open(self.file, "xb") as f:
			_grow(f, self.size, self.randomize)

	def undo(self):
		os.remove(self.file)

class Resize(object):
	def __init__(self, file, size, randomize=True):
		self.file = file
		self.size = util.human_size(size)
		self.randomize = randomize

	def run(self):
		with open(self.file, "ab") as f:
			if self.size >= 0:
				_grow(f, self.size, self.randomize)
			else:
				# `size` is negative, so add
				total = os.path.getsize(self.file) + size
				f.truncate(total)

	def undo(self):
		pass

def _grow(f, size, randomize):
	if randomize:
		_randomize(f, size)
	else:
		f.truncate(size)

def _randomize(f, size):
	with open("/dev/urandom", "rb") as u:
		while size > 0:
			s = min(1024*1024, size)
			size -= f.write(u.read(s))
