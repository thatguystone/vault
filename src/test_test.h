/**
 * @copyright 2015 Andrew Stone <a@stoney.io>
 * @license MIT
 */

#pragma once
#include <paratec.h>
#include "std.h"

namespace vault
{
namespace test
{

const constexpr char *kTestDataDir = "test_data/";

/**
 * Basic setup and teardowns wrapping PARATEC
 */
#define VLTEST(name, ...)                                                      \
	PARATEC(name, PTUP(test::setup), PTDOWN(test::teardown), ##__VA_ARGS__)

/**
 * Basic setup and teardowns wrapping PARATECV
 */
#define VLTESTV(name, tvec, ...)                                               \
	PARATECV(name, tvec, PTUP(test::setup), PTDOWN(test::teardown),            \
			 ##__VA_ARGS__)

/**
 * Test version of humanReadableBytes().
 */
off_t bytes(const std::string &s);

/**
 * Test setup
 */
void setup();

/**
 * Test teardown
 */
void teardown();

/**
 * Get path for the given file in the test directory
 */
std::string file(const std::string &f);

/**
 * Like `testFile()`, but with formatting
 */
PRINTF(1, 2)
std::string filef(const char *format, ...);
}
}
