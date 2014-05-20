#include "settingsdialog.h"
#include "settings.h"
#include "ui_settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::SettingsDialog)
{
  ui->setupUi(this);

  // we want to display a message to the user
  // if they start up the program for the first time
  if (!Settings::getInstance().hasSettings()) {
    QMessageBox msgBox;

    // set up the text and the buttons
    msgBox.setText("Welcome! This looks like to be your first time running Open Item Sets");
    msgBox.setInformativeText("Please make sure the League of Legends installation path is correct, and the username is entered correctly");
    msgBox.setStandardButtons(QMessageBox::Ok);

    // display the message box
    msgBox.setModal(true);
    msgBox.exec();
  }
}

SettingsDialog::~SettingsDialog()
{
  delete ui;
}
