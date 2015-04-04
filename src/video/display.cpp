#include "display.h"

#include <QDebug>
#include <QThread>
#include <QMutexLocker>


Display* Display::getInstance()
{
    static Display instance;

    return &instance;
}

Display::Display()
    : refcount(0)
    , workerThread(nullptr)
    , worker(nullptr)
{
    worker = new DisplayWorker(0);
    workerThread = new QThread();

    worker->moveToThread(workerThread);

    connect(workerThread, &QThread::started, worker, &DisplayWorker::onStart);
    connect(workerThread, &QThread::finished, worker, &DisplayWorker::deleteLater);
    connect(worker, &DisplayWorker::newFrameAvailable, this, &Display::onNewFrameAvailable);

    workerThread->start();
}

Display::~Display()
{
    workerThread->exit();
    workerThread->deleteLater();
}

void Display::subscribe()
{
    if (refcount++ <= 0)
        worker->resume();
}

void Display::unsubscribe()
{
    if (--refcount <= 0)
    {
        worker->suspend();
        refcount = 0;
    }
}

void Display::onNewFrameAvailable(const VideoFrame &frame)
{
    emit frameAvailable(frame);

    mutex.lock();
    currFrame = frame;
    mutex.unlock();
}

VideoFrame Display::getLastFrame()
{
    QMutexLocker lock(&mutex);
    return currFrame;
}
