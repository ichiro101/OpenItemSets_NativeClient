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
