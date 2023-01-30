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

#include "xrechnung.h"
#include "qdebug.h"

#include <QFile>
#include <QProcess>

namespace {

QString ublFileName(const QUrl& url)
{
    return QString("%1.ubl.xml").arg(url.toLocalFile());
}

}


XRechnung::XRechnung(QObject *parent)
    : QObject{parent}
{

}

XRechnung::XRechnung(const QUrl& file, const QString& SaxonJar, const QString& XslUBL, const QString& XslHtml, QObject *parent)
    : QObject{parent},
      _url(file)
{
	this->SaxonJar = SaxonJar;
	this->XslUBL = XslUBL;
	this->XslHtml = XslHtml;
	createUbl();
}

QString XRechnung::html() const
{
    return _html;
}

QString XRechnung::value(const QString& name) const
{
    QDomNodeList elems = _domDoc.elementsByTagName(name);

    if (elems.isEmpty()) {
        qDebug() << "Can not find dom element" << name;
        return QString();
    }

    if (elems.size() > 1)
        qDebug() << "Dom list larger than 1";

    QDomElement elem = elems.at(0).toElement();
    return elem.text();
}

QDomDocument XRechnung::domDocument() const
{
    return _domDoc;
}

void XRechnung::createUbl()
{
    QStringList args;
    QString file{ _url.toLocalFile() };

    args.append("-jar");
    args.append(SaxonJar);
    args.append(QString("-s:%1").arg(file));
    args.append(QString("-xsl:%1").arg(XslUBL));
    args.append(QString("-o:%1").arg(ublFileName(_url)));

    QProcess *process = new QProcess;

    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
                this, &XRechnung::slotDomFinished);
    connect(process, &QProcess::errorOccurred, this, &XRechnung::slotErrorOccurred);

    qDebug() << "Starting" << "java" << args;
    process->start("java", args);
}

void XRechnung::slotErrorOccurred(QProcess::ProcessError error)
{
    int exitCode = 1; // general error
    if (error == QProcess::FailedToStart) {
        qDebug() << "failed to start saxon";
    }
}

void XRechnung::slotDomFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    Q_UNUSED(exitStatus)
    Q_UNUSED(exitCode)

    QProcess *p = qobject_cast<QProcess*>(sender());

    qDebug() << "stderr output: " << p->readAllStandardError();
    p->deleteLater();
    readDomDocument();

    // start the conversion to html
    createHtml();
}

void XRechnung::createHtml()
{
    QStringList args;
    QString file{ _url.toLocalFile() };

    args.append("-jar");
    args.append(SaxonJar);
    args.append(QString("-s:%1").arg(ublFileName(_url)));
    args.append(QString("-xsl:%1").arg(XslHtml));

    QProcess *process = new QProcess;

    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
                this, &XRechnung::slotHtmlFinished);

    connect(process, &QProcess::errorOccurred, this, &XRechnung::slotErrorOccurred);

    qDebug() << "Starting" << "java" << args;
    process->start("java", args);
}

void XRechnung::slotHtmlFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    Q_UNUSED(exitStatus)
    Q_UNUSED(exitCode)

    QProcess *p = qobject_cast<QProcess*>(sender());

    qDebug() << "stderr output: " << p->readAllStandardError();
    _html = p->readAllStandardOutput();
    p->deleteLater();
    emit htmlAvailable();
}


bool XRechnung::readDomDocument()
{
    bool re{false};
    QDomDocument doc("InterDoc");
    QFile file(ublFileName(_url));

    if (file.open(QIODevice::ReadOnly)) {

        if (doc.setContent(&file)) {
            qDebug() << "Successfully loaded xml" << file.fileName();
            _domDoc = doc;
            re = true;
        }
    }
    file.close();

    emit domDocAvailable();

    return re;
}
