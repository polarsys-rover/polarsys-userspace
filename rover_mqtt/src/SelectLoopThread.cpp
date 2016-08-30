#include <SelectLoopThread.hpp>
#include <unistd.h>
#include <iostream>

SelectLoopThread::SelectLoopThread(std::string name, int timeout_ms)
: m_name(name), m_timeout_ms(timeout_ms),
  m_quit_read_fd(-1), m_quit_write_fd(-1), m_quit(false),
  m_callbacks(), m_pollfds()
{
    int fds[2];

    int ret = pipe(fds);

    if (ret == 0) {
	m_quit_read_fd = fds[0];
	m_quit_write_fd = fds[1];

	CallbackType cb = std::bind(&SelectLoopThread::mark_quit, this);
	register_read_fd(m_quit_read_fd, cb);
    } else {
	perror("pipe");
    }
}

SelectLoopThread::~SelectLoopThread()
{
    close(m_quit_read_fd);
    close(m_quit_write_fd);
}

void SelectLoopThread::please_stop()
{
    std::cout << "Asking thread " << m_name << " to shut down." << std::endl;

    char c = '!';
    int ret = write(m_quit_write_fd, &c, sizeof(c));

    if (ret != sizeof(c)) {
	perror("write");
    }
}

void SelectLoopThread::mark_quit(void)
{
    char c;
    int ret = read(m_quit_read_fd, &c, sizeof(c));

    if (ret != sizeof(c)) {
	perror("read");
    }

    m_quit = true;
}

void SelectLoopThread::register_read_fd(int fd, std::function<void()> callback)
{
    struct pollfd pollfd;

    pollfd.fd = fd;
    pollfd.events = POLLIN;

    m_pollfds.push_back(pollfd);

    m_callbacks[fd] = callback;
}

void SelectLoopThread::operator ()(void) {
    init();

    while (!m_quit) {
	int num_ready = poll(m_pollfds.data(), m_pollfds.size(), m_timeout_ms);

	if (num_ready == -1) {
	    perror("poll");
	} else if (num_ready > 0) {
	    for (const auto& pollfd : m_pollfds) {
		if (pollfd.revents & POLLIN) {
		    /* There is data to read on this fd */
		    CallbackType cb = m_callbacks[pollfd.fd];
		    cb();
		}
	    }
	} else {
	    timeout();
	}
    }

    fini();

    std::cout << "Thread " << m_name << " shutting down." << std::endl;
}

const std::string &SelectLoopThread::getName(void)
{
    return this->m_name;
}
