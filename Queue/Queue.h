#ifndef __QUEUE_H__
#define __QUEUE_H__

#include<stdlib.h>
#include<string.h>

template<typename T>
struct QueueNode
{
    T data;
    T *next;
};

template<typename T>
class Queue
{
protected:
    QueueNode<T> *head,*tail,*tmp;
    unsigned int cnt;
public:
    Queue():cnt(0),head(NULL),tail(NULL)
    {
    }

    bool push(T t)
    {
        tmp = new QueueNode<T>;
        if(tmp == NULL)
            return false;
        tmp -> data = t;
        tmp -> next = NULL;
        tail -> next = tmp;
        tail = tmp;
        ++cnt;
        return true;
    }

    void pop()
    {
        if(head ==  NULL)
        {
            return;
        }

        tmp = head -> next;
        delete head;
        head = tmp;
        --cnt;
    }

    T front()
    {
        if(head != NULL)
        {
            return head -> data;
        }
        else
        {
            return T(0);
        }
    }

    T back()
    {
        if(tail != NULL)
        {
            return tail -> data;
        }
        else
        {
            return T(0);
        }
    }

    unsigned int size()
    {
        return cnt;
    }

    bool empty()
    {
        return cnt == 0;
    }
};

template<typename T>
class CircleQueue
{
protected:
    unsigned int cnt,pos;
    T *dataPtr;
public:
    CircleQueue(unsigned int n):cnt(n)
    {
        dataPtr = new T[n];
        pos = 0;
        memset(dataPtr,0,n * sizeof(T));
    }

    void push(T t)
    {
        *(dataPtr + pos) = t;
        pos = (pos + 1) % cnt;
    }

    T front()
    {
        return dataPtr[(pos - 1 + cnt) % cnt];
    }

    T back()
    {
        return dataPtr[pos];
    }
};

template<typename T>
class CircleQueue_Avg
{
protected:
    unsigned int cnt,pos;
    T *dataPtr;
    T Avg;
public:
    CircleQueue_Avg(unsigned int n = 8):pos(0),cnt(n),Avg(0)
    {
        dataPtr = new T[n];
        memset(dataPtr,0,n * sizeof(T));
    }

    T front()
    {
        return dataPtr[(pos + cnt - 1) % cnt];
    }

    T back()
    {
        return dataPtr[pos];
    }

    void push(T t)
    {
        Avg -= back();
        *(dataPtr + pos) = t;
        pos = (pos + 1) % cnt;
        Avg += t;
    }

    T avg()
    {
        return Avg / cnt;
    }
};


#endif // __QUEUE_H__
