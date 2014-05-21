#include "src/mainwindow.h"
#include "src/settings.h"
#include "src/settingsdialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  QSettings settings("Open Item Sets Project", "Open Item Sets");

  MainWindow mainWindow;
  mainWindow.show();

  return a.exec();
}
