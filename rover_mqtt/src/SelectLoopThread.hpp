#ifndef SELECTLOOPTHREAD_HPP
#define SELECTLOOPTHREAD_HPP

#include <thread>
#include <functional>
#include <map>
#include <vector>

#include <poll.h>

class SelectLoopThread {
public:
    SelectLoopThread(std::string name, int timeout_ms);
    virtual ~SelectLoopThread();

    /* Kindly ask the thread if it could cease it operation within some
       reasonable delay. */
    virtual void please_stop() final;

    /* Entry point for the thread execution (meant to be called by std::thread). */
    virtual void operator()(void) final;

protected:
    /*  */
    typedef std::function<void()> CallbackType;
    void register_read_fd(int fd, CallbackType callback);

    virtual void init(void) {}
    virtual void fini(void) {}

    /* Method called when the select/poll timeout expired.  Meant to be
       overridden by extending classes. */
    virtual void timeout(void) {}

private:
    SelectLoopThread(const SelectLoopThread &) = delete;
    void operator=(const SelectLoopThread &) = delete;

    /* Internal callback that sets m_quit. */
    void mark_quit(void);

    std::string m_name;
    int m_timeout_ms;

    int m_quit_read_fd, m_quit_write_fd;
    bool m_quit;

    std::map<int, CallbackType> m_callbacks;
    std::vector<struct pollfd> m_pollfds;
};

#endif /* SELECTLOOPTHREAD_HPP */
