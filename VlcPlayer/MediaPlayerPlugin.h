#ifndef IMEDIAPLAYERPLUGIN_H
#define IMEDIAPLAYERPLUGIN_H

#include <QWidget>
#include "IMediaPlayerPlugin.h"
#include "IMediaPlayer.h"

class MediaPlayerPlugin : public IMediaPlayerPlugin {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.xraystudio.MediaPlayerPlugin" FILE "MediaPlayerPlugin.json")
    Q_INTERFACES(IMediaPlayerPlugin)
public:
    explicit MediaPlayerPlugin(QObject* parent = 0):IMediaPlayerPlugin(parent) {}
    ~MediaPlayerPlugin() {}
    IMediaPlayer * createPlayer(QWidget * parent);
    virtual QString version() { return QString("Interface"); }
};

#endif // IMEDIAPLAYERPLUGIN_H
