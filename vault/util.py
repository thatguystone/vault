import logging
import sys
import subprocess

_suffixes = [
	[("k", "kb"), 0],
	[("m", "mb"), 0],
	[("g", "gb"), 0],
	[("t", "tb"), 0],
]

log = logging.getLogger(__name__)

for i, s in enumerate(_suffixes):
	if i == 0:
		s[1] = 1024
	else:
		s[1] = _suffixes[i-1][1] * 1024

def human_size(size):
	size = size.lower()
	for s in _suffixes:
		for e in s[0]:
			if size.endswith(e):
				size = size[:-len(e)]
				return int(size) * s[1]

	return int(size)

def to_bytes(obj):
	if obj and isinstance(obj, str):
		obj = obj.encode("utf-8")
	return obj

def _proc_name(args):
	if len(args) == 0:
		return "<unknown>"

	if len(args) >1 and args[0] == "sudo":
		return args[1]

	return args[0]

def run(*args, stdin=None):
	log.debug("exec: %s", args)

	p = subprocess.Popen(args,
		stdin=sys.stdin if not stdin else subprocess.PIPE,
		stdout=subprocess.PIPE,
		stderr=subprocess.STDOUT,
		close_fds=True)
	out = p.communicate(input=to_bytes(stdin))[0].decode("utf-8")

	if p.returncode != 0:
		raise RuntimeError("failed to execute {proc}, process said: {out}".format(
			proc=_proc_name(args),
			out=out))

	return out
