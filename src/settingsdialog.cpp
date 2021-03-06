#include "settingsdialog.h"
#include <QDebug>

SettingsDialog::SettingsDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::SettingsDialog)
{
  ui->setupUi(this);

  if (!Settings::getInstance().hasSettings()) {
    // we want to display a message to the user
    // if they start up the program for the first time
    QMessageBox msgBox;

    // set up the text and the buttons
    msgBox.setText("Welcome! This looks like to be your first time running Open Item Sets");
    msgBox.setInformativeText("Please make sure the League of Legends installation path is correct"
                              ", and the username is entered correctly");
    msgBox.setStandardButtons(QMessageBox::Ok);

    // display the message box
    msgBox.setModal(true);
    msgBox.exec();

    // since it's our first time initializing, we should
    // also scan the computer for existing League of Legends
    // installations
    auto installations = scanInstallations();

    for (auto folder : installations) {
      ui->listWidget->addItem(folder);
    }
  } else {
    // read data from settings
    ui->usernameField->setText(Settings::getInstance().getUsername());
    for (auto currentDir : Settings::getInstance().getLoLDirs()) {
      ui->listWidget->addItem(currentDir);
    }
  }

  // add and remove directory events
  connect(ui->manualAddButton, SIGNAL(clicked()), this, SLOT(manualAdd()));
  connect(ui->removeButton, SIGNAL(clicked()), this, SLOT(removeDirectory()));
}

SettingsDialog::~SettingsDialog()
{
  delete ui;
}


void SettingsDialog::accept() {
  // check validity of input first
  auto validateResult = validateInput();


  if (validateResult) {
    // now we need to set the lolDirs in settings
    QVector<QString> lolDirs;
    for(int i = 0; i < ui->listWidget->count(); i++) {
      auto dirname = ui->listWidget->itemAt(0, i)->text();
      lolDirs.push_back(dirname);
    }

    Settings::getInstance().setLolDirs(lolDirs);

    if (Settings::getInstance().writeSettings()) {
      // succesfully wrote to settings, we can now close
      // the settings window
      QDialog::accept();
    } else {
      QMessageBox messageBox;
      messageBox.setText("Failed to save settings to file");
      messageBox.setStandardButtons(QMessageBox::Ok);
      messageBox.setModal(true);
      messageBox.exec();
    }
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
  } else {
    // do nothing if we already have Settings file
    QDialog::reject();
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
      selectedFolder = processDirectory(selectedFolder);
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

void SettingsDialog::removeDirectory() {
  // see if there is a selected item
  auto currentItems = ui->listWidget->selectedItems();
  if (currentItems.count() > 0) {
    delete currentItems.at(0);
  } else {
    QMessageBox messageDialog;
    messageDialog.setText("You must select a directory before you can remove it from the list.");
    messageDialog.setStandardButtons(QMessageBox::Ok);
    messageDialog.setModal(true);
    messageDialog.exec();
  }
}

bool SettingsDialog::validateInput() {
  // check if we at least one have directory in the widget
  if (ui->listWidget->count() == 0) {
    this->errors.push_back("You need to have at least one valid League of Legends directory on the list");
  }

  // check if the username is valid
  QString username = ui->usernameField->text();
  QString userId = getUserId(username);

  if (userId.startsWith("error")) {
    // the error message is contained in the return message of getUserId()
    if (userId == "error: user not found") {
      // put a more user friendly message
      this->errors.push_back("The Open Item Sets Username/Email you have entered doesn't seem to be valid.");
    } else {
      // some other error happend
      this->errors.push_back(userId);
    }
  } else {
    // since the username and userId are valid, let's set them
    // in our Settings class, which will be staged into a file
    Settings::getInstance().setUserId(userId);
    Settings::getInstance().setUsername(username);
  }


  // after we done validating, it's time to check if we have
  // errors from the validation
  if (this->errors.empty()) {
    // no errors found, return true
    return true;
  } else {

    // errors found, display all the errors we found to the user
    QMessageBox messageDialog;
    QString errorMessage = "";
    for(auto errorMessageItem : this->errors) {
      errorMessage += errorMessageItem + "\n";
    }
    messageDialog.setText(errorMessage);
    messageDialog.setStandardButtons(QMessageBox::Ok);
    messageDialog.setModal(true);
    messageDialog.exec();

    // clear the error messages after the error dialog has been shown
    // so they don't get readded
    this->errors.clear();
    return false;
  }
}
