/**
 * @copyright 2015 Andrew Stone <a@stoney.io>
 * @license MIT
 */

#include <sys/stat.h>
#include "file.h"

namespace vault
{
namespace file
{

void Resizer::resize(File f, off_t size, bool randomize)
{
	struct stat st;
	OSError(stat(f.path_.c_str(), &st), "failed to stat");

	// @todo calculate with vault headers

	if (st.st_size == size) { // Cool. Nothing to do.
		return;
	}

	OSError(ftruncate(f, size), "failed to resize");

	if (st.st_size < size && randomize) {
		// @todo don't use st_size here with vault headers
		this->randomize(std::move(f), st.st_size, size);
	}
}

void Resizer::randomize(File f, off_t start, off_t end)
{
	char buff[1024 * 1024];
	File r("/dev/urandom");

	r.open(O_RDONLY);

	OSError(lseek(f, start, SEEK_SET), "failed to seek");

	Assert(end >= start, "cannot randomize, end is before start: %ld < %ld",
		   start, end);

	auto remaining = end - start;
	while (remaining > 0) {
		off_t size = std::min(remaining, (off_t)sizeof(buff));

		auto err = read(r, buff, remaining);
		OSError(err, "failed to read random");
		Assert(err == size, "failed to read %ld random bytes, got %ld", size,
			   err);

		err = write(f, buff, size);
		OSError(err, "failed to write random");
		Assert(err == size, "failed to write %ld random bytes, put %ld", size,
			   err);

		remaining -= size;
	}
}

void CreateOp::run(std::string path, off_t size, bool randomize)
{
	File f(path);

	f.create();
	this->resize(std::move(f), size, randomize);
}

void CreateOp::undo() noexcept
{
}
}
}
