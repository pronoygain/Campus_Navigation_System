#include "Queue.h"

IntQueue::IntQueue()
{
    frontIndex = 0;
    rearIndex = 0;
    count = 0;
}

bool IntQueue::enqueue(int value)
{
    if (count == CAPACITY)
    {
        return false;
    }

    data[rearIndex] = value;
    rearIndex = (rearIndex + 1) % CAPACITY;
    count++;
    return true;
}

int IntQueue::dequeue()
{
    if (empty())
    {
        return -1;
    }

    int value = data[frontIndex];
    frontIndex = (frontIndex + 1) % CAPACITY;
    count--;
    return value;
}

bool IntQueue::empty() const
{
    return count == 0;
}
