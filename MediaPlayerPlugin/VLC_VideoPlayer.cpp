#include "VLC_VideoPlayer.h"
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

static const int defaultFrameWidth = 1280;
static const int defaultFrameHeight = 720;
static const int defaultBitsPerPixel = 4;

void * VLC_VideoPlayer::lockCallback( void *data, void **p_pixels ){
    VLC_VideoPlayer * videoPlayer = static_cast<VLC_VideoPlayer *>(data);
    videoPlayer->lock();
    *p_pixels = videoPlayer->videoBuffer();

    return NULL;
}


void VLC_VideoPlayer::unlockCallback( void *data, void *id, void *const *ipixels ){

    VLC_VideoPlayer * videoPlayer = static_cast<VLC_VideoPlayer *>(data);
    videoPlayer->unlock();
    videoPlayer->updateImage();
}

void VLC_VideoPlayer::displayCallback( void *data, void *id ){
    (void) data;
}


unsigned char *VLC_VideoPlayer::videoBuffer(){
    return m_videoPixelsARGB;
}

VLC_VideoPlayer::VLC_VideoPlayer(QObject *parent):QObject(parent){
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

void VLC_VideoPlayer::updateImage(){
    emit newImage();
}

VLC_VideoPlayer::VLC_VideoPlayer(QString sourcePath, QObject *parent):QObject(parent){
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

VLC_VideoPlayer::~VLC_VideoPlayer(){
    if(m_isPlaying)
        stop();
}

bool VLC_VideoPlayer::isPlaying(){
    return m_isPlaying;
}

char ** VLC_VideoPlayer::stringListToCharArray(QStringList list){
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

void VLC_VideoPlayer::releaseCharArray(char ** resultArray, int count){
    for(int i=0; i < count; ++i){
        delete [] (resultArray[i]);
    }
    delete [] resultArray;
}

bool VLC_VideoPlayer::isUrl(QString path){
    return (path.contains("rtsp:", Qt::CaseInsensitive)||path.contains("http:", Qt::CaseInsensitive)||path.contains("https:", Qt::CaseInsensitive)||path.contains("ftp:\\\\", Qt::CaseInsensitive));
}

bool VLC_VideoPlayer::initVLC(){
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
        qDebug() << "VLC_VideoPlayer::initVLC:can't init libvlc";
        return false;
    }

    if(isUrl(m_sourcePath))
        this->m_media  = libvlc_media_new_location( m_instance,  m_sourcePath.toStdString().c_str());
    else
        this->m_media  = libvlc_media_new_path( m_instance, m_sourcePath.toStdString().c_str() );
    if(!m_media){
        qDebug() <<"VLC_VideoPlayer::initVLC:can't open media";
        return false;
    }

    this->m_mediaPlayer = libvlc_media_player_new( m_instance );
    if(!m_mediaPlayer){
        qDebug() <<"VLC_VideoPlayer::initVLC:can't create media player";
        return false;
    }

    libvlc_media_player_set_media( m_mediaPlayer, m_media );

    libvlc_video_set_format( m_mediaPlayer, "RV32", m_videoWidth, m_videoHeight, m_videoWidth*m_bitsPerPixel );
    libvlc_video_set_callbacks( m_mediaPlayer, &VLC_VideoPlayer::lockCallback, &VLC_VideoPlayer::unlockCallback, &VLC_VideoPlayer::displayCallback, this );

    return true;
}

void VLC_VideoPlayer::lock(){
    m_videoMutex.lock();
}

void VLC_VideoPlayer::unlock(){
    m_videoMutex.unlock();
}

void VLC_VideoPlayer::start(){
    //m_videoPixelsARGB = new unsigned char[m_videoWidth * m_videoHeight * m_bitsPerPixel];
    if(initVLC()){
        m_isPlaying = true;

        qDebug("VLC_VideoPlayer::start");
        libvlc_media_player_play(m_mediaPlayer);
     }
}

void VLC_VideoPlayer::stop(){
    if(m_mediaPlayer){
        m_isPlaying = false;
        qDebug("VLC_VideoPlayer::stop");
        libvlc_media_player_stop(m_mediaPlayer);
    }
    releaseVLC();
}

void VLC_VideoPlayer::releaseVLC(){
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

QString VLC_VideoPlayer::sourcePath(){
    return m_sourcePath;
}

void VLC_VideoPlayer::setSourcePath(QString path){
    m_sourcePath = path;
}

int VLC_VideoPlayer::videoWidth(){
    return m_videoWidth;
}

int VLC_VideoPlayer::videoHeight(){
    return m_videoHeight;
}


