#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTime>
#include <QTimer>
#include <QJsonObject>
#include <QJsonDocument>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

private slots:
  void on_actionOpen_Settings_triggered();

  void on_actionExit_triggered();
  void checkUpdates();


private:
  Ui::MainWindow *ui;

  // given the item set in JSON, save it in the correct directory/directories
  bool saveItemSet(QString itemSetJson, QString itemSetId);
};

#endif // MAINWINDOW_H
