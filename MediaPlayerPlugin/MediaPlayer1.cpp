#include "MediaPlayer.h"
#include <QDebug>
#include <QPainter>

/*MediaPlayer::MediaPlayer():QWidget(NULL){
    initUI();
}*/

MediaPlayer::MediaPlayer(QWidget *parent):IMediaPlayer(parent){
    initUI();
}

void MediaPlayer::initUI(){
    m_videoWidget  = new QLabel();
    m_layout  = new QVBoxLayout();
    setLayout(m_layout);
    m_layout->addWidget(m_videoWidget);

    m_videoPlayer = new OpenCV_VideoPlayer(this);
    connect(m_videoPlayer, &OpenCV_VideoPlayer::newImage, this, &MediaPlayer::onNewFrame);

    m_recordDotCounter = 0;
    m_needShowRecordDot = false;
}

void MediaPlayer::onNewFrame(QImage image){
    if (!image.isNull()){
        if(isRecording()){
            addRecordIcon(image);
        }
        m_videoWidget->setAlignment(Qt::AlignCenter);
        m_videoWidget->setPixmap(QPixmap::fromImage(image).scaled(m_videoWidget->size(), Qt::KeepAspectRatio, Qt::FastTransformation));
    }
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
    return m_videoPlayer->isRunning();
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
    m_videoPlayer->play();
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
