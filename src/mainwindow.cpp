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

  // assume that dialog box information have been successfully entered...
  // we go into the normal update loop
  QTimer* timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()), this, SLOT(checkUpdates()));
  checkUpdates();
  timer->start(15000);
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::cleanLogs() {
  auto listWidget = ui->listWidget;
  while (listWidget->count() > 50) {
    delete listWidget->itemAt(0, 0);
  }
}

void MainWindow::checkUpdates() {
  cleanLogs();

  // this checkUpdates() requires internet connection, we may need to expect
  // internet to fail, hence we are putting these code into a try { } block
  // and try to catch it if Internet fails
  try {
    // check the state hash
    auto currentHash = getStateHash();

    if (currentHash == Settings::getInstance().getStateHash()
		&& Settings::getInstance().settingsChanged == false) {
      // nothing has been updated since our state hash
      // matches
      return;
    }

    auto tm1 = QTime::currentTime();

    ui->listWidget->addItem(tm1.toString("[hh:mm:ss]") +
                            "Updates to item sets detected or settings changed: updating now...");

    auto subList = getUserSubscription();

	// if a user unsubscribed from a file, make sure that file
	// gets deleted
	this->cleanUnusedFiles(subList);
    auto hasError = false;

    for (QString itemSetId : subList) {
      QString itemSetJson = getItemSet(itemSetId);
      if(!this->saveItemSet(itemSetJson, itemSetId)) {
        hasError = true;
      }
    }

    if(!hasError) {
      // if everything proceeded without errors
      Settings::getInstance().setStateHash(currentHash);

      // persist the state hash
      Settings::getInstance().writeSettings();
	  Settings::getInstance().settingsChanged = false;
    }
  } catch (int i) {
    if (i == NETWORK_EXCEPTION) {
      auto tm1 = QTime::currentTime();
      ui->listWidget->addItem(tm1.toString("[hh:mm:ss]") +
                              "Network error occured while trying to contact Open Item Sets server (Internet Down?)");
    } else {
      auto tm1 = QTime::currentTime();
      ui->listWidget->addItem(tm1.toString("[hh:mm:ss]") +
                              "Error occured while trying to update");
    }
  }

  // make sure the user gets to see the updated logs
  ui->listWidget->scrollToBottom();
}

bool MainWindow::saveItemSet(QString itemSetJson, QString itemSetId) {
  bool hasNoError = true;
  auto tm1 = QTime::currentTime();

  // before we write the itemSetJson to a file, we have to parse
  // the information, in particular, we need to know the champion
  QJsonDocument loadDoc = QJsonDocument::fromJson(itemSetJson.toUtf8());
  QJsonObject itemSetJsonObject = loadDoc.object();

  QString champion = itemSetJsonObject["champion"].toString();

  QVector<QString> lolDirs = Settings::getInstance().getLoLDirs();
  for (auto currentDir : lolDirs) {
    QString logEntry;

    QDir itemSetDir(currentDir + "/Config/Champions/" + champion + "/Recommended");
    if (!itemSetDir.exists()) {
      itemSetDir.mkpath(".");
    }

    QString path = itemSetDir.path() + "/ois_" + itemSetId + ".json";
    QFile file(path);
    if (file.open(QIODevice::WriteOnly)) {
      QTextStream out(&file);
      out << itemSetJson;
      file.close();

      logEntry = tm1.toString("[hh:mm:ss]")
          + "Successfully written item set for " + champion + " to " + currentDir;
    } else {
      logEntry = tm1.toString("[hh:mm:ss]")
          + "Failed to write item set for " + champion + " to " + currentDir;
      hasNoError = false;
    }

    // add the log entry
    ui->listWidget->addItem(logEntry);
  }

  return hasNoError;
}

void MainWindow::cleanUnusedFiles(std::vector<QString> itemSetList) {
  QVector<QString> lolDirs = Settings::getInstance().getLoLDirs();

  for (auto currentDir : lolDirs) {
    QDir itemSetDir(currentDir + "/Config/Champions/");
    auto entryList = itemSetDir.entryList();
    
    // iterating through every champion here
    // we need to go through EVERY champion directory to look for
    // files that are placed by Open Item Sets. If we find files with
    // ids that are not in the current itemSetList, then
    // we delete the file from the file system
    for (QString entry : entryList) {
      // we need to skip the "." and ".."
      if (entry == "." || entry == "..") {
        continue;
      }

      QDir entryDir(itemSetDir.absolutePath() + "/" + entry + "/Recommended/");

      auto itemSetEntryList = entryDir.entryList();

      for (QString itemSetEntry : itemSetEntryList) {
        // we need to skip the "." and ".."
        if (itemSetEntry == "." || itemSetEntry == "..") {
          continue;
        }

        QFile itemSetFile(itemSetDir.absolutePath() + "/" + itemSetEntry);

        if (itemSetEntry.startsWith("ois")) {
          // we need to strip out .json part
          itemSetEntry.chop(5);

          // remove ois_ from the start
          itemSetEntry.remove(0, 4);

          // if we can't find itemSetEntry in the itemSetList, then the itemSetEntry
          // should be removed
          if (std::find(itemSetList.begin(), itemSetList.end(), itemSetEntry) == itemSetList.end()) {
            auto tm1 = QTime::currentTime();

            // remove the file from the file system
            itemSetFile.remove();

            QString logEntry = tm1.toString("[hh:mm:ss]")
              + "Unsubscribed item set removed from " + entryDir.absolutePath();
            ui->listWidget->addItem(logEntry);
          }
        }
      }
    }
  }
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


void MainWindow::on_actionAbout_triggered()
{
  QMessageBox aboutBox;
  QString version = QString::number(VERSION_MAJOR)
      + "." + QString::number(VERSION_MINOR)
      + "." + QString::number(VERSION_BUILD);
  QString informativeText = "Version: " + version + "\n";

  aboutBox.setText("Open Item Sets");
  aboutBox.setInformativeText(informativeText);
  aboutBox.setStandardButtons(QMessageBox::Ok);
  aboutBox.setModal(true);
  aboutBox.exec();
}
