#include "src/mainwindow.h"
#include "src/settings.h"
#include "src/settingsdialog.h"
#include "ui_mainwindow.h"
#include <cstdlib>

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

void MainWindow::on_actionOpen_Settings_triggered()
{
  SettingsDialog dialog;
  dialog.exec();
}

void MainWindow::on_actionExit_triggered()
{
  QApplication::exit();
}
