#ifndef LEAGUEDIRECTORY_H
#define LEAGUEDIRECTORY_H

#include <QDir>
#include <QString>
#include <QSettings>
#include <vector>

// functions responsible for scanning
// and reviewing validity of provided League of Legends
// directory


// returns a list of QStrings from the Windows Registry (if
// we are on Windows)
std::vector<QString> scanInstallations();

// check if the league directory is valid
bool isValidLeagueDirectory(QString path);

// process the path into a directory path that we can easily use
// for the purpose of this application
QString processDirectory(QString path);

#endif // LEAGUEDIRECTORY_H
