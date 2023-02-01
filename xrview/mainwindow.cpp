/*
 * XRechnung Viewer Suite
 *
 * Copyright (C) 2023 Klaas Freitag <opensource@freisturz.de>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "xrechnung.h"
#include "xrwidget.h"

#include <QFile>
#include <QFileInfo>
#include <QMessageBox>
#include <QTimer>
#include <QSettings>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , _maxIndx(0)
{
    ui->setupUi(this);
    _xrs.resize(10);

    ui->_mainToolBox->removeItem(0);

    connect(ui->_mainToolBox, &QToolBox::currentChanged, this, &MainWindow::slotShowDocumentNo);
    setWindowTitle("XRechnung Viewer");

    QTimer::singleShot(0, this, &MainWindow::checkConfig);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::checkConfig()
{
    QSettings config;
    bool bad{false};

    qDebug() << "Reading config file" << config.fileName();

    const QString jar = config.value("saxon/jar").toString();
    QFileInfo fiJar(jar);
    if (! (fiJar.exists() && fiJar.isReadable())) {
        bad = true;
    }
    const QString ubl = config.value("saxon/xslUbl").toString();
    QFileInfo fiUbl(ubl);
    if (! (fiUbl.exists() && fiUbl.isReadable())) {
        bad = true;
    }
    const QString html = config.value("saxon/xslHtml").toString();
    QFileInfo fiHtml(html);
    if (! (fiHtml.exists() && fiHtml.isReadable())) {
        bad = true;
    }

    if (bad) {
        QMessageBox::information(this, "XRechnung Viewer", "Die konfigurierten Saxon Dateien können nicht gefunden werden.\n\n"
                                 "Bitte legen Sie eine Konfig-Datei in ~/.config/xrview/xrview.conf an und hinterlegen Sie die Pfade"
                                 "wie in der Dokumentation angegeben.");
    } else {
        config.setValue("main/lastUsage", QDateTime::currentDateTime());
        config.sync();
    }

}

void MainWindow::slotShowXRechnung(XRechnung *xr)
{
    if (xr == nullptr) return;

    ui->_mainToolBox->blockSignals(true);
    int indx = ui->_mainToolBox->addItem(new XRWidget(xr, this), xr->fileName());
    _xrs.append(xr);

    int currSize = _xrs.size();
    if (indx > currSize-1) {
        _xrs.resize(currSize+10);
    }

    _xrs[indx] = xr;
    _maxIndx=indx;

    connect( xr, &XRechnung::htmlAvailable, this, [=]() {
        this->slotShowDocumentNo(indx);
    });
    ui->_mainToolBox->blockSignals(false);
}

void MainWindow::slotShowDocumentNo(int newIndx)
{
    if (newIndx >= 0 && newIndx <= _maxIndx) {
        XRechnung *xr = _xrs[newIndx];
        ui->_htmlView->setHtml(xr->html());
    } else {
        qDebug() << "Unable to show indx" << newIndx;
    }
}
