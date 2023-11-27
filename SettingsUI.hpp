#ifndef SETTINGSUI_HPP
#define SETTINGSUI_HPP

#include <QMainWindow>
#include <QSettings>

namespace Ui {
class SettingsUI;
}

class SettingsUI : public QMainWindow
{
    Q_OBJECT

public:
    explicit SettingsUI(QWidget *parent = nullptr);
    ~SettingsUI();

private slots:
    void on_btnSave_clicked();

private:
    Ui::SettingsUI *ui;

    QSettings *settings;
    QString pathBackup;
};

#endif // SETTINGSUI_HPP
