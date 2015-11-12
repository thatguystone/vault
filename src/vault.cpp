/**
 * @copyright 2015 Andrew Stone <a@stoney.io>
 * @license MIT
 */

#include <fcntl.h>
#include "vault.h"

Error Vault::truncate(off_t size, bool randomize)
{
	int ierr;
	Error err;

	ierr = ::truncate(this->path_.c_str(), size);
	err = OSError(ierr, "failed to set vault size");

	if (randomize) {
		throw "randomize not implemented yet";
	}

	return err;
}

Error Vault::create(off_t size,
					NewCredentials,
					bool randomize,
					const std::string &,
					const std::string &)
{
	Error err;

	int fd = ::open(this->path_.c_str(), O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);
	err = OSError(fd, "failed to create vault at %s", this->path_.c_str());

	if (err.ok()) {
		::close(fd);
		err = this->truncate(size, randomize);
	}

	return err;
}
