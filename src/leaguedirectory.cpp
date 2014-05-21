#include "leaguedirectory.h"

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
    if (isValidLeagueDirectory(folder)) {
      folder = processDirectory(folder);
      s.push_back(folder);
    }
  }

  return s;
}

bool isValidLeagueDirectory(QString path) {
  path = processDirectory(path);
  QDir dirToLeague(path);

  // check if config/champions is there
  if (dirToLeague.cd("Config")) {
    if (dirToLeague.cd("Champions")) {
      return true;
    } else {
      return false;
    }
  } else {
    return false;
  }
}

QString processDirectory(QString path) {
  if (path.endsWith("RADS")) {
    // to remove "/RADS" from the string
    path.chop(5);
  }

  // format the path
  QDir dir(path);
  path = dir.absolutePath();

  return path;
}
