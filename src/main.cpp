#include "src/mainwindow.h"
#include "src/settings.h"
#include "src/settingsdialog.h"
#include <QApplication>

#include <iostream>

void openSettings();
void openMainWindow();

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);

  MainWindow mainWindow;
  mainWindow.show();

  return a.exec();
}
