/**
 * @copyright 2015 Andrew Stone <a@stoney.io>
 * @license MIT
 */

#include "util.h"

bool humanReadableBytes(std::string &s, off_t *res)
{
	int shift;
	char *endp;
	off_t num;

	errno = 0;
	num = strtoul(s.c_str(), &endp, 10);

	if (errno != 0 || endp == s.c_str()) {
		return false;
	}

	switch (*endp) {
	case 'k':
	case 'K':
		shift = 10;
		break;

	case 'm':
	case 'M':
		shift = 20;
		break;

	case 'g':
	case 'G':
		shift = 30;
		break;

	case 't':
	case 'T':
		shift = 40;
		break;

	case 0:
		shift = 0;
		break;

	default:
		return false;
	}

	if (num > (std::numeric_limits<off_t>::max() >> shift)) {
		return false;
	}

	*res = num << shift;

	return true;
}
