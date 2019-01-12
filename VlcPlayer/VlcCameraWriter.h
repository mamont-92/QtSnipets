#ifndef VLCCAMERAWRITER_H
#define VLCCAMERAWRITER_H

#include <QObject>
#include "vlc/vlc.h"

class VlcCameraWriter
{
public:
    explicit VlcCameraWriter();
    explicit VlcCameraWriter(QString sourcePath, QString destPath);
    ~VlcCameraWriter();
    void start();
    void stop();
    bool isRecording();

    QString sourcePath();
    QString destPath();
    void setSourcePath(QString);
    void setDestPath(QString);
    void setOptions(QStringList);
signals:

public slots:
private:
    bool initVLC();
    void releaseVLC();
    libvlc_instance_t *m_instance;
    libvlc_media_player_t *m_mediaPlayer;
    libvlc_media_t *m_media;
    bool m_isRecording;
    //int m_frameRate;
    QString m_sourcePath;
    QString m_destPath;
    QStringList m_listOptions;
};

#endif // VLCCAMERAWRITER_H
