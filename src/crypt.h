/**
 * @copyright 2015 Andrew Stone <a@stoney.io>
 * @license MIT
 */

#pragma once
#include "fs.h"
#include "safe.h"
#include "transaction.h"

namespace vault
{
namespace crypt
{

/**
 * Creds is used to access an existing vault.
 *
 * When you're done with this, be sure to free it immediately so that it can
 * be scrubbed.
 */
class Creds
{
protected:
	Safe password_;

	/**
	 * How long to iterate on PBKDF2 function. May only be set when creating
	 * new creds or when read from an existing vault.
	 */
	uint64_t iteration_ms = 1000;

public:
	/**
	 * Request creds from the user as necessary
	 */
	Creds() = default;
};

/**
 * Creds used to create a vault or change the password of an existing vault.
 *
 * When you're done with this, be sure to free it immediately so that it can
 * be scrubbed.
 */
class NewCreds : Creds
{
public:
	/**
	 * Request and verify creds from the user as necessary.
	 */
	NewCreds() = default;
};

/**
 * Format a raw device
 */
class FormatOp : public Transaction::Op
{
public:
	sp<Creds> run(const fs::Dev &dev,
				  const std::string &cipher_mode,
				  const std::string &hash);

	void undo() noexcept;
};

/**
 * Open a device
 */
class OpenOp : public Transaction::Op
{
public:
	fs::Dev
	run(const fs::Dev &dev, const std::string &orig_path, sp<Creds> creds);

	void undo() noexcept;
};

/**
 * Update credentials on a device
 */
class UpdateCredsOp : public Transaction::Op
{
public:
	void run(const fs::Dev &dev, sp<Creds> curr_creds, sp<NewCreds> new_creds);

	void undo() noexcept;
};
}
}
