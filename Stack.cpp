#include "Stack.h"

PathStack::PathStack()
{
    topIndex = -1;
}

void PathStack::push(int value)
{
    if (topIndex < 999)
    {
        topIndex++;

        data[topIndex] = value;
    }
}

int PathStack::pop()
{
    if (topIndex == -1)
    {
        return -1;
    }

    return data[topIndex--];
}

bool PathStack::empty()
{
    return topIndex == -1;
}

void PathStack::clear()
{
    topIndex = -1;
}