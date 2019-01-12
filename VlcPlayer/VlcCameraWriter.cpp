#include "VlcCameraWriter.h"
#include <QDebug>
#include <stdlib.h>
#include <QStringList>

static QString libvlc_param_sout = "--sout=#std{access=file,mux=ps,dst=%out_file%}";
static QString out_file = "%out_file%";
static QString libvlc_param_interface = "-I";
static QString libvlc_param_dummy = "dummy";
static QString libvlc_param_ignoreConfig = "--ignore-config";

static QString defaultDestPath = "D:\\work\\7711.mp4";  //TO DO: change for system variables, for examplae %temp%
static QString defaultSourcePath = "rtsp://admin:hg34mpdF88@192.168.88.209"; //

VlcCameraWriter::VlcCameraWriter(){
    m_isRecording = false;
    m_instance = NULL;
    m_mediaPlayer = NULL;
    m_media = NULL;

    m_sourcePath = defaultSourcePath;
    m_destPath = defaultDestPath;
}

VlcCameraWriter::VlcCameraWriter(QString sourcePath, QString destPath){
    m_isRecording = false;
    m_instance = NULL;
    m_mediaPlayer = NULL;
    m_media = NULL;

    m_sourcePath = sourcePath;
    m_destPath =  destPath;
}

VlcCameraWriter::~VlcCameraWriter(){
    if(m_isRecording)
        stop();
}

bool VlcCameraWriter::isRecording(){
    return m_isRecording;
}

char ** stringListToCharArray(QStringList list){
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

void releaseCharArray(char ** resultArray, int count){
    for(int i=0; i < count; ++i){
        delete [] (resultArray[i]);
    }
    delete [] resultArray;
}

bool isUrl(QString path){
    return (path.contains("rtsp:", Qt::CaseInsensitive)||path.contains("http:", Qt::CaseInsensitive)||path.contains("https:", Qt::CaseInsensitive)||path.contains("ftp:\\\\", Qt::CaseInsensitive));
}

bool VlcCameraWriter::initVLC()
{
    QString libvlc_param_sout_cur = libvlc_param_sout;
    libvlc_param_sout_cur = libvlc_param_sout_cur.replace(out_file, m_destPath);

    QStringList argList;
    argList.push_back(libvlc_param_interface);
    argList.push_back(libvlc_param_dummy);
    argList.push_back(libvlc_param_ignoreConfig);
    argList.push_back(libvlc_param_sout_cur);

    char ** vlc_args = stringListToCharArray(argList);

    m_instance = libvlc_new (argList.count(), vlc_args);

    releaseCharArray(vlc_args, argList.count());

    if(!m_instance){
        qDebug() << "VlcCameraWriter::initVLC:can't init libvlc";
        return false;
    }

    qDebug() << Q_FUNC_INFO << m_sourcePath;
    if(isUrl(m_sourcePath))
        m_media = libvlc_media_new_location( m_instance,  m_sourcePath.toStdString().c_str());
    else
        m_media = libvlc_media_new_location( m_instance,  m_sourcePath.toStdString().c_str());
        //m_media  = libvlc_media_new_path( m_instance, m_sourcePath.toStdString().c_str());
    if(!m_media) {
        qDebug() <<"VlcCameraWriter::initVLC:can't open media";
        return false;
    }

    foreach (QString option, m_listOptions) {
        libvlc_media_add_option(m_media, option.toStdString().c_str());
    }

    this->m_mediaPlayer = libvlc_media_player_new( m_instance );
    if (!m_mediaPlayer) {
        qDebug() <<"VlcCameraWriter::initVLC:can't create media player";
        return false;
    }

    libvlc_media_player_set_media( m_mediaPlayer, m_media );

    return true;
}

void VlcCameraWriter::setOptions(QStringList opt)
{
    m_listOptions = opt;
}

void VlcCameraWriter::start(){
    if(initVLC()){
        m_isRecording = true;
        qDebug("VlcCameraWriter::start");
        libvlc_media_player_play(m_mediaPlayer);
    }
}

void VlcCameraWriter::stop(){
    if (m_mediaPlayer) {
        m_isRecording = false;
        qDebug("VlcCameraWriter::stop");
        libvlc_media_player_stop(m_mediaPlayer);
    }
    releaseVLC();
}

void VlcCameraWriter::releaseVLC(){
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

QString VlcCameraWriter::sourcePath(){
    return m_sourcePath;
}

QString VlcCameraWriter::destPath(){
    return m_destPath;
}

void VlcCameraWriter::setSourcePath(QString path){
    m_sourcePath = path;
}

void VlcCameraWriter::setDestPath(QString path){
    m_destPath =  path;
}
