#include "displayworker.h"

#include <QTimer>
#include <QDebug>
#include <QThread>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <cstdint>
#include <cstring>
#include <vector>


DisplayWorker::DisplayWorker(int index)
    : clock(nullptr)
    , camIndex(index)
    , refCount(0)
{
    qRegisterMetaType<VideoFrame>();
    qRegisterMetaType<QList<QSize>>();
}

DisplayWorker::~DisplayWorker()
{
    if (clock)
        delete clock;
}

void DisplayWorker::onStart()
{
    clock = new QTimer(this);
    clock->setSingleShot(false);
    clock->setInterval(1000/60);

    connect(clock, &QTimer::timeout, this, &DisplayWorker::doWork);

    emit started();
}

void DisplayWorker::_suspend()
{
    qDebug() << "DisplayWorker: Suspend";
    clock->stop();
}

void DisplayWorker::_resume()
{
    qDebug() << "DisplayWorker: Resume";
    clock->start();
}

void DisplayWorker::doWork()
{
    int width = 0;
    int height = 0;
    int bitsPerPixels = 0;
    pixels.resize(0);
    imageFromDisplay(pixels, width, height, bitsPerPixels);

    QByteArray frameData(reinterpret_cast<char*>(pixels.data()), pixels.size());

    emit newFrameAvailable(VideoFrame{frameData, QSize(width, height), VideoFrame::BGR});
}

void DisplayWorker::imageFromDisplay(std::vector<uint8_t>& pixels, int& width, int& height, int& bitsPerPixel)
{
    Display* display = XOpenDisplay(nullptr);
    Window root = DefaultRootWindow(display);

    XWindowAttributes attributes = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    XGetWindowAttributes(display, root, &attributes);

    width = attributes.width;
    height = attributes.height;

    XImage* img = XGetImage(display, root, 0, 0 , width, height, AllPlanes, ZPixmap);
    bitsPerPixel = img->bits_per_pixel;
    pixels.resize(width * height * 4);

    memcpy(&pixels[0], img->data, pixels.size());

    XFree(img);
    XCloseDisplay(display);
}

void DisplayWorker::suspend()
{
    QMetaObject::invokeMethod(this, "_suspend");
}

void DisplayWorker::resume()
{
    QMetaObject::invokeMethod(this, "_resume");
}
