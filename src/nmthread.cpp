#include "nmthread.hpp"

const char * NMThreadStopStoppedException::what() const noexcept
{
    return "Thread already stopped";
}

const char * NMThreadRestartException::what() const noexcept
{
    return "Thread already started";
}

const char * NMThreadStartException::what() const noexcept
{
    return "Thread creation failed";
}

void* threadProc(void * threadDataPtr) noexcept
{
    ((NMThread*)( threadDataPtr ))->run();
    return nullptr;
}

void NMThread::start()
{
    if(started)
    {
        throw NMThreadRestartException();
    }
    started    = true;

    if( pthread_create( &thread, nullptr, threadProc, this ) != 0)
    {
        started = false;
        throw NMThreadStartException();
    }
}

void NMThread::stop()
{
    if(!started)
    {
        throw NMThreadStopStoppedException();
    }

    pthread_join( thread , nullptr );

    started = false;
}

NMThread::NMThread()
{
}

NMThread::~NMThread()
{
    if(started)
    {
        stop();
    }
}
