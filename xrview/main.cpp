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
#include "xrcontrol.h"

#include <QApplication>
#include <QCommandLineParser>
#include <QTranslator>
#include <QUrl>
#include <QObject>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow w;
    XRControl xrc;

    QCommandLineParser parser;
    parser.addVersionOption();
    parser.addHelpOption();
    // parser.addOption(QCommandLineOption(QStringList() << QLatin1String("demopoints_single"), tr("Add built-in demo points as single markers")));
    // parser.addOption(QCommandLineOption(QStringList() << QLatin1String("demopoints_group"),  tr("Add built-in demo points as groupable markers")));
    // parser.addOption(QCommandLineOption(QStringList() << QLatin1String("single"),            tr("Do not group the displayed images")));
    parser.addPositionalArgument(QString::fromLatin1("xrechnung"), QLatin1String("List of xrechnung"), QString::fromLatin1("[xrechnung...]"));
    parser.process(app);

    // get the list of images to load on startup:
    QList<QUrl> filesList;

    for (const QString& file : parser.positionalArguments()) {
        const QUrl argUrl = QUrl::fromLocalFile(file);
        qDebug() << argUrl;
        filesList << argUrl;
    }

    // The controller gets a list of urls to laod the XRechnung docs from.
    // This connection cares for displaying the XRechnung.
    QObject::connect(&xrc, &XRControl::showXRechnung, &w, &MainWindow::slotShowXRechnung);

    xrc.registerFiles(filesList);

    w.show();
    return app.exec();
}
