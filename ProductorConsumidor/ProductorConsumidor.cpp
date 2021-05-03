#include <iostream>
#include <queue>
#include <thread>
#include <omp.h>
#include <random>
#include <time.h>

using namespace std;
bool done(int myRank, int thread_count);
void enqueue(int dest, int msg);
void receiveMessage(int myRank);
void producerConsumer(int num_msg);
void sendMessage(int thread_count, int myRank);

struct messageOMP {
    int value, dest;
    messageOMP(int val, int dest) : value(val), dest(dest) {}
};

struct QueueOMP {
    std::queue<messageOMP> q_p;
    int queued; // size of send
    int unqueued; // dequeue
    int done_sending; // threads finished?

    QueueOMP() {
        queued = 0;
        unqueued = 0;
        done_sending = 0;
    }
};
QueueOMP* threads_queue;

bool done(int myRank, int thread_count) {
    int queueSize = threads_queue[myRank].queued - threads_queue[myRank].unqueued;
    return (!queueSize && (threads_queue[myRank].done_sending == thread_count));
}
void producerConsumer(int num_msg) {

    int myRank = omp_get_thread_num();
    int thread_count = omp_get_num_threads();


    for (int i = 0; i < num_msg; i++) {
        sendMessage(thread_count, myRank);
        receiveMessage(myRank);
    }
    threads_queue[myRank].done_sending++;

    while (!done(myRank, thread_count))
        receiveMessage(myRank);
}

void enqueue(int dest, int msg) {
    threads_queue[dest].q_p.push(messageOMP(msg, dest));
    threads_queue[dest].queued++;
}

void sendMessage(int thread_count, int myRank) {
    int msg = rand() % 10 + 1;
    int dest = rand() % thread_count;
#pragma omp critical
    cout << "Thread: " << myRank << " Mensaje: " << msg << '\n';
#pragma omp critical
    enqueue(dest, msg);
}

int dequeue(QueueOMP* cola, int rank) {
    if (cola[rank].q_p.front().dest == rank) {
        int val = cola[rank].q_p.front().value;
        cola[rank].q_p.pop();
        cola[rank].unqueued++;
        return val;
    }
}

void receiveMessage(int myRank) {
    int queueSize = threads_queue[myRank].queued - threads_queue[myRank].unqueued;
    int msg;
    if (!queueSize)
        return;
    else if (queueSize == 1)
#pragma omp critical
        msg = dequeue(threads_queue, myRank);
    else
        msg = dequeue(threads_queue, myRank);
    cout << "Thread: " << myRank << " Mensaje: " << msg << '\n';
}






int main()
{
    srand(time(NULL));
    omp_set_num_threads(4);
    int thread_count = omp_get_num_threads();
    threads_queue = new QueueOMP[thread_count];
    cout << thread_count << endl;
#pragma omp parallel num_threads(thread_count)
    {
        producerConsumer(10);
    }

    delete threads_queue;
}

