#include "MediaPlayerPlugin.h"
#include "MediaPlayer.h"
#include <QDebug>

IMediaPlayer * MediaPlayerPlugin::createPlayer(QWidget  * parent){
    IMediaPlayer *player = new MediaPlayer(parent);
    return player;
}
