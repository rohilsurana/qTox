#ifndef DISPLAYWORKER_H
#define DISPLAYWORKER_H

#include <QObject>
#include <QList>
#include <QMap>
#include <QMutex>
#include <QQueue>
#include <QSize>

#include "opencv2/highgui/highgui.hpp"
#include "videosource.h"

class QTimer;

class DisplayWorker : public QObject
{
    Q_OBJECT
public:
    DisplayWorker(int index);
    ~DisplayWorker();
    void doWork();

    void suspend();
    void resume();
    void imageFromDisplay(std::vector<uint8_t>& pixels, int& width, int& height, int& bitsPerPixel);

public slots:
    void onStart();

signals:
    void started();
    void newFrameAvailable(const VideoFrame& frame);

private slots:
    void _suspend();
    void _resume();

private:
    void subscribe();
    void unsubscribe();

private:
    QMutex mutex;
    QQueue<cv::Mat3b> queue;
    QTimer* clock;
    std::vector<uint8_t> pixels;
    int camIndex;
    QMap<int, double> props;
    QList<QSize> resolutions;
    int refCount;
};

#endif // DISPLAYWORKER_H
