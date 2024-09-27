#include <iostream>
#include "queue"
#include <thread>
#include "mutex"
#include "queue"
#include "condition_variable"
std::mutex g_mutex;
// template <typename T>
#define BUFFER_SIZE 4096
std::condition_variable g_cv;
// int number = 0;
bool isWait = false;

std::queue<int> buff;

int produceData()
{
    int randomNumber = rand() * 1000;
    // std::this_thread::sleep_for(std::chrono::seconds(1));
    // std::cout << "the value of random number is " << randomNumber << std::endl;
    return randomNumber;
}

void consumeData(std::queue<int> &buff)
{
    // std::cout << "Consumer data - " << queue.front() << std::endl;
    while (!buff.empty())
    {
        int num = buff.front();
        buff.pop();
        // std::this_thread::sleep_for(std::chrono::seconds(1000));
        std::cout << "Consumer data - " << num << std::endl;
        // std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // Simulate processing time
    }
}

void producerThread()
{
    while (1)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Simulate work

        std::unique_lock<std::mutex> lock(g_mutex);

        int number = produceData();
        buff.push(number);
        std::cout << "Produced value: " << number << std::endl;
        // std::cout << "size of buff is " << buff.size() << std::endl;
        if (buff.size() > 1)
        {
            // std::cout << "inside if " << std::endl;
            isWait = true;
            g_cv.notify_all();
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
        g_mutex.unlock();
        // std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        // g_cv.wait(ul, []() { return isWait == false; });
        // while (isWait)
        // {
        //     std::this_thread::sleep_for(std::chrono::seconds(1));
        // }
    }
}

void consumerThread()
{
    while (1)
    {
        // while (!isWait)
        // {
        //     std::this_thread::sleep_for(std::chrono::seconds(1));
        // }
        std::unique_lock<std::mutex> lock(g_mutex);

        g_cv.wait(lock, []()
                  { return isWait; });
        // while (queue.size() > 0)
        // {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        consumeData(buff);
        // std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        isWait = false;
        g_cv.notify_one();
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));

        // queue.pop();
        // }
    }
}

int main()
{
    // char buffer[BUFFER_SIZE];
    std::thread t1(producerThread);
    std::thread t2(consumerThread);
    t1.join();
    t2.join();
    return 0;
}


// consumerthread not able to consume data because there was stop for cosumerthread like 1 second but for producerthread there was no time so it is juct producing the data and after that consumerthread didn't get anytime to print any value