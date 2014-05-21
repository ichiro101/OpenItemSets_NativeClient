#include "settingsdialog.h"
#include <QDebug>

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

  connect(ui->manualAddButton, SIGNAL(clicked()), this, SLOT(manualAdd()));
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

void SettingsDialog::manualAdd() {
  QFileDialog dialog;
  dialog.setFileMode(QFileDialog::Directory);
  dialog.setOption(QFileDialog::ShowDirsOnly);

  dialog.setModal(true);
  if (dialog.exec()) {
    auto selectedFolder = dialog.selectedFiles().at(0);

    if (isValidLeagueDirectory(selectedFolder)) {
      // if it's a valid league of legends directory, then
      // add it to the list

      // first check if the item is already in the list
      for(int i = 0; i < ui->listWidget->count(); i++) {
        if (ui->listWidget->itemAt(0, i)->text() == selectedFolder) {

          // if it's already in the list, tell the user that it's
          // already in the list
          QMessageBox messageDialog;
          messageDialog.setText("This directory is already in the list");
          messageDialog.setStandardButtons(QMessageBox::Ok);
          messageDialog.setModal(true);
          messageDialog.exec();
          return;
        }
      }

      // if it's not already in the list, then we should add it to the
      // listWidget
      ui->listWidget->addItem(selectedFolder);
    } else {
      // if it's not a valid league of legends directory, then
      // we show a dialog to the user
      QMessageBox messageDialog;
      messageDialog.setText("This does not look like a valid League of Legends directory");
      messageDialog.setStandardButtons(QMessageBox::Ok);
      messageDialog.setModal(true);
      messageDialog.exec();
    }
  }

}

bool SettingsDialog::validateInput() {
  // code to validate input here...
  return false;
}
