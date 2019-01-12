#ifndef MEDIAPLAYER_H
#define MEDIAPLAYER_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include "VLC_CameraWriter.h"
#include "VLC_VideoPlayer.h"
#include "IMediaPlayer.h"
#include <QImage>

class MediaPlayer: public IMediaPlayer{
    Q_OBJECT
public:
    explicit MediaPlayer(QWidget *parent = 0);
    ~MediaPlayer();
    void initUI();

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
    void onNewFrame();
private:
    void addRecordIcon(QImage & image);
    QLabel * m_videoWidget;
    QVBoxLayout * m_layout;
    VLC_CameraWriter m_videoWriter;
    VLC_VideoPlayer * m_videoPlayer;

    int m_recordDotCounter;
    bool m_needShowRecordDot;
    unsigned char * m_videoBuffer;
};

#endif // MEDIAPLAYER_H
