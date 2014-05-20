#include "src/mainwindow.h"
#include "src/settings.h"
#include "src/settingsdialog.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  if (!Settings::getInstance().hasSettings()) {
    // if the settings havn't been set, we need show
    // the settings dialog
    SettingsDialog dialog;
    dialog.setModal(true);
    dialog.exec();
  }
}

MainWindow::~MainWindow()
{
  delete ui;
}
