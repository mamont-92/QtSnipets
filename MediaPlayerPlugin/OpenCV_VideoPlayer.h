#ifndef OPENCV_VIDEOPLAYER_H
#define OPENCV_VIDEOPLAYER_H

#include <QObject>
#include <QMutex>
#include <QThread>
#include <QImage>
#include <QWaitCondition>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

class OpenCV_VideoPlayer : public QThread{
    Q_OBJECT
public:
    OpenCV_VideoPlayer(QObject *parent = 0);
    ~OpenCV_VideoPlayer();

    QString sourcePath();
    bool setSourcePath(QString path);

    void play();
    void stop();
    bool isStopped() const;

signals:
    void newImage(QImage image);
protected:
    void run();
private:
       bool m_stop;
       QMutex m_mutex;
       //QWaitCondition m_condition;
       cv::Mat m_frame;
       int m_frameRate;
       cv::VideoCapture m_capture;
       cv::Mat m_RGBframe;
       QImage m_imgage;
       QString m_sourcePath;

};

#endif // OPENCV_VIDEOPLAYER_H
