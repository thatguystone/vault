/**
 * @copyright 2015 Andrew Stone <a@stoney.io>
 * @license MIT
 */

#pragma once
#include <string>
#include <vector>
#include "crypt.h"
#include "error.h"

namespace vault
{

class Vault
{
	/**
	 * Path to vault file
	 */
	std::string path_;

public:
	Vault(std::string path) : path_(std::move(path))
	{
	}

	/**
	 * Create a new vault of the given size.
	 *
	 * @param mount_dir
	 *     Where to mount the newly created vault to
	 * @param size
	 *     How large the vault should be, in bytes
	 * @param new_creds
	 *     New credentials to use to setup the vault.
	 * @param randomize
	 *     If the newly-created file should be completely randomized.
	 * @param fs
	 *     Which filesystem to use.
	 * @param cipher_mode
	 *     Cipher mode to create vault with. Can be any mode supported by
	 *     cryptsetup, for example (as of late 2015): either "xts-plain64" or
	 *     "cbc-essiv:sha256" (with a hash of your choice instead of sha256).
	 * @param hash
	 *     Which hash function to use for PBKDF2.
	 */
	void create(std::string mount_dir,
				off_t size,
				sp<crypt::NewCreds> new_creds,
				bool randomize = true,
				const std::string &fs = "ext4",
				const std::string &cipher_mode = "xts-plain64",
				const std::string &hash = "sha1");

	void open();
	void close();
};
}
