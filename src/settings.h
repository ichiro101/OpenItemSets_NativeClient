#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>
#include <QDir>
#include <QFile>
#include <vector>

class Settings {
public:
  static Settings& getInstance()
  {
    static Settings instance;
    return instance;
  }

  bool hasSettings();

  // getters and setters
  QString getUserId();
  QString getUsername();
  void setUserId(QString id);
  void setUsername(QString username);

  std::vector<QString> getLoLDirs();
  QString getStateHash();
  void setStateHash(QString hashStr);

private:

  Settings();
  Settings(Settings const&); // don't implement
  void operator=(Settings const&); // don't implement


  // internal methods for reading/writing data
  bool readSettings();
  void writeSettings();


  // internal helper methods
  QString settingsFile();

  QString _userId;
  QString _username;
  QString _stateHash;
  bool _hasSettings;
  std::vector<QString> _lolDirs;
};

#endif // SETTINGS_H
