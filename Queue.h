#ifndef QUEUE_H
#define QUEUE_H

// Circular array-backed queue (FIFO) used by breadth-first search.
class IntQueue
{
private:
    static const int CAPACITY = 1000;

    int data[CAPACITY];
    int frontIndex;
    int rearIndex;
    int count;

public:
    IntQueue();

    bool enqueue(int value);
    int dequeue();
    bool empty() const;
};

#endif
