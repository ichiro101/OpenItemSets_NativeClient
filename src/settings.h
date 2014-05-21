#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>
#include <QDir>
#include <QFile>
#include <QVector>
#include <QSettings>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

class Settings {
public:
  static Settings& getInstance()
  {
    static Settings instance;
    return instance;
  }

  bool settingsChanged;
  bool hasSettings();

  // getters and setters
  QString getUserId();
  QString getUsername();
  QVector<QString> getLoLDirs();
  QString getStateHash();
  void setUserId(QString id);
  void setUsername(QString username);
  void setLolDirs(QVector<QString> lolDirs);
  void setStateHash(QString hashStr);

  // reading and writing data to disk
  bool readSettings();
  bool writeSettings();

private:

  Settings();
  Settings(Settings const&); // don't implement
  void operator=(Settings const&); // don't implement



  // internal helper methods
  QString settingsFile();

  QString _userId;
  QString _username;
  QString _stateHash;
  bool _hasSettings;
  QVector<QString> _lolDirs;
};

#endif // SETTINGS_H
