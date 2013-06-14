#include "nmthread.hpp"

void* threadProc(void * threadDataPtr) noexcept
{
    ((NMThread*)( threadDataPtr ))->run();
    return nullptr;
}

void NMThread::start()
{
    if(started)
    {
        throw 1; // TODO
    }
    started    = true;

    if( pthread_create( &thread, nullptr, threadProc, this ) != 0)
    {
        started = false;
        throw 1; // TODO
    }
    
}

void NMThread::stop()
{
    if(!started)
    {
        throw 1; // TODO
    }
    pthread_join( thread , nullptr );
}

NMThread::NMThread()
{
}

NMThread::~NMThread()
{
}