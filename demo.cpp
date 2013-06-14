#include "src/nmqueue.hpp"
#include "src/nmthread.hpp"
#include <iostream>

const long threadCount = 128;

class ThreadC : public NMThread
{
    public:
        long id;
    private:
        void run() override;
};

void ThreadC::run()
{
    std::cout<<id<<std::endl;
}

NMQueue<void*, 1024> queue;

ThreadC threads[threadCount];

void sender()
{
    queue.send(NULL, NULL);
}

void receiver()
{
    void* data;
    queue.receive(data, NULL);
}

int main(int argc, char** args)
{
    {
        long id=0;
        for(id=0;id<threadCount;++id)
        {
            threads[id].id=id;
            threads[id].start();
        }
        for(id=0;id<threadCount;++id)
        {
            threads[id].stop();
        }
    }
    return 0;
}