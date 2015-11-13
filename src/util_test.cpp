/**
 * @copyright 2015 Andrew Stone <a@stoney.io>
 * @license MIT
 */

#include <string>
#include "util.h"
#include "test_test.h"

namespace vault
{
namespace util
{

static struct {
	std::string in;
	bool ok;
	off_t out;
} _humanReadableTests[] = {
	{
	 .in = "15m",
	 .ok = true,
	 .out = 15728640,
	},
	{
	 .in = "1m",
	 .ok = true,
	 .out = 1048576,
	},
	{
	 .in = "1k",
	 .ok = true,
	 .out = 1024,
	},
	{
	 .in = "100k",
	 .ok = true,
	 .out = 102400,
	},
	{
	 .in = "1G",
	 .ok = true,
	 .out = 1073741824,
	},
	{
	 .in = "1T",
	 .ok = true,
	 .out = 1099511627776,
	},
	{
	 .in = "10000000000000000000000",
	 .ok = false,
	},
	{
	 .in = "10000000000T",
	 .ok = false,
	},
	{
	 .in = "15",
	 .ok = true,
	 .out = 15,
	},
	{
	 .in = "15z",
	 .ok = false,
	},
	{
	 .in = "",
	 .ok = false,
	},
};

VLTESTV(utilHumanReadableBytes, _humanReadableTests)
{
	pt_set_iter_name("%s", _t->in.c_str());

	off_t res;
	auto ok = humanReadableBytes(_t->in, &res);

	pt_eq(_t->ok, ok);
	if (ok) {
		pt_ueq(_t->out, res);
	}
}
}
}
