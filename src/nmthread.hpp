#ifndef _NMTHREAD_HEADERPP_
#define _NMTHREAD_HEADERPP_

#include <pthread.h>
#include <exception>

class NMThreadStopStoppedException : public std::exception
{
    const char * what() const noexcept override;
};

class NMThreadRestartException : public std::exception
{
    const char * what() const noexcept override;
};

class NMThreadStartException : public std::exception
{
    const char * what() const noexcept override;
};

class NMThread
{
    private:
        pthread_t     thread;
        bool          started = false;

    public:
        virtual void run() = 0;
        void start();
        void stop();
        NMThread();
        virtual ~NMThread();

        friend void* threadProc(void*) noexcept;

};

#endif
