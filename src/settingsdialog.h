#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include "settings.h"
#include "ui_settingsdialog.h"
#include "leaguedirectory.h"
#include <cassert>
#include <iostream>
#include <vector>
#include <QString>
#include <QDialog>
#include <QMessageBox>
#include <QFileDialog>

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
  Q_OBJECT

public:
  explicit SettingsDialog(QWidget *parent = 0);
  ~SettingsDialog();
  void accept();
  void reject();

  // validates input, returns true if the input on the
  // forms are considered valid, false if they are not
  bool validateInput();

public slots:
  void manualAdd();

private:

  Ui::SettingsDialog *ui;

  // errors
  std::vector<QString> errors;
};

#endif // SETTINGSDIALOG_H
