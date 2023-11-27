#include "SettingsUI.hpp"
#include "ui_SettingsUI.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QFile>

SettingsUI::SettingsUI(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SettingsUI)
{
    ui->setupUi(this);

    settings = new QSettings("Settings.ini", QSettings::IniFormat);
    QFile file("Settings.ini");
    if(!file.exists()){
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        file.close();
        settings->setValue("General/pathBackup", "/sdcard/BackupSystem/");
    }
    pathBackup = settings->value("General/pathBackup").toString();
    ui->lineSaveDirectory->setText(pathBackup);
}

SettingsUI::~SettingsUI()
{
    delete settings;
    delete ui;
}


void SettingsUI::on_btnSave_clicked()
{
    pathBackup = ui->lineSaveDirectory->text();
    settings->setValue("General/pathBackup", pathBackup);
    settings->sync();

    QMessageBox msg;
    msg.setText("Saved =)");
    msg.exec();
}

