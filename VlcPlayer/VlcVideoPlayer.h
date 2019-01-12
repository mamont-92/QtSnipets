#ifndef VLCVIDEOPLAYER_H
#define VLCVIDEOPLAYER_H

#include <QObject>

#include "vlc/vlc.h"
#include <QMutex>

class VlcVideoPlayer: public QObject
{
    Q_OBJECT
public:
    explicit VlcVideoPlayer(QObject * parent = 0);
    explicit VlcVideoPlayer(QString sourcePath, QObject * parent = 0);
    ~VlcVideoPlayer();
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
    void setOptions(QStringList);
    void updateImage();

    static void *lockCallback( void *data, void **p_pixels );
    static void displayCallback( void *data, void *id );
    static void unlockCallback( void *data, void *id, void *const *ipixels );
signals:
    void newImage();

public slots:
private:
    bool initVLC();
    void releaseVLC();
    void releaseCharArray(char **resultArray, int count);
    char **stringListToCharArray(QStringList list);
    libvlc_instance_t *m_instance;
    libvlc_media_player_t *m_mediaPlayer;
    libvlc_media_t *m_media;
    bool m_isPlaying;
    int m_frameRate;
    QString m_sourcePath;
    QMutex m_videoMutex;

    unsigned char *m_videoPixelsARGB;
    int m_videoWidth;
    int m_videoHeight;
    int m_bitsPerPixel;
    QStringList m_listOptions;

};

#endif // VLCVIDEOPLAYER_H
