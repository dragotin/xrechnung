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

#ifndef XRCONTROL_H
#define XRCONTROL_H

#include "xrechnung.h"

#include <QObject>

class XRControl : public QObject
{
    Q_OBJECT
public:
    explicit XRControl(QObject *parent = nullptr);

    void registerFiles( const QList<QUrl> list);

signals:
    void showXRechnung(XRechnung*);

private:
    QVector<XRechnung*> _xrList;
};

#endif // XRCONTROL_H
