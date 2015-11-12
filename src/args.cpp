/**
 * @copyright 2015 Andrew Stone <a@stoney.io>
 * @license MIT
 */

#include "args.h"
#include <boost/algorithm/string.hpp>

ArgsOut Args::default_out_;

std::string ArgsOut::trim(std::string str)
{
	boost::trim_right(str);
	return std::move(str);
}

void ArgsOut::usage(TCLAP::CmdLineInterface &cmd)
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

NewCredentials NewCredentialsArgs::asNewCredentials()
{
	return NewCredentials();
}
