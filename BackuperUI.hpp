#ifndef BACKUPERUI_HPP
#define BACKUPERUI_HPP

#include <QMainWindow>

#include "SettingsUI.hpp"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QProcess>
#include <QSettings>

QT_BEGIN_NAMESPACE
namespace Ui { class BackuperUI; }
QT_END_NAMESPACE

class BackuperUI : public QMainWindow
{
    Q_OBJECT

public:
    BackuperUI(QWidget *parent = nullptr);
    ~BackuperUI();

private slots:
    void on_btnSettings_clicked();
    void onDownloadResult(QNetworkReply *replyD);

    void onProgress(qint64 receivedSize, qint64 totalSize);

    void on_btnBackup_clicked();

private:
    Ui::BackuperUI *ui;
    SettingsUI *settingsUI;

    QNetworkAccessManager *downloader;

    QProcess *bkp;
    QProcess *mkdir;

    QSettings *settings;
    QString pathBackup;
};
#endif // BACKUPERUI_HPP
