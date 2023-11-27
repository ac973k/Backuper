#include "BackuperUI.hpp"
#include "ui_BackuperUI.h"

#include <QFile>
#include <QMessageBox>

#include <QtCore/private/qandroidextras_p.h>

#include <QTextCodec>

bool checkPermission(QString chaine)
{
    auto result = QtAndroidPrivate::checkPermission(chaine).then([](
                                                                     QtAndroidPrivate::PermissionResult result) {
        return result;
    });
    result.waitForFinished();

    return result.result() != QtAndroidPrivate::PermissionResult::Denied;
}

bool requestPermission(QString chaine)
{
    auto result = QtAndroidPrivate::requestPermission(chaine).then([](
                                                                       QtAndroidPrivate::PermissionResult result) {
        return result;
    });
    result.waitForFinished();

    return result.result() != QtAndroidPrivate::PermissionResult::Denied;
}

BackuperUI::BackuperUI(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::BackuperUI)
{
    ui->setupUi(this);

    checkPermission("android.permission.READ_EXTERNAL_STORAGE");
    requestPermission("android.permission.READ_EXTERNAL_STORAGE");
    checkPermission("android.permission.WRITE_EXTERNAL_STORAGE");
    requestPermission("android.permission.WRITE_EXTERNAL_STORAGE");
    requestPermission("android.permission.MANAGE_DOCUMENTS");

    settingsUI = new SettingsUI();

    QFile fBackuper("dcfldd");
    if (!fBackuper.exists()) {
        downloader = new QNetworkAccessManager();

        QNetworkReply *reply;

        QString url = "https://github.com/ac973k/android-binaries/raw/master/dcfldd";

        // Создаём запрос
        QNetworkRequest request(url);
        // Обязательно разрешаем переходить по редиректам
        request.setAttribute(QNetworkRequest::RedirectionTargetAttribute, true);
        // Запускаем скачивание

        reply = downloader->get(request);

        QObject::connect(reply, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(onProgress(qint64, qint64)));
        QObject::connect(downloader, SIGNAL(finished(QNetworkReply *)), this, SLOT(onDownloadResult(QNetworkReply*)));
    }

    bkp = new QProcess;
    mkdir = new QProcess;

    settings = new QSettings("Settings.ini", QSettings::IniFormat);
    pathBackup = settings->value("General/pathBackup").toString();
}

BackuperUI::~BackuperUI()
{
    delete ui;
    delete settingsUI;
    delete downloader;
    delete bkp;
    delete mkdir;
    delete settings;
}


void BackuperUI::on_btnSettings_clicked()
{
    settingsUI->show();
}

void BackuperUI::onProgress(qint64 receivedSize, qint64 totalSize)
{
    ui->textEdit->clear();
    ui->textEdit->append("Загружено: " + QString::number(receivedSize/1024) + "KB / " + QString::number(totalSize/1024) + "KB");
}

void BackuperUI::onDownloadResult(QNetworkReply *replyD)
{
    QFile fBackuper("dcfldd");

    if(!replyD->error())
    {
        fBackuper.open(QFile::WriteOnly);
        fBackuper.write(replyD->readAll());
        fBackuper.flush();
    }
    else
    {
        ui->textEdit->append(replyD->errorString());
        replyD->abort();
        replyD->deleteLater();
    }

    fBackuper.close();

    replyD->deleteLater();

    ui->textEdit->append("Загрузка Завершена!");
    fBackuper.setPermissions(QFileDevice::ReadOwner | QFileDevice::WriteOwner | QFileDevice::ExeOwner |
                             QFileDevice::ReadUser | QFileDevice::ExeUser |
                             QFileDevice::ReadGroup | QFileDevice::ExeGroup);
}


void BackuperUI::on_btnBackup_clicked()
{
    mkdir->setProcessChannelMode(QProcess::MergedChannels);
    bkp->setProcessChannelMode(QProcess::MergedChannels);

    mkdir->start("su", QStringList() << "-c" << "mkdir " + pathBackup);
    mkdir->waitForStarted();

    QObject::connect(mkdir, &QProcess::readyReadStandardOutput, this, [this]() {
        while (mkdir->canReadLine()) {
            QByteArray BA = mkdir->readLine();
            QTextCodec *codec = QTextCodec::codecForName("UTF-8");
            QString line = codec->toUnicode(BA.data());
            ui->textEdit->append(line);
        }
    });



    QObject::connect(mkdir, &QProcess::finished, this, [this]() {
        ui->textEdit->append("Backuping...");
        bkp->start("su", QStringList() << "-c" << "./dcfldd if=/dev/block/bootdevice/by-name/" + ui->boxList->currentText() + " of=" + pathBackup + ui->boxList->currentText() + ".img" <<
                                         "hash=sha256 hashlog=" + pathBackup + ui->boxList->currentText() + ".sha256" <<
                                         "status=on");
        mkdir->waitForStarted();

        QObject::connect(bkp, &QProcess::readyReadStandardOutput, this, [this]() {
            while (bkp->canReadLine()) {
                QByteArray BA = bkp->readLine();
                QTextCodec *codec = QTextCodec::codecForName("UTF-8");
                QString line = codec->toUnicode(BA.data());
                ui->textEdit->append(line);
            }
        });
    });

    bkp->waitForFinished();
    connect(bkp, &QProcess::finished, this, [this]() {
        ui->textEdit->append("OK!");
    });
}

