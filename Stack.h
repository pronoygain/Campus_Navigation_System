#ifndef STACK_H
#define STACK_H
//stack using array to store path history,graph traversal and route reconstruction
class PathStack
{
private:
    int data[1000];
    int topIndex;

public:
    PathStack();

    void push(int value);

    int pop();

    bool empty();

    void clear();
};

#endif