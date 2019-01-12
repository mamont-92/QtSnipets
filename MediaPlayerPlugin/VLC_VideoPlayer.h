#ifndef VLC_VIDEOPLAYER_H
#define VLC_VIDEOPLAYER_H

#include <QObject>

#include "vlc/vlc.h"
#include <QMutex>

class VLC_VideoPlayer: public QObject{
    Q_OBJECT
public:
    explicit VLC_VideoPlayer(QObject * parent = 0);
    explicit VLC_VideoPlayer(QString sourcePath, QObject * parent = 0);
    ~VLC_VideoPlayer();
    void start();
    void stop();
    int videoWidth();
    int videoHeight();
    void lock();
    void unlock();
    unsigned char * videoBuffer();
    bool isPlaying();
    bool isUrl(QString path);

    QString sourcePath();

    void setSourcePath(QString path);
    void updateImage();

    static void * VLC_VideoPlayer::lockCallback( void *data, void **p_pixels );
    static void VLC_VideoPlayer::displayCallback( void *data, void *id );
    static void VLC_VideoPlayer::unlockCallback( void *data, void *id, void *const *ipixels );
signals:
    void newImage();

public slots:
private:
    bool initVLC();
    void releaseVLC();
    void releaseCharArray(char ** resultArray, int count);
    char ** stringListToCharArray(QStringList list);
    libvlc_instance_t *  m_instance;
    libvlc_media_player_t * m_mediaPlayer;
    libvlc_media_t * m_media;
    bool m_isPlaying;
    int m_frameRate;
    QString m_sourcePath;
    QMutex m_videoMutex;

    unsigned char * m_videoPixelsARGB;
    int m_videoWidth, m_videoHeight, m_bitsPerPixel;

};

#endif // VLC_VIDEOPLAYER_H
