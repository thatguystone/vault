/**
 * @copyright 2015 Andrew Stone <a@stoney.io>
 * @license MIT
 */

#include "test_test.h"
#include "util.h"

namespace vault
{
namespace test
{

static void _cleanup()
{
	int ierr = system(("rm -rf " + file("")).c_str());
	pt_eq(0, ierr);
}

off_t bytes(const std::string &s)
{
	off_t res;

	pt(util::humanReadableBytes(s, &res));

	return res;
}

void setup()
{
	int ierr;

	_cleanup();

	ierr = system(("mkdir -p " + file("")).c_str());
	pt_eq(0, ierr);
}

void teardown()
{
	_cleanup();
}

std::string file(const std::string &f)
{
	return std::string(kTestDataDir) + pt_get_name() + "/" + f;
}

std::string filef(const char *format, ...)
{
	va_list args;
	char buff[1024];

	va_start(args, format);

	vsnprintf(buff, sizeof(buff), format, args);
	auto ret = file(buff);

	va_end(args);

	return ret;
}
}
}
