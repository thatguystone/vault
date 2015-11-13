/**
 * @copyright 2015 Andrew Stone <a@stoney.io>
 * @license MIT
 */

#include "test_test.h"
#include "error.h"

namespace vault
{

template <typename T, typename Arg> static void mustCatch(Arg a)
{
	bool caught = false;
	try {
		T(a, "fail, baby, fail!");
	} catch (Error) {
		caught = true;
	}
	pt(caught);
}

VLTEST(errorBasic)
{
	Error(0, "don't fail");
	OSError(0, "don't fail");
	Assert(true, "don't fail");

	mustCatch<Error>(-1);
	mustCatch<Assert>(false);

	errno = EINVAL;
	mustCatch<OSError>(-1);
}
}
