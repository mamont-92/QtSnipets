#include "VlcVideoPlayer.h"
#include <QDebug>
#include <stdlib.h>
#include <QStringList>
#include <QMutex>
#include <QImage>
#include <QThread>

static QString libvlc_param_interface = "-I";
static QString libvlc_param_dummy = "dummy";
static QString libvlc_param_ignoreConfig = "--ignore-config";
static QString libvlc_param_videoOut = "--vout";
static QString libvlc_param_videoMemory = "vmem";

static const int defaultFrameWidth = 768;
static const int defaultFrameHeight = 576;
static const int defaultBitsPerPixel = 4;

void * VlcVideoPlayer::lockCallback( void *data, void **p_pixels ){
    VlcVideoPlayer * videoPlayer = static_cast<VlcVideoPlayer *>(data);
    videoPlayer->lock();
    *p_pixels = videoPlayer->videoBuffer();

    return NULL;
}


void VlcVideoPlayer::unlockCallback( void *data, void *id, void *const *ipixels ){

    VlcVideoPlayer * videoPlayer = static_cast<VlcVideoPlayer *>(data);
    videoPlayer->unlock();
    videoPlayer->updateImage();
}

void VlcVideoPlayer::displayCallback( void *data, void *id ){
    (void) data;
}


unsigned char *VlcVideoPlayer::videoBuffer(){
    return m_videoPixelsARGB;
}

VlcVideoPlayer::VlcVideoPlayer(QObject *parent):QObject(parent){
    m_isPlaying = false;
    m_instance = NULL;
    m_mediaPlayer = NULL;
    m_media = NULL;
    m_sourcePath = "";

    m_videoWidth = defaultFrameWidth;
    m_videoHeight = defaultFrameHeight;
    m_bitsPerPixel = defaultBitsPerPixel;
    m_videoPixelsARGB = new unsigned char[m_videoWidth * m_videoHeight * m_bitsPerPixel];
}

void VlcVideoPlayer::updateImage(){
    emit newImage();
}

VlcVideoPlayer::VlcVideoPlayer(QString sourcePath, QObject *parent):QObject(parent){
    m_isPlaying = false;
    m_instance = NULL;
    m_mediaPlayer = NULL;
    m_media = NULL;

    m_videoWidth = defaultFrameWidth;
    m_videoHeight = defaultFrameHeight;
    m_bitsPerPixel = defaultBitsPerPixel;
    m_videoPixelsARGB = new unsigned char[m_videoWidth * m_videoHeight * m_bitsPerPixel];

    m_sourcePath = sourcePath;
}

void VlcVideoPlayer::setOptions(QStringList opt)
{
  m_listOptions = opt;
}

VlcVideoPlayer::~VlcVideoPlayer(){
    if(m_isPlaying)
        stop();
}

bool VlcVideoPlayer::isPlaying(){
    return m_isPlaying;
}

char ** VlcVideoPlayer::stringListToCharArray(QStringList list){
    QStringListIterator iter(list);
    int totalStringCount = list.count();
    char ** resultArray = new char * [totalStringCount];
    int curentStringCount = 0;
    while(iter.hasNext()){
        QString q_str = iter.next();
        char * c_str = new char [q_str.length()+1];
        strcpy(c_str, q_str.toStdString().c_str());
        resultArray[curentStringCount] = c_str;
        curentStringCount++;
    }
    return resultArray;
}

void VlcVideoPlayer::releaseCharArray(char ** resultArray, int count){
    for(int i=0; i < count; ++i){
        delete [] (resultArray[i]);
    }
    delete [] resultArray;
}

bool VlcVideoPlayer::isUrl(QString path){
    return (path.contains("rtsp:", Qt::CaseInsensitive)||path.contains("http:", Qt::CaseInsensitive)||path.contains("https:", Qt::CaseInsensitive)||path.contains("ftp:\\\\", Qt::CaseInsensitive));
}

bool VlcVideoPlayer::initVLC(){
    QStringList argList;
    /*argList.push_back(libvlc_param_interface);
    argList.push_back(libvlc_param_dummy);
    argList.push_back(libvlc_param_ignoreConfig);*/
    /*argList.push_back(libvlc_param_videoOut);
    argList.push_back(libvlc_param_videoMemory);*/
    argList.push_back("--no-xlib");
    argList.push_back("-q");
    argList.push_back("--no-video-title-show");
    argList.push_back("--quiet");
    argList.push_back("--ignore-config");
    argList.push_back("--no-xlib");
    argList.push_back("--vout");
    argList.push_back("vmem");
    argList.push_back("-I");
    argList.push_back("dumy");

    char ** vlc_args = stringListToCharArray(argList);

    this->m_instance = libvlc_new (argList.count(), vlc_args);

    releaseCharArray(vlc_args, argList.count());

    if(!m_instance){
        qDebug() << "VlcVideoPlayer::initVLC:can't init libvlc";
        return false;
    }

    if(isUrl(m_sourcePath))
        m_media = libvlc_media_new_location( m_instance,  m_sourcePath.toStdString().c_str());
    else
        m_media = libvlc_media_new_location( m_instance,  m_sourcePath.toStdString().c_str());
//        this->m_media  = libvlc_media_new_path( m_instance, m_sourcePath.toStdString().c_str() );
    if(!m_media){
        qDebug() <<"VlcVideoPlayer::initVLC:can't open media";
        return false;
    }

    foreach (QString option, m_listOptions) {
        libvlc_media_add_option(m_media, option.toStdString().c_str());
    }

    m_mediaPlayer = libvlc_media_player_new( m_instance );
    if(!m_mediaPlayer){
        qDebug() <<"VlcVideoPlayer::initVLC:can't create media player";
        return false;
    }

    libvlc_media_player_set_media( m_mediaPlayer, m_media );

    libvlc_video_set_format( m_mediaPlayer, "RV32", m_videoWidth, m_videoHeight,
                             m_videoWidth*m_bitsPerPixel );
    libvlc_video_set_callbacks( m_mediaPlayer, &VlcVideoPlayer::lockCallback,
                                &VlcVideoPlayer::unlockCallback,
                                &VlcVideoPlayer::displayCallback, this );

    return true;
}

void VlcVideoPlayer::lock(){
    m_videoMutex.lock();
}

void VlcVideoPlayer::unlock(){
    m_videoMutex.unlock();
}

void VlcVideoPlayer::start(){
    if (initVLC()) {
        m_isPlaying = true;
        qDebug("VlcVideoPlayer::start");
        libvlc_media_player_play(m_mediaPlayer);
     }
}

void VlcVideoPlayer::stop(){
    if (m_mediaPlayer) {
        m_isPlaying = false;
        qDebug("VlcVideoPlayer::stop");
        libvlc_media_player_stop(m_mediaPlayer);
    }
    releaseVLC();
}

void VlcVideoPlayer::releaseVLC(){
    if(m_mediaPlayer){
        libvlc_media_player_release( m_mediaPlayer );
        m_mediaPlayer = NULL;
    }
    if(m_media){
        libvlc_media_release(m_media);
        m_media = NULL;
    }
    if(m_instance){
        libvlc_release(m_instance);
        m_instance = NULL;
    }

}

QString VlcVideoPlayer::sourcePath(){
    return m_sourcePath;
}

void VlcVideoPlayer::setSourcePath(QString path){
    m_sourcePath = path;
}

int VlcVideoPlayer::videoWidth(){
    return m_videoWidth;
}

int VlcVideoPlayer::videoHeight(){
    return m_videoHeight;
}


