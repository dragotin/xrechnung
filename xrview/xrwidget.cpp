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

#include "xrwidget.h"
#include "xrechnung.h"

#include <QFormLayout>
#include <QLabel>

XRWidget::XRWidget(QWidget *parent)
    : QWidget{parent}
{

}

XRWidget::XRWidget(XRechnung *xr, QWidget *parent)
    : QWidget{parent},
      _xr{xr}

{
    _layout = new QFormLayout(this);

    connect(xr, &XRechnung::domDocAvailable, this, &XRWidget::slotDomAvailable);
}

void XRWidget::slotDomAvailable()
{
    addDisplay("xr:Buyer_reference", "Leitweg-ID:");
    addDisplay("xr:Invoice_issue_date", "Datum:");
    addDisplay("xr:Seller_name", "Verkäufer:");
    addDisplay("xr:Sum_of_Invoice_line_net_amount", "Netto:");
    addDisplay("xr:Invoice_total_VAT_amount", "MwSt:");
    addDisplay("xr:Invoice_total_amount_with_VAT", "Brutto:");
}

void XRWidget::addDisplay(const QString& xmlName, const QString& displayName)
{
    QWidget *w = new QWidget(this);
    new QLabel(_xr->value(xmlName), w);

    _layout->addRow(displayName, w);

}
