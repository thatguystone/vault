/**
 * @copyright 2015 Andrew Stone <a@stoney.io>
 * @license MIT
 */

#include "crypt.h"
#include "file.h"
#include "fs.h"
#include "transaction.h"
#include "vault.h"

namespace vault
{

void Vault::create(std::string mount_dir,
				   off_t size,
				   sp<crypt::NewCreds> new_creds,
				   bool randomize,
				   const std::string &fs,
				   const std::string &cipher_mode,
				   const std::string &hash)
{
	Transaction t;

	t.run<file::CreateOp>(this->path_, size, randomize);

	auto lo_dev = t.run<fs::LoMountOp>(this->path_);

	auto initial_creds = t.run<crypt::FormatOp>(lo_dev, cipher_mode, hash);

	t.run<crypt::UpdateCredsOp>(lo_dev, initial_creds, std::move(new_creds));

	auto crypt_dev
		= t.run<crypt::OpenOp>(lo_dev, this->path_, std::move(initial_creds));

	t.run<fs::FormatOp>(crypt_dev, fs);

	t.run<fs::MountOp>(crypt_dev, std::move(mount_dir));

	t.commit();
}
}
