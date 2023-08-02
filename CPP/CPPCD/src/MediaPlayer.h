//
// Created by SkyRain on 2023/06/08.
//

#ifndef YUNAPLAYERAPP_MEDIAPLAYER_H
#define YUNAPLAYERAPP_MEDIAPLAYER_H
#include <QAudioOutput>
#include <QCoreApplication>
#include <QMediaMetaData>
#include <QMediaPlayer>
#include <QObject>
#include <QQmlApplicationEngine>
#include <QQmlProperty>
#include <QMediaFormat>
#include <filesystem>
#include <iostream>
#include <QJsonArray>
#include <iostream>
#include <fstream>
#include <QFile>

#include "SList.h"
#include "bpbtable/bpptablemodel.h"

class MediaPlayer : public QObject {
  Q_OBJECT
private:
  QQmlApplicationEngine* engine;
  QObject* mainScreen, *songList;
  QMediaPlayer player;
  QAudioOutput audioOutput;
  SList<QVariantMap*> *sList, *filteredList;
  bool isReady = false;



public:
  explicit MediaPlayer(QQmlApplicationEngine* engine): QObject(engine), engine(engine){
    // Find mainScreen
    sList = new SList<QVariantMap*>();
    mainScreen = engine->rootObjects().first()->findChild<QObject* >("mainScreen");
    songList = engine->rootObjects().first()->findChild<QObject* >("songList");
    assert(mainScreen != nullptr && mainScreen != nullptr);
    // Load player
    player.setAudioOutput(&audioOutput);
    connect(&player, SIGNAL(positionChanged(qint64)), this, SLOT(positionChanged(qint64)));
    connect(&player, SIGNAL(durationChanged(qint64)), this, SLOT(durationChanged(qint64)));
    connect(&player, SIGNAL(metaDataChanged()), this, SLOT(onMetaInfoChanged()));
    connect(&player, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)), this, SLOT(playStateChanged(QMediaPlayer::MediaStatus)));
    audioOutput.setVolume(50);
    refreshList();
  }

  ~MediaPlayer(){
    delete sList;
  }
public:
  Q_INVOKABLE void onPlayButtonClick(bool play){
    // This function is called from QML Runtime
    if(play){
      qDebug() << "Playing";
      player.play();
    }else player.pause();
  };

  Q_INVOKABLE void addFile(QString filePath){
    // Add filepath
    qDebug() << "From " << QUrl(filePath).toLocalFile() << " To" << QCoreApplication::applicationDirPath()+"/song/" + QUrl(filePath).fileName();
    QFile::copy(QUrl(filePath).toLocalFile(), QCoreApplication::applicationDirPath()+"/song/" + QUrl(filePath).fileName());
    qDebug() << "Copy OK";
  }

  Q_INVOKABLE void deleteFile(int index){
      auto name = (*(*filteredList)[index])["file"];
      qDebug() << name.toString();
      QFile file(name.toString());
      file.remove();
  }

  Q_INVOKABLE void refreshList(){
      player.setSource(QUrl(""));
      int i=0;
      sList->clear();
      for(const auto& file : std::filesystem::directory_iterator((QCoreApplication::applicationDirPath()+ "/song/").toStdString())){
      isReady = false;
      player.setSource(QUrl::fromLocalFile(QString::fromStdString(file.path().generic_u8string())));
      auto *map = new QVariantMap();
      (*map)["sid"] = ++i;
      (*map)["file"] = QString::fromStdString(file.path().generic_u8string());
      while(!isReady);
      auto metaInfo = player.metaData();
      if(metaInfo.value(QMediaMetaData::Title).isValid())
      (*map)["name"] = metaInfo.stringValue(QMediaMetaData::Title);
      else (*map)["name"] = QString::fromStdString(file.path().filename().generic_u8string());
      if(metaInfo.value(QMediaMetaData::AlbumArtist).isValid()){
        (*map)["artist"] = metaInfo.stringValue(QMediaMetaData::AlbumArtist);
        (*map)["ci"] = metaInfo.stringValue(QMediaMetaData::AlbumArtist);
      }else{
        (*map)["artist"] = metaInfo.stringValue(QMediaMetaData::ContributingArtist);
        (*map)["ci"] = metaInfo.stringValue(QMediaMetaData::ContributingArtist);
      }
      (*map)["qu"] = metaInfo.stringValue(QMediaMetaData::AlbumTitle);
      (*map)["time"] = metaInfo.stringValue(QMediaMetaData::Date);
      (*map)["duration"] = static_cast<int>(metaInfo.value(QMediaMetaData::Duration).value<int>()/1000);
      for(auto& key : metaInfo.keys()){
        qDebug() << key << " " << metaInfo.value(key);
      }
      sList->push_back(map);
      }
  filteredList = sList;
  QQmlProperty::write(mainScreen, "currentSongIndex", filteredList->getSize()-1);

  }

  Q_INVOKABLE void onVolumeChanged(float volume){
    qDebug() << "Volume changed to " << volume << "%";
    audioOutput.setVolume(volume/100);
  };

  Q_INVOKABLE void onPlaySlideChanged(float seek){
    qDebug() << "Play time seeked to " << seek << "s";
    if(abs(player.position()/1000-seek) >= 1){
      // Seeked by human
      if(seek >= 0 && seek <= player.duration()/1000+1){
        player.setPosition(seek*1000);
      }else{
        qDebug() << "Invalid seek position: " << seek << "(0 to " << player.duration()/1000+1 << ")";
      }
    }
  };

  Q_INVOKABLE QVariantMap fillData(int i){
    if(filteredList->getSize() == 1) {
      auto k= (*filteredList)[0];
      qDebug() << k;
      return *(*filteredList)[0];
    }else{
      qDebug() << "Get" << *(*filteredList)[i];
      return *(*filteredList)[i];
    }
  }

  Q_INVOKABLE int getDataSize(){
    qDebug() << "Get " << filteredList->getSize();
    return filteredList->getSize();
  }

  Q_INVOKABLE void searchFor(const QString s){
    qDebug() << s ;
    if(s != ""){
      auto list = new SList<QVariantMap*>;
      for(int i=0; i<sList->getSize(); i++){
        auto* v = (*sList)[i];
        if((*v)["name"].value<QString>().contains(s)){
        list->push_back(v);
        qDebug() << "Searched " << (*v)["name"];
        }
      }
      filteredList = list;
      for(int i=0; i<filteredList->getSize(); i++){
        qDebug() << "Test: " << *(*filteredList)[i];
      }
    } else{
      filteredList = sList;
    }
  };

  Q_INVOKABLE void playMusic(int index){
    index += filteredList->getSize();
    index %= filteredList->getSize();
    auto url = (*filteredList)[index]->value("file").value<QString>();
    qDebug() << "PlayMusic: " << url;
    isReady = false;
    player.setSource(QUrl::fromLocalFile(url));
    while(!isReady);
    player.play();
    QQmlProperty::write(mainScreen, "currentSongIndex", index);
    QQmlProperty::write(mainScreen, "playing", true);
  }
public slots:
  void positionChanged(qint64 milisecond){
    // this function is called when playing.
      QQmlProperty::write(mainScreen, "currentPlayingTime", (int)(milisecond/1000));
  };

  void playStateChanged(const QMediaPlayer::MediaStatus status){
      switch (status) {
      case QMediaPlayer::EndOfMedia:{
      QQmlProperty::write(mainScreen, "playing", false);
      auto policy = QQmlProperty::read(mainScreen, "playPolicy").toInt();
      switch (policy){
        case 0:
        break;
        case 1:{
        auto index = QQmlProperty::read(mainScreen, "currentSongIndex").toInt();
        playMusic(index+1);
        player.play();
        break;
        }
        case 2:{
        auto index = QQmlProperty::read(mainScreen, "currentSongIndex").toInt();
        player.setPosition(0);
        QQmlProperty::write(mainScreen, "playing", true);
        player.play();
        }
        }
        break;
      }
      case QMediaPlayer::LoadedMedia:{
        //isReady = true;
        break;
      }
      }
  }

  void durationChanged(qint64 milisecond){
      qDebug() << "Duration has changed to " << milisecond << " ms";
      QQmlProperty::write(mainScreen, "totalPlayingTime", (int)(milisecond/1000));
  };

  void onMetaInfoChanged(){
      isReady = true;
      auto metaInfo = player.metaData();
      qDebug() << metaInfo.value(QMediaMetaData::AudioCodec).toString() << " " << metaInfo.value(QMediaMetaData::AudioBitRate).value<int>() << " " <<
          metaInfo.value(QMediaMetaData::ThumbnailImage) << " " << metaInfo.value(QMediaMetaData::FileFormat) << " " << metaInfo.value(QMediaMetaData::Composer);
      QQmlProperty::write(mainScreen, "songType", metaInfo.value(QMediaMetaData::AudioCodec).toString());
      QQmlProperty::write(mainScreen, "songBitrate", QString::number(metaInfo.value(QMediaMetaData::AudioBitRate).value<int>()/1000) + "kbps");
      QQmlProperty::write(mainScreen, "songChannel", metaInfo.value(QMediaMetaData::TrackNumber).toString());
      QQmlProperty::write(mainScreen, "songName", player.source().toString().split('/').last());
  }
};

#endif // YUNAPLAYERAPP_MEDIAPLAYER_H
