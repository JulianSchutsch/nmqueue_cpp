#ifndef _NMQUEUE_HEADERPP_
#define _NMQUEUE_HEADERPP_

#include <pthread.h>
#include <exception>

template <class Element, int Length>
class NMQueue
{
    private:
        Element[Length] queue;
        size_t          readPosition;
        size_t          writePosition;
        pthread_mutex_t mutex;
        pthread_cond_t  writtenCond;
        pthread_cond_t  readCond;

    public:
        NMQueue():readPosition(0),
                  writePosition(0)
        {
            if ( pthread_mutex_init( &mutex, NULL ) == 0 )
            {
                if ( pthread_cond_init( &writtenCond, NULL ) == 0 )
                {
                    if ( pthread_cond_init ( &readCond, NULL ) == 0 )
                    {
                        return;
                    }
                    //error = NMQUEUEERROR_CONDVAR_INITIALIZE_FAILED;
                    pthread_cond_destroy( &writtenCond);

                } else { /*error = NMQUEUEERROR_CONDVAR_INITIALIZE_FAILED;*/ }
                pthread_mutex_destroy( &mutex);

            } else { /*error = NMQUEUEERROR_MUTEX_INITIALIZE_FAILED;*/ }

            raise std::exception;
        }

        ~NMQueue()
        {
        }
}

#endif