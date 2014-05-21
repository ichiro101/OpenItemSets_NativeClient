#include "settingsdialog.h"
#include "settings.h"
#include "ui_settingsdialog.h"
#include "leaguedirectory.h"
#include <cassert>
#include <cstdlib>
#include <iostream>

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
    msgBox.setInformativeText("Please make sure the League of Legends installation path is correct"
                              ", and the username is entered correctly");
    msgBox.setStandardButtons(QMessageBox::Ok);

    // display the message box
    msgBox.setModal(true);
    msgBox.exec();

    auto installations = scanInstallations();

    for (auto folder : installations) {
      ui->listWidget->addItem(folder);
    }
  }
}

SettingsDialog::~SettingsDialog()
{
  delete ui;
}


void SettingsDialog::accept() {
  // check validity of input first
  auto validateResult = validateInput();

  if (validateResult) {
    std::cout << "validate success" << std::endl;
  } else {
    std::cout << "validate fail" << std::endl;
  }
}

void SettingsDialog::reject() {
  if (!Settings::getInstance().hasSettings()) {
    QMessageBox msgBox;

    // set up the text and the buttons
    msgBox.setText("You have not completed setting up, are you sure you wish to quit?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);

    // display the message box
    msgBox.setModal(true);
    int ret = msgBox.exec();

    switch(ret) {
    case QMessageBox::Yes:
      // quit the application
      std::exit(1);
      break;
    case QMessageBox::No:
      // close the dialog box, the user is sent back to the settings
      msgBox.close();
      break;
    default:
      // should not be here
      assert(false);
      break;
    }
  }
}

bool SettingsDialog::validateInput() {
  // code to validate input here...
  return false;
}
