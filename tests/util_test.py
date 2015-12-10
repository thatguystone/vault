from nose.tools import *

from vault import util

def test_human_size():
	def check(tin, tout):
		assert_equal(tout, util.human_size(tin))

	tests = [
		("10k", 10240),
		("10kb", 10240),
		("100k", 102400),
		("1m", 1048576),
		("10m", 10485760),
		("15M", 15728640),
		("15MB", 15728640),
		("1g", 1073741824),
		("1GB", 1073741824),
		("1T", 1099511627776),
		("1tb", 1099511627776),
	]

	for test in tests:
		yield check, test[0], test[1]
