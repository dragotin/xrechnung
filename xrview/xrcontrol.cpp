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

#include "xrcontrol.h"
#include "xrechnung.h"

#include <QFileInfo>

XRControl::XRControl(QObject *parent)
    : QObject{parent}
{

}

void XRControl::registerFiles( const QList<QUrl> list)
{
    if (! list.size()) return;

    for (const QUrl& url : list ) {
        QFileInfo fi(url.toLocalFile());

        if (fi.exists() && fi.isReadable() && fi.size()) {
            XRechnung *xr = new XRechnung(url, this);
            _xrList.append(xr);
            emit showXRechnung(xr);
        }
    }    
}

