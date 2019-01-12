#include "MediaPlayer.h"
#include <QDebug>
#include <QPainter>
#include <QThread>


MediaPlayer::MediaPlayer(QWidget *parent):IMediaPlayer(parent){
    initUI();
}

void MediaPlayer::initUI(){
    m_videoWidget  = new QLabel();
    m_layout  = new QVBoxLayout();
    setLayout(m_layout);
    m_layout->addWidget(m_videoWidget);

    m_videoBuffer = NULL;
    m_videoPlayer = new VLC_VideoPlayer(this);

    connect(m_videoPlayer, SIGNAL(newImage()), this, SLOT(onNewFrame()));

    m_recordDotCounter = 0;
    m_needShowRecordDot = false;
}

void MediaPlayer::onNewFrame(){
    m_videoPlayer->lock();
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

void MediaPlayer::addRecordIcon(QImage & image){
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

QString MediaPlayer::sourcePath(){
    return m_videoPlayer->sourcePath();
}

QString MediaPlayer::destPath(){
    return m_videoWriter.destPath();
}

bool MediaPlayer::isPlaying(){
    return m_videoPlayer->isPlaying();
}

bool MediaPlayer::isRecording(){
    return m_videoWriter.isRecording();
}

void MediaPlayer::setSourcePath(QString path){
    if(isPlaying() || isRecording())
        stop();
    m_videoPlayer->setSourcePath(path);
    m_videoWriter.setSourcePath(path);
}

void MediaPlayer::setDestPath(QString path){
    m_videoWriter.setDestPath(path);
}

void MediaPlayer::play(){
    m_videoPlayer->start();
    m_videoBuffer = m_videoPlayer->videoBuffer();
}

void MediaPlayer::stop(){
    m_videoPlayer->stop();
    m_videoWriter.stop();
}

void MediaPlayer::startRecord(){
    m_videoWriter.start();
}

void MediaPlayer::stopRecord(){
    m_videoWriter.stop();
}

MediaPlayer::~MediaPlayer(){
    if(isPlaying() || isRecording()){
        qDebug() << "yolo";
        stop();
    }
}
