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

#ifndef XRWIDGET_H
#define XRWIDGET_H

#include <QFormLayout>
#include <QWidget>

class XRechnung;

/**
 * @brief The XRWidget class shows information about an XRechnung Object.
 */
class XRWidget : public QWidget
{
    Q_OBJECT
public:
    explicit XRWidget(QWidget *parent = nullptr);
    XRWidget(XRechnung *xr, QWidget *parent);

    void addDisplay(const QString& xmlName, const QString& displayName);

public slots:
    void slotDomAvailable();

private:
    XRechnung *_xr;
    QFormLayout *_layout;
};

#endif // XRWIDGET_H
