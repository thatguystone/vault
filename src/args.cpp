/**
 * @copyright 2015 Andrew Stone <a@stoney.io>
 * @license MIT
 */

#include "args.h"
#include <boost/algorithm/string.hpp>

namespace vault
{
namespace args
{

StdOutput Args::default_out_;

std::string StdOutput::trim(std::string str)
{
	boost::trim_right(str);
	return std::move(str);
}

void StdOutput::usage(TCLAP::CmdLineInterface &cmd)
{
	{
		printf("Usage:\n");
		std::stringstream ss;
		_shortUsage(cmd, ss);
		printf("%s %s\n", this->trim(ss.str()).c_str(),
			   this->extraUsage().c_str());
	}

	printf("\n");

	{
		printf("Arguments:\n");
		std::stringstream ss;
		_longUsage(cmd, ss);
		printf("%s\n", this->trim(ss.str()).c_str());
	}

	printf("\n");

	this->printExtras();
}

::vault::crypt::NewCreds NewCreds::asNewCreds()
{
	return ::vault::crypt::NewCreds();
}
}
}
