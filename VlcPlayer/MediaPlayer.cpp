#include "MediaPlayer.h"
#include <QDebug>
#include <QPainter>
#include <QThread>
#include <QStandardPaths>
#include "opencv2/opencv.hpp"

using namespace cv;

MediaPlayer::MediaPlayer(QWidget *parent):IMediaPlayer(parent)
{
    qDebug() << "Try to load vlc plugin";
    /*
    QString writablePath = QStandardPaths::writableLocation(
        QStandardPaths::AppDataLocation);
    qDebug() << writablePath;
    m_settings = new QSettings(QString("%1\\%2.ini").
        arg(writablePath, "vlcplayer"), QSettings::IniFormat, this);
    m_videoPlayer->setSourcePath(m_settings->value("sourcepath", "").toString());
    qDebug() << "Vlc Source Path: " << m_videoPlayer->sourcePath();*/
    initUI();
}

void MediaPlayer::initUI()
{
    m_videoWidget  = new QLabel();
    m_layout  = new QVBoxLayout();
    setLayout(m_layout);
    m_layout->addWidget(m_videoWidget);

    m_videoBuffer = NULL;
    m_videoPlayer = new VlcVideoPlayer(this);

    connect(m_videoPlayer, SIGNAL(newImage()), this, SLOT(onNewFrame()));

    m_recordDotCounter = 0;
    m_needShowRecordDot = false;
}

void MediaPlayer::onNewFrame()
{
    m_videoPlayer->lock();
    Mat img(m_videoPlayer->videoHeight(), m_videoPlayer->videoWidth(), CV_8UC4, m_videoPlayer->videoBuffer());
    m_cvVideoWriter.write(img);
    QImage image(m_videoPlayer->videoBuffer(), m_videoPlayer->videoWidth(), m_videoPlayer->videoHeight(), QImage::Format_ARGB32);
    QImage tempImage(image);

    if (!image.isNull()){
        if(isRecording()){
            addRecordIcon(tempImage);
        }
        m_videoWidget->setAlignment(Qt::AlignCenter);
        m_videoWidget->setPixmap(QPixmap::fromImage(tempImage).scaled(m_videoWidget->size(), Qt::KeepAspectRatio, Qt::FastTransformation));
    }
    m_videoPlayer->unlock();
}

void MediaPlayer::addRecordIcon(QImage & image)
{
    m_recordDotCounter++;
    if(m_recordDotCounter > 10){
        m_recordDotCounter = 0;
        m_needShowRecordDot = !m_needShowRecordDot;
    }
    if(m_needShowRecordDot){
        QPainter painter(&image);
        painter.setBrush(QBrush(Qt::red));
        painter.drawEllipse(40,40,25,25);
    }

}

QString MediaPlayer::sourcePath()
{
    return m_videoPlayer->sourcePath();
}

QString MediaPlayer::destPath()
{
    return m_videoWriter.destPath();
}

bool MediaPlayer::isPlaying()
{
    return m_videoPlayer->isPlaying();
}

bool MediaPlayer::isRecording()
{
    return m_videoWriter.isRecording();
}

void MediaPlayer::setSourcePath(QString path)
{
    if(isPlaying() || isRecording())
        stop();
    m_videoPlayer->setSourcePath(path);
    m_videoWriter.setSourcePath(path);
}

void MediaPlayer::setDestPath(QString path)
{
    m_videoWriter.setDestPath(path);
}

void MediaPlayer::setOptions(QStringList opt)
{
    m_videoPlayer->setOptions(opt);
    m_videoWriter.setOptions(opt);
}

void MediaPlayer::play()
{
    m_videoPlayer->start();
    m_videoBuffer = m_videoPlayer->videoBuffer();
}

void MediaPlayer::stop()
{
    m_videoPlayer->stop();
    m_videoWriter.stop();
}

void MediaPlayer::startRecord()
{
//    m_videoWriter.start();
    int codec = CV_FOURCC('X','2','6','4');
    double fps = 20.0;
    m_cvVideoWriter.open(m_videoWriter.destPath().toStdString(), codec, fps,
                         cvSize(m_videoPlayer->videoWidth(),
                                m_videoPlayer->videoHeight()),
                         true);
}

void MediaPlayer::stopRecord()
{
//    m_videoWriter.stop();
    m_cvVideoWriter.release();
}

MediaPlayer::~MediaPlayer()
{
    if(isPlaying() || isRecording()){
        stop();
    }
}
