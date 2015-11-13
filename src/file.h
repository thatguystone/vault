/**
 * @copyright 2015 Andrew Stone <a@stoney.io>
 * @license MIT
 */

#pragma once
#include <fcntl.h>
#include "error.h"
#include "transaction.h"

namespace vault
{
namespace file
{

struct File {
	std::string path_;
	int fd_ = -1;

	File(std::string path) : path_(std::move(path))
	{
	}

	File(const File &) = delete;

	File(File &&o)
	{
		this->path_ = std::move(o.path_);
		this->fd_ = o.fd_;
		o.fd_ = -1;
	}

	~File()
	{
		if (this->fd_ != -1) {
			::close(this->fd_);
		}
	}

	/**
	 * To access this like a file. As you probably want to.
	 */
	inline operator int() const
	{
		return this->fd_;
	}

	void create()
	{
		this->fd_ = ::open(this->path_.c_str(),
						   O_WRONLY | O_CREAT | O_EXCL | O_CLOEXEC,
						   S_IRUSR | S_IWUSR);
		OSError(this->fd_, "failed to create file at %s", this->path_.c_str());
	}

	void open(int flags)
	{
		this->fd_ = ::open(this->path_.c_str(), flags | O_CLOEXEC);
		OSError(this->fd_, "failed to open file at %s", this->path_.c_str());
	}
};

class Resizer
{
	void randomize(File f, off_t start, off_t end);

public:
	void resize(File f, off_t size, bool randomize);
};

class CreateOp : public Transaction::Op, Resizer
{
public:
	void run(std::string path, off_t size, bool randomize);

	void undo() noexcept;
};
}
}
