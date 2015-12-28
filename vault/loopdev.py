from . import util

class Open(object):
	def __init__(self, file):
		self.file = file

	def run(self):
		return util.run("losetup", "--show", "-f", self.file).strip()

	def undo(self):
		Close(self.file).run()

class Close(object):
	def __init__(self, file):
		self.file = file

	def run(self):
		devs = find(self.file)
		if not devs:
			raise RuntimeError("{} does not have a loopdev".format(self.file))
		util.run("losetup", "--detach", *devs.split("\n"))

def find(file):
	out = util.run(
		"losetup",
		"--associated", file).strip()
	parts = out.split(":")

	if parts and parts[0].startswith("/dev/"):
		return parts[0]

	return ""
