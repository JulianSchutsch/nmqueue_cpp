#include "src/nmqueue.hpp"
#include "src/nmthread.hpp"

#include <iostream>
#include <atomic>

#include <unistd.h>

const long receiverCount = 1;
const long senderCount   = 1;
const long sendCount     = 10000000;

NMQueue<void*, 10000> queue;

std::atomic<long> receivedPackets(0);

class SendThread : public NMThread
{
    public:
        void run() override;
};

class ReceiveThread : public NMThread
{
    public:
        long received = 0;
        void run() override;
};

void SendThread::run()
{
    for( long i=0; i<sendCount; ++i )
    {
        if(!queue.send(NULL, this))
        {
            std::cout<<"Premature abort."<<std::endl;
            break;
        }
    }
}

void ReceiveThread::run()
{
    void* data;
    while(queue.receive(data, this))
    {
        received++;
        receivedPackets++;
    }
}

SendThread    sendThreads[senderCount];
ReceiveThread receiveThreads[receiverCount];

int main(int, char**)
{
    for( long id = 0; id<receiverCount; ++id )
    {
        receiveThreads[id].start();
    }
    for( long id = 0; id<senderCount ; ++id )
    {
        sendThreads[id].start();
    }

    while( receivedPackets < sendCount*senderCount )
    {
        sleep(1);
    }
    std::cout<<"All packets received."<<std::endl;

    for( long id = 0; id<receiverCount; ++id )
    {
        std::cout<<"Thread "<<id<<" received "<<receiveThreads[id].received<<std::endl;
    }

    for( long id = 0; id<receiverCount; ++id )
    {
        queue.abortThread( &receiveThreads[id] );
        receiveThreads[id].stop();
    }
    for( long id = 0; id<senderCount; ++id )
    {
        queue.abortThread( &sendThreads[id] );
        sendThreads[id].stop();
    }
    return 0;
}
