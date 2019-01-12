#ifndef VLC_CAMERAWRITER_H
#define VLC_CAMERAWRITER_H

#include <QObject>
#include "vlc/vlc.h"

class VLC_CameraWriter{
public:
    explicit VLC_CameraWriter();
    explicit VLC_CameraWriter(QString sourcePath, QString destPath);
    ~VLC_CameraWriter();
    void start();
    void stop();
    bool isRecording();

    QString sourcePath();
    QString destPath();
    void setSourcePath(QString path);
    void setDestPath(QString path);
signals:

public slots:
private:
    bool initVLC();
    void releaseVLC();
    libvlc_instance_t *  m_instance;
    libvlc_media_player_t * m_mediaPlayer;
    libvlc_media_t * m_media;
    bool m_isRecording;
    //int m_frameRate;
    QString m_sourcePath, m_destPath;

};

#endif // VLC_CAMERAWRITER_H
