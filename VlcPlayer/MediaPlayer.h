#ifndef MEDIAPLAYER_H
#define MEDIAPLAYER_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include "VlcCameraWriter.h"
#include "VlcVideoPlayer.h"
#include "IMediaPlayer.h"
#include <QImage>
#include <QSettings>

#include "opencv2/videoio.hpp"

class MediaPlayer: public IMediaPlayer
{
    Q_OBJECT
public:
    explicit MediaPlayer(QWidget *parent = 0);
    ~MediaPlayer();
    void initUI();

    QString sourcePath();
    QString destPath();
    bool isPlaying();
    bool isRecording();

    void setSourcePath(QString);
    void setDestPath(QString);
    void setOptions(QStringList);

    void play();
    void stop();
    void startRecord();
    void stopRecord();

private slots:
    void onNewFrame();
private:
    void addRecordIcon(QImage & image);
    QLabel *m_videoWidget;
    QVBoxLayout *m_layout;
    VlcCameraWriter m_videoWriter;
    VlcVideoPlayer *m_videoPlayer;
    QSettings *m_settings;
    int m_recordDotCounter;
    bool m_needShowRecordDot;
    unsigned char * m_videoBuffer;
    cv::VideoWriter m_cvVideoWriter;
};

#endif // MEDIAPLAYER_H
