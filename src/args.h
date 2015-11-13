/**
 * @copyright 2015 Andrew Stone <a@stoney.io>
 * @license MIT
 */

#pragma once
#include <sstream>
#include <tclap/CmdLine.h>
#include "std.h"
#include "util.h"
#include "vault.h"

namespace vault
{
namespace args
{

class StdOutput : public ::TCLAP::StdOutput
{
	static StdOutput self_;

protected:
	static std::string trim(std::string str);

	virtual std::string extraUsage()
	{
		return "";
	}

	virtual void printExtras()
	{
	}

public:
	virtual void usage(::TCLAP::CmdLineInterface &cmd);
};

class Args : public ::TCLAP::CmdLine
{
	static StdOutput default_out_;

public:
	Args() : Args(&Args::default_out_)
	{
	}

	Args(::TCLAP::CmdLineOutput *out) : ::TCLAP::CmdLine("", ' ', VERSION)
	{
		this->setOutput(out);
	}
};

class Creds
{
protected:
	Creds(Args *args, bool change)
		: password_("",
					change ? "new-password" : "password",
					"Password to use. This should only be used for testing "
					"since it is rather insecure.",
					false,
					"",
					"password")
	{
		args->add(this->password_);
	}

public:
	::TCLAP::ValueArg<std::string> password_;

	Creds(Args *args) : Creds(args, false)
	{
	}

	::vault::crypt::Creds asCreds();
};

struct NewCreds : Creds {
	NewCreds(Args *args, bool change) : Creds(args, change)
	{
	}

	::vault::crypt::NewCreds asNewCreds();
};

struct _Size {
	off_t bytes_ = 0;

	friend std::istream &operator>>(std::istream &input, _Size &sa)
	{
		std::string s;
		input >> s;

		if (!util::humanReadableBytes(s, &sa.bytes_)) {
			throw ::TCLAP::ArgParseException("Failed to read size arg: " + s);
		}

		return input;
	}
};

struct Size : public ::TCLAP::UnlabeledValueArg<_Size> {
	uint64_t bytes_;

	Size(Args *args)
		: ::TCLAP::UnlabeledValueArg<_Size>(
			  "size",
			  "Size of vault. Values may either be numbers (for number "
			  "of bytes), or human-readable strings like \"15M\", "
			  "\"1G\", etc.",
			  true,
			  _Size(),
			  "size",
			  *args)
	{
	}
};

struct File : public ::TCLAP::UnlabeledValueArg<std::string> {
	File(Args *args)
		: ::TCLAP::UnlabeledValueArg<std::string>("file",
												  "vault file to use",
												  true,
												  "vault.vlt",
												  "FILE.vlt",
												  *args)
	{
	}
};
}
}

template <> struct TCLAP::ArgTraits<vault::args::_Size> {
	typedef ::TCLAP::ValueLike ValueCategory;
};
