/**
 * @copyright 2015 Andrew Stone <a@stoney.io>
 * @license MIT
 */

#pragma once
#include "std.h"

/**
 * Turn a string from, for example, "15m" to bytes.
 */
bool humanReadableBytes(std::string &s, off_t *res);
