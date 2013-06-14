#ifndef _NMQUEUE_HEADERPP_
#define _NMQUEUE_HEADERPP_

#include <pthread.h>
#include <exception>
#include <iostream>

enum class NMQueueFailedInitCode {
    ConditionalVariable,
    Mutex
};

class NMQueueInitException : public std::exception
{
    private:
        NMQueueFailedInitCode code;
    public:
        NMQueueInitException(NMQueueFailedInitCode error):std::exception(),
                                                          code(error){}

        const char* what() const noexcept override
        {
            switch( code )
            {
                case NMQueueFailedInitCode::ConditionalVariable:
                    return "Failed to initialize conditional variable";
                case NMQueueFailedInitCode::Mutex:
                    return "Failed to initialize mutex";
            }
            return ""; // Should be unnecessary, since all cases are catched in the switch
        }
};

template <class Element, unsigned int Length>
class NMQueue
{
    private:
        Element         queue[Length];
        size_t          readPosition  = 0;
        size_t          writePosition = 0;
        void*           abortId       = nullptr;
        pthread_mutex_t mutex;
        pthread_cond_t  writtenCond;
        pthread_cond_t  readCond;

    public:
        NMQueue()
        {
            enum NMQueueFailedInitCode error;
            if ( pthread_mutex_init( &mutex, nullptr ) == 0 )
            {
                if ( pthread_cond_init( &writtenCond, nullptr ) == 0 )
                {
                    if ( pthread_cond_init ( &readCond, nullptr ) == 0 )
                    {
                        return;
                    }
                    error = NMQueueFailedInitCode::ConditionalVariable;
                    pthread_cond_destroy( &writtenCond);

                } else { error = NMQueueFailedInitCode::ConditionalVariable; }
                pthread_mutex_destroy( &mutex);

            } else { error = NMQueueFailedInitCode::Mutex; }

            throw NMQueueInitException( error );
        }

        ~NMQueue()
        {
            pthread_cond_destroy( &writtenCond );
            pthread_cond_destroy( &readCond );
            pthread_mutex_destroy( &mutex );
        }

        // Return true if send was successfull
        bool send(Element data,
                  void*   threadId) noexcept
        {

            pthread_mutex_lock( &mutex );
            if( abortId == threadId )
            {
                abortId = nullptr;
                pthread_mutex_unlock( &mutex );
                return false;
            }

            while( (writePosition+1) % Length == readPosition )
            {
                pthread_cond_wait( &readCond, &mutex );
                if( abortId == threadId )
                {
                    abortId = nullptr;
                    pthread_mutex_unlock( &mutex);
                    return false;
                }
            }

            queue[writePosition] = data;
            writePosition = (writePosition+1) % Length;
            pthread_cond_signal( &writtenCond );
            pthread_mutex_unlock( &mutex);

            return true;

        }

        bool receive(Element& data,
                     void*    threadId) noexcept
        {
            pthread_mutex_lock( &mutex );
            if( abortId == threadId )
            {
                abortId = nullptr;
                pthread_mutex_unlock( &mutex );
                return false;
            }

            while( readPosition == writePosition )
            {
                pthread_cond_wait( &writtenCond, &mutex );
                if( abortId == threadId )
                {
                    abortId = nullptr;
                    pthread_mutex_unlock( &mutex );
                    return false;
                }
            }

            readPosition = (readPosition+1) & Length;
            pthread_cond_signal( &readCond );
            pthread_mutex_unlock( &mutex );

            return true;

        }

        NMQueue( const NMQueue&& ) = delete;
        NMQueue( const NMQueue& ) = delete;
        NMQueue operator =  ( const NMQueue& ) = delete;
};

#endif