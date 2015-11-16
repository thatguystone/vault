/**
 * @copyright 2015 Andrew Stone <a@stoney.io>
 * @license MIT
 */

#include <fcntl.h>
#include <poll.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include "exec.h"
#include "error.h"
#include "std.h"

namespace vault
{

PRINTF(2, 3) static void _assert(int err, const char *format, ...)
{
	if (err < 0) {
		char buff[8192];
		int eno = errno;

		va_list args;
		va_start(args, format);
		vsnprintf(buff, sizeof(buff), format, args);
		va_end(args);

		fprintf(stderr, "%s: %s", buff, strerror(eno));
		fflush(stderr);

		abort();
	}
}

static bool drain(int fd, std::string *out)
{
	int ierr;
	char buff[4096];

	while (true) {
		ierr = read(fd, buff, sizeof(buff));
		if (ierr <= 0) {
			if (ierr == -1 && (errno != EWOULDBLOCK && errno != EAGAIN)) {
				OSError(ierr, "failed to read from pipe");
			}

			return ierr == 0;
		}

		out->append(buff, ierr);
	}
}

static void _pipe(const std::string &what, int fds[2])
{
	auto ierr = pipe(fds);
	OSError(ierr, "failed to create %s pipes", what.c_str());

	ierr = fcntl(fds[0], F_SETFL, O_NONBLOCK);
	OSError(ierr, "failed to set %s stdout NONBLOCK", what.c_str());
}

std::string exec(const std::string &cmd, std::vector<std::string> args)
{
	const size_t total = args.size() + 1;

	int ierr;
	int stdout[2];
	int stderr[2];
	char const *argv[total];

	argv[total] = nullptr;
	for (size_t i = 0; i < args.size(); i++) {
		argv[i] = args[i].c_str();
	}

	_pipe("stdout", stdout);
	_pipe("stderr", stderr);

	auto pid = fork();
	OSError(pid, "failed to fork");

	if (pid == 0) {
		// Not allowed to read from stdin: it shouldn't be allowed to interact
		// with the user
		close(STDIN_FILENO);

		close(stdout[0]);
		close(stderr[0]);

		/*
		 * No exceptions are allowed in the child. Modifying parent memory
		 * could allow a secret to leak to swap since this process isn't
		 * mlock'd. The only thing allowed from here is an exec or an abort,
		 * neither of which touch parent pages and cause COWing.
		 */

		ierr = dup2(stdout[1], STDOUT_FILENO);
		_assert(ierr == STDOUT_FILENO, "failed to dup2 stdout");

		ierr = dup2(stderr[1], STDERR_FILENO);
		_assert(ierr == STDERR_FILENO, "failed to dup2 stderr");

		ierr = execvp(cmd.c_str(), (char *const *)argv);
		while (true) {
			_assert(-1, "failed to execvp(%s)", cmd.c_str());
		}
	} else {
		int status;
		std::string out;

		close(stdout[1]);
		close(stderr[1]);

		pollfd pfds[] = {
			{
			 .fd = stdout[0], .events = POLLIN,
			},
			{
			 .fd = stderr[0], .events = POLLIN,
			},
		};

		auto closed = false;
		while (!closed) {
			ierr = poll(pfds, NELS(pfds), -1);
			OSError(ierr, "failed to wait for child");

			for (auto pfd : pfds) {
				if ((pfd.revents | POLLIN)) {
					closed |= drain(pfd.fd, &out);
				}
			}
		}

		drain(stdout[0], &out);
		close(stdout[0]);

		drain(stderr[0], &out);
		close(stderr[0]);

		ierr = waitpid(pid, &status, 0);
		OSError(ierr, "failed to wait for child");
		Assert(!WIFSIGNALED(status), "child killed by signal %d, said: %s",
			   WTERMSIG(status), out.c_str());
		Assert(WIFEXITED(status) && WEXITSTATUS(status) == 0,
			   "child exited with status %d, said: %s", WEXITSTATUS(status),
			   out.c_str());

		return out;
	}
}
}
