#include "includes/mainwindow.h"
#include "includes/settings.h"
#include <QApplication>

#include <iostream>

// If settings are not found, then we have
// to run our setup wizard to for LoL
// directory detection
void runWizard();

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);

  if(Settings::getInstance().hasSettings()) {
    MainWindow w;
    w.show();
  } else {
    runWizard();
  }

  return a.exec();
}

void runWizard() {
  // TODO: code to run the wizard here...
}
