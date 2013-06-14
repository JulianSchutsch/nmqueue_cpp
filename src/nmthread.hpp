#ifndef _NMTHREAD_HEADERPP_
#define _NMTHREAD_HEADERPP_

#include <pthread.h>

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
        ~NMThread();

        friend void* threadProc(void*) noexcept;

};

#endif