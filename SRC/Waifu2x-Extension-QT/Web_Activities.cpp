﻿/*
    Copyright (C) 2020  Aaron Feng

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

    My Github homepage: https://github.com/AaronFeng753
*/
#include "mainwindow.h"
#include "ui_mainwindow.h"

/*
从网络下载文件到本地
*/
bool MainWindow::DownloadTo(QString OnlineLink,QString LocalPath)
{
    QString program = Current_Path+"/python_ext_waifu2xEX.exe";
    QProcess Downlad2;
    Downlad2.start("\""+program+"\" \""+OnlineLink+"\" download2 \""+LocalPath+"\"");
    while(!Downlad2.waitForStarted(500)&&!QProcess_stop) {}
    while(!Downlad2.waitForFinished(500)&&!QProcess_stop) {}
    return QFile::exists(LocalPath);
}
/*
判断与github的链接状态以告知用户是否可以禁用gitee
*/
void MainWindow::on_checkBox_BanGitee_clicked()
{
    if(ui->checkBox_BanGitee->isChecked())
    {
        if(isConnectivityTest_RawGithubusercontentCom_Running==false)
        {
            QtConcurrent::run(this, &MainWindow::ConnectivityTest_RawGithubusercontentCom);//后台运行网络测试,判断是否可以链接raw.githubusercontent.com
        }
    }
}
/*
Github连通性测试
*/
void MainWindow::ConnectivityTest_RawGithubusercontentCom()
{
    QMutex_ConnectivityTest_RawGithubusercontentCom.lock();
    isConnectivityTest_RawGithubusercontentCom_Running=true;
    //===
    QString OnlineAddress="https://raw.githubusercontent.com/AaronFeng753/Waifu2x-Extension-GUI/master/.github/ConnectivityTest_githubusercontent.txt";
    QString LocalAddress=Current_Path+"/ConnectivityTest_Waifu2xEX.txt";
    QFile::remove(LocalAddress);
    //===
    emit Send_TextBrowser_NewMessage(tr("Start testing if your PC can connect to raw.githubusercontent.com."));
    DownloadTo(OnlineAddress,LocalAddress);
    if(QFile::exists(LocalAddress))
    {
        emit Send_TextBrowser_NewMessage(tr("Detection complete, your PC can connect to raw.githubusercontent.com."));
    }
    else
    {
        emit Send_TextBrowser_NewMessage(tr("Detection complete, your PC cannot connect to raw.githubusercontent.com."));
        emit Send_Unable2Connect_RawGithubusercontentCom();
    }
    QFile::remove(LocalAddress);
    //===
    isConnectivityTest_RawGithubusercontentCom_Running=false;
    QMutex_ConnectivityTest_RawGithubusercontentCom.unlock();
}
/*
Github连通性测试--提示无法连接
*/
void MainWindow::Unable2Connect_RawGithubusercontentCom()
{
    QMessageBox *MSG_2 = new QMessageBox();
    MSG_2->setWindowTitle(tr("Notification"));
    MSG_2->setText(tr("It is detected that you are currently unable to connect to raw.githubusercontent.com, so enabling [Ban Gitee] will affect the software to automatically check for updates. It is recommended that you disable [Ban Gitee]."));
    MSG_2->setIcon(QMessageBox::Information);
    MSG_2->setModal(false);
    MSG_2->show();
}
