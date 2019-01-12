#ifndef MEDIAPLAYER_H
#define MEDIAPLAYER_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include "VLC_CameraWriter.h"
#include "imediaplayer.h"
#include "OpenCV_VideoPlayer.h"
#include <QImage>

class MediaPlayer: public IMediaPlayer{
    Q_OBJECT
public:
    explicit MediaPlayer(QWidget *parent = 0);
    //explicit MediaPlayer();
    ~MediaPlayer();

    QString sourcePath();
    QString destPath();
    bool isPlaying();
    bool isRecording();

    void setSourcePath(QString path);
    void setDestPath(QString path);

    void play();
    void stop();
    void startRecord();
    void stopRecord();

private slots:
    void onNewFrame(QImage image);
private:
    void addRecordIcon(QImage & image);
    void initUI();
    QLabel * m_videoWidget;
    QVBoxLayout * m_layout;
    VLC_CameraWriter m_videoWriter;
    OpenCV_VideoPlayer * m_videoPlayer;

    int m_recordDotCounter;
    bool m_needShowRecordDot;
};

#endif // MEDIAPLAYER_H
