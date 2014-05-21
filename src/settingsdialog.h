#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QMessageBox>

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

private:
  Ui::SettingsDialog *ui;
};

#endif // SETTINGSDIALOG_H
