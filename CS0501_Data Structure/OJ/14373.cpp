#include <bits/stdc++.h>
using namespace std;

template <class elemType>
class seqQueue
{
    private:
        elemType *elem;
        int maxSize;
        int front;
        int rear;

        void doubleSpace();
    
    public:
        seqQueue(int size = 10){
            elem = new elemType[size];
            maxSize = size;
            front = rear = 0;
        }
        ~seqQueue(){
            delete[] elem;
        }
        bool isEmpty(){
            return front == rear;
        }
        void enQueue(const elemType &x);
        elemType deQueue();
        elemType getHead(){
            return elem[(front + 1) % maxSize];
        }
};


template <class elemType>
elemType seqQueue<elemType>::deQueue()
{
    if (front == rear)
    {
        cout << front << " " << rear - front << endl;
        return elem[front];
    }
    else
    {
        front = (front + 1) % maxSize;
        cout << front << " " << rear - front << endl;
        return elem[front];
    }
        
}


template <class elemType>
void seqQueue<elemType>::enQueue(const elemType &x)
{
    if ((rear + 1) % maxSize == front)
        doubleSpace();
    rear = rear + 1 % maxSize;
    elem[rear] = x;
    cout << rear << " " << rear - front << endl;
}

template <class elemType>
void seqQueue<elemType>::doubleSpace()
{
    elemType *tmp = elem;
    elem = new elemType[2 * maxSize];
    for (int i = 0; i < maxSize; i++)
    {
        elem[i] = tmp[(front + i) % maxSize];
    }
    front = 0;
    rear = maxSize - 1;
    maxSize *= 2;
    delete tmp;
}


int main()
{
    long long s, n;
    cin >> s >> n;
    seqQueue<long long> q(s);
    long long op;
    while(n--)
    {
        cin >> op;
        switch (op)
        {
        case 0:
            int x;
            cin>>x;
            q.enQueue(x);
            break;
        case 1:
            q.deQueue();
        }
    }
    return 0;
}