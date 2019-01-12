#include "OpenCV_VideoPlayer.h"
#include <QDebug>
using namespace cv;

static const int defaultFrameRate = 20;

OpenCV_VideoPlayer::OpenCV_VideoPlayer(QObject *parent) : QThread(parent)
{

}


bool OpenCV_VideoPlayer::setSourcePath(QString path) {
    m_sourcePath = path;
    m_capture.open(path.toStdString());
    if (m_capture.isOpened()){
        m_frameRate = (int) m_capture.get(CV_CAP_PROP_FPS);
        if(!m_frameRate)
            m_frameRate = defaultFrameRate;
        return true;
    }
    else
        return false;
}

void OpenCV_VideoPlayer::play(){
    if (!isRunning()) {
        m_stop = false;
        start(LowPriority);
    }
}

void OpenCV_VideoPlayer::run(){
    int delay = (1000/m_frameRate);
    while(true){
        m_mutex.lock();
        bool stoped =  m_stop;
        m_mutex.unlock();
        if(stoped)
            break;

        if (!m_capture.read(m_frame))
        {
            m_stop = true;
        }
        if (m_frame.channels()== 3){
            cv::cvtColor(m_frame, m_RGBframe, CV_BGR2RGB);
            m_imgage = QImage((const unsigned char*)(m_RGBframe.data),
                              m_RGBframe.cols,m_RGBframe.rows,QImage::Format_RGB888);
        }
        else
        {
            m_imgage = QImage((const unsigned char*)(m_frame.data),
                                 m_frame.cols,m_frame.rows,QImage::Format_Indexed8);
        }
        emit newImage(m_imgage);
        this->msleep(delay);
    }
}

OpenCV_VideoPlayer::~OpenCV_VideoPlayer(){
    m_mutex.lock();
    m_stop = true;
    m_mutex.unlock();
    wait();
    m_capture.release();

}
void OpenCV_VideoPlayer::stop(){
    m_stop = true;
}

bool OpenCV_VideoPlayer::isStopped() const{
    return this->m_stop;
}

QString OpenCV_VideoPlayer::sourcePath(){
    return m_sourcePath;
}
