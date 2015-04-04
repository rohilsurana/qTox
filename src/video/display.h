#ifndef DISPLAY_H
#define DISPLAY_H

#include <QImage>
#include <QList>
#include <QMutex>
#include "vpx/vpx_image.h"
#include "opencv2/highgui/highgui.hpp"
#include "src/video/videosource.h"
#include "src/video/displayworker.h"

class Display;


class Display : public VideoSource
{
    Q_OBJECT
public:
    enum Prop : int {
        BRIGHTNESS = CV_CAP_PROP_BRIGHTNESS,
        SATURATION = CV_CAP_PROP_SATURATION,
        CONTRAST = CV_CAP_PROP_CONTRAST,
        HUE = CV_CAP_PROP_HUE,
        WIDTH = CV_CAP_PROP_FRAME_WIDTH,
        HEIGHT = CV_CAP_PROP_FRAME_HEIGHT,
    };

    ~Display();

    static Display* getInstance(); ///< Returns the global widget's Display instance
    VideoFrame getLastFrame();

    void setResolution(QSize res);
    QSize getCurrentResolution();

    void setProp(Prop prop, double val);
    double getProp(Prop prop);

    void probeProp(Prop prop);
    void probeResolutions();

    // VideoSource interface
    virtual void subscribe();
    virtual void unsubscribe();

signals:
    void resolutionProbingFinished(QList<QSize> res);
    void propProbingFinished(Prop prop, double val);

protected:
    Display();

private:
    int refcount; ///< Number of users suscribed to the Display
    VideoFrame currFrame;
    QMutex mutex;

    QThread* workerThread;
    DisplayWorker* worker;

private slots:
    void onNewFrameAvailable(const VideoFrame& frame);

};
#endif // DISPLAY_H
