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

#ifndef XRECHNUNG_H
#define XRECHNUNG_H

#include <QObject>
#include <QDomDocument>
#include <QUrl>
#include <QProcess>

class XRechnung : public QObject
{
    Q_OBJECT
public:
    explicit XRechnung(QObject *parent = nullptr);

    XRechnung(const QUrl& file, QObject *parent = nullptr);
    QDomDocument domDocument() const;

    QString html() const;

    QString value(const QString& name) const;
    QString fileName() { return _url.toLocalFile(); }

signals:
    void htmlAvailable();
    void domDocAvailable();

private slots:
    bool readDomDocument();
    void createUbl();
    void slotErrorOccurred(QProcess::ProcessError error);
    void slotDomFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void createHtml();
    void slotHtmlFinished(int exitCode, QProcess::ExitStatus exitStatus);

private:
    QUrl         _url;
    QString      _html;
    QDomDocument _domDoc;

    const QString SaxonJar{"/home/kf/projects/xrech2html/saxon-he-11.4.jar"};
    const QString XslUBL{"/home/kf/projects/xrechnung-visualization/src/xsl/ubl-invoice-xr.xsl"};
    const QString XslHtml{"/home/kf/projects/xrechnung-visualization/src/xsl/xrechnung-html.xsl"};
};

#endif // XRECHNUNG_H
