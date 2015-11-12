/**
 * @copyright 2015 Andrew Stone <a@stoney.io>
 * @license MIT
 */

#pragma once
#include <string>
#include <vector>
#include "error.h"
#include "safe.h"

class Credentials
{
protected:
	Safe password_;

	/**
	 * How long to iterate on PBKDF2 function.
	 */
	uint64_t iteration_ms = 1000;

public:
	/**
	 * Request credentials from the user as necessary
	 */
	Credentials() = default;
};

class NewCredentials : Credentials
{

public:
	/**
	 * Request and verify credentials from the user as necessary.
	 */
	NewCredentials() = default;
};

class Vault
{
	std::string path_;
	Credentials creds_;

	Error truncate(off_t size, bool randomize);

public:
	Vault(std::string path, Credentials creds)
		: path_(std::move(path)), creds_(std::move(creds))
	{
	}

	/**
	 * Create a new vault of the given size.
	 *
	 * @param size
	 *     How large the vault should be, in bytes
	 * @param creds
	 *     Credentials to use to authorize with the vault.
	 * @param randomize
	 *     If the newly-created file should be completely randomized.
	 * @param cipher_mode
	 *     Cipher mode to create vault with. Can be any mode supported by
	 *     cryptsetup, for example (as of late 2015): either "xts-plain64" or
	 *     "cbc-essiv:sha256" (with a hash of your choice instead of sha256).
	 * @param hash
	 *     Which hash function to use for PBKDF2.
	 */
	Error create(off_t size,
				 NewCredentials creds,
				 bool randomize = true,
				 const std::string &cipher_mode = "xts-plain64",
				 const std::string &hash = "sha1");

	Error open();
	Error close();
};
