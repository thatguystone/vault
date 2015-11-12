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

class ArgsOut : public TCLAP::StdOutput
{
	static ArgsOut self_;

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
	virtual void usage(TCLAP::CmdLineInterface &cmd);
};

class Args : public TCLAP::CmdLine
{
	static ArgsOut default_out_;

public:
	Args() : Args(&Args::default_out_)
	{
	}

	Args(TCLAP::CmdLineOutput *out) : TCLAP::CmdLine("", ' ', VERSION)
	{
		this->setOutput(out);
	}
};

class CredentialsArgs
{
protected:
	CredentialsArgs(Args *args, bool change)
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
	TCLAP::ValueArg<std::string> password_;

	CredentialsArgs(Args *args) : CredentialsArgs(args, false)
	{
	}

	Credentials asCredentials();
};

struct NewCredentialsArgs : CredentialsArgs {
	NewCredentialsArgs(Args *args, bool change) : CredentialsArgs(args, change)
	{
	}

	NewCredentials asNewCredentials();
};

struct _SizeArg {
	off_t bytes_ = 0;

	friend std::istream &operator>>(std::istream &input, _SizeArg &sa)
	{
		std::string s;
		input >> s;

		if (!humanReadableBytes(s, &sa.bytes_)) {
			throw TCLAP::ArgParseException("Failed to read size arg: " + s);
		}

		return input;
	}
};

template <> struct TCLAP::ArgTraits<_SizeArg> {
	typedef TCLAP::ValueLike ValueCategory;
};

struct SizeArg : public TCLAP::UnlabeledValueArg<_SizeArg> {
	uint64_t bytes_;

	SizeArg(Args *args)
		: TCLAP::UnlabeledValueArg<_SizeArg>(
			  "size",
			  "Size of vault. Values may either be numbers (for number "
			  "of bytes), or human-readable strings like \"15M\", "
			  "\"1G\", etc.",
			  true,
			  _SizeArg(),
			  "size",
			  *args)
	{
	}
};

struct FileArg : public TCLAP::UnlabeledValueArg<std::string> {
	FileArg(Args *args)
		: TCLAP::UnlabeledValueArg<std::string>("file",
												"vault file to use",
												true,
												"vault.vlt",
												"FILE.vlt",
												*args)
	{
	}
};
