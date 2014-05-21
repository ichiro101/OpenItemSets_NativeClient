#include "src/mainwindow.h"
#include "src/settings.h"
#include "src/settingsdialog.h"
#include "ui_mainwindow.h"
#include <cstdlib>
#include <QDebug>

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
  timer->start(30000);
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::checkUpdates() {
  // check the state hash
  auto currentHash = getStateHash();

  if (currentHash == Settings::getInstance().getStateHash()) {
    // nothing has been updated since our state hash
    // matches
    return;
  }

  auto tm1 = QTime::currentTime();

  ui->listWidget->addItem(tm1.toString("[hh:mm:ss]") +
                          "Updates to item sets detected: updating now...");

  auto subList = getUserSubscription();
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
  }
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

void MainWindow::on_actionOpen_Settings_triggered()
{
  SettingsDialog dialog;
  dialog.exec();
}

void MainWindow::on_actionExit_triggered()
{
  QApplication::exit();
}
