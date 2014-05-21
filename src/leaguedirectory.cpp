#include "leaguedirectory.h"

#include <iostream>

static std::vector<QString> scanWindowsInstallations();

std::vector<QString> scanInstallations() {
  std::vector <QString> s;

#ifdef Q_OS_WIN
  return scanWindowsInstallations();
#endif

  // return an empty vector if we can't identify the operating system
  return s;
}

std::vector<QString> scanWindowsInstallations() {
  std::vector <QString> s;

  QString riotKey = "HKEY_CURRENT_USER\\Software\\Riot Games\\RADS";
  QSettings settings(riotKey, QSettings::NativeFormat);
  QString folder = settings.value("LocalRootFolder").toString();

  if (folder != "") {
    folder = processDirectory(folder);
    s.push_back(folder);
  }

  return s;
}

QString processDirectory(QString path) {
  if (path.endsWith("RADS")) {
    // to remove "/RADS" from the string
    path.chop(5);
  }

  return path;
}
