#ifndef SETTINGS_H
#define SETTINGS_H

#include <string>
#include <vector>

class Settings {
public:
  static Settings& getInstance()
  {
    static Settings instance;
    return instance;
  }

  bool hasSettings();
  int getUserId();
  std::string getUsername();
  std::vector<std::string> getLoLDirs();

private:

  Settings() {  }
  Settings(Settings const&); // don't implement
  void operator=(Settings const&); // don't implement


  // internal methods for reading/writing data
  bool readSettings();
  void writeSettings();

  int userId;
  std::string username;
  std::vector<std::string> lolDirs;
};

#endif // SETTINGS_H
