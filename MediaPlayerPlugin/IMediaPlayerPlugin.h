#ifndef MEDIAPLAYERPLUGIN_H
#define MEDIAPLAYERPLUGIN_H

#include <QWidget>

#include "IMediaPlayer.h"
//class IMediaPlayer;

class IMediaPlayerPlugin:public QObject{
    Q_OBJECT
public:
    explicit IMediaPlayerPlugin(QObject* parent = 0):QObject(parent) {}
    virtual ~IMediaPlayerPlugin() {}
    virtual IMediaPlayer * createPlayer(QWidget * parent) = 0;
    virtual QString version() = 0;
};

Q_DECLARE_INTERFACE(IMediaPlayerPlugin, "com.xraystudio.IMediaPlayerPlugin")


#endif // IMEDIAPLAYERPLUGIN_H
