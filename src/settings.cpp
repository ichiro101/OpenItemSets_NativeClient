#include "src/settings.h"

Settings::Settings() {
  QFile jsonFile(this->settingsFile());

  if (jsonFile.exists()) {
    // if we have settings file, then set this to true
    // TODO: verify its validity
    this->_hasSettings = true;
  } else {
    // if the file doesn't exist, we are probably
    // running this program for the first time.
    this->_hasSettings = false;
  }
}

QString Settings::settingsFile() {
  return QDir::homePath() + "/openitemsets/settings.json";
}

bool Settings::hasSettings() {
  return this->_hasSettings;
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
