#include "src/settings.h"

Settings::Settings() {
  QFile file(this->settingsFile());

  if (file.exists()) {
    // if we have settings file, then set this to true
    // TODO: verify its validity
    this->_hasSettings = true;

    // read the settings from the JSON file
    this->readSettings();
  } else {
    // if the file doesn't exist, we are probably
    // running this program for the first time.
    this->_hasSettings = false;
  }

}

void Settings::setUserId(QString userId) {
  this->_userId = userId;
}

QString Settings::getUserId() {
  return this->_userId;
}

void Settings::setUsername(QString username) {
  this->_username = username;
}

QString Settings::getUsername() {
  return this->_username;
}

QString Settings::settingsFile() {
  // check if openitemsets folder exists, if it doesn't
  // automatically create it
  QDir dir(QDir::homePath() + "/openitemsets/");
  if (!dir.exists()) {
      dir.mkpath(".");
  }

  return QDir::homePath() + "/openitemsets/settings.json";
}

QVector<QString> Settings::getLoLDirs() {
  return this->_lolDirs;
}

void Settings::setLolDirs(QVector<QString> lolDirs) {
  this->_lolDirs = lolDirs;
}

QString Settings::getStateHash() {
  return this->_stateHash;
}

void Settings::setStateHash(QString hashStr) {
  this->_stateHash = hashStr;
}

bool Settings::hasSettings() {
  return this->_hasSettings;
}

bool Settings::readSettings() {
  QFile file(this->settingsFile());

  if (!file.open(QIODevice::ReadOnly)) {
    qWarning("could not open settings file");
    return false;
  }

  QByteArray data = file.readAll();
  QJsonDocument loadDoc(QJsonDocument::fromJson(data));
  QJsonObject loadObject = loadDoc.object();

  this->setUserId(loadObject["userId"].toString());
  this->setUsername(loadObject["username"].toString());
  QJsonArray lolDirsJsonArray = loadObject["lolDirs"].toArray();
  QVector<QString> lolDirs;
  for (int i = 0; i < lolDirsJsonArray.count(); i++) {
    lolDirs.append(lolDirsJsonArray[i].toString());
  }
  this->setLolDirs(lolDirs);
  this->setStateHash(loadObject["stateHash"].toString());
  return true;
}

bool Settings::writeSettings() {
  QFile file(this->settingsFile());

  if (!file.open(QIODevice::WriteOnly)) {
    qWarning("could not open settings file");
    return false;
  }

  QJsonObject settingsObject;
  settingsObject["stateHash"] = this->getStateHash();
  QJsonArray lolDirsArray;
  for (QString currentDir: this->getLoLDirs()) {
    lolDirsArray.append(currentDir);
  }
  settingsObject["lolDirs"] = lolDirsArray;
  settingsObject["userId"] = this->getUserId();
  settingsObject["username"] = this->getUsername();
  QJsonDocument saveDoc(settingsObject);
  file.write(saveDoc.toJson());
  return true;
}
