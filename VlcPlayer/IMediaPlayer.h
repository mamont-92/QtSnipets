#ifndef IMEDIAPLAYER_H
#define IMEDIAPLAYER_H

#include <QString>
#include <QWidget>

class IMediaPlayer: public QWidget{
    Q_OBJECT
public:
    explicit IMediaPlayer(QWidget *parent):QWidget(parent) {}
    virtual ~IMediaPlayer() {}

    virtual QString sourcePath()= 0;
    virtual QString destPath()= 0;
    virtual bool isPlaying()= 0;
    virtual bool isRecording()= 0;

    virtual void setSourcePath(QString)= 0;
    virtual void setDestPath(QString)= 0;
    virtual void setOptions(QStringList) = 0;

    virtual void play()= 0;
    virtual void stop()= 0;
    virtual void startRecord()= 0;
    virtual void stopRecord()= 0;
};

#endif // IMEDIAPLAYER_H
