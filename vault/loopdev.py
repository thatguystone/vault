from . import util

class Open(object):
	def __init__(self, file):
		self.file = file

	def run(self):
		return util.run("sudo", "losetup", "--show", "-f", self.file).strip()

	def undo(self):
		Close(self.file).run()

class Close(object):
	def __init__(self, file):
		self.file = file

	def run(self):
		dev = find(self.file)
		if not dev:
			raise RuntimeError("%s does not have a loopdev" % self.file)
		util.run("sudo", "losetup", "--detach", dev)

def find(file):
	return util.run("sudo",
		"losetup",
		"-O", "NAME", "--noheadings",
		"--associated", file).strip()
