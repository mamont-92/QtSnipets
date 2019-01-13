/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt Charts module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QLineSeries>

#include "TBezierInterpolation.h"

#include "chart.h"
#include "chartview.h"

QT_CHARTS_USE_NAMESPACE

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);



    /*series->append(0, 6);
    series->append(1, 3);
    series->append(2, 4);
    series->append(3, 8);
    series->append(7, 13);
    series->append(10, 5);*/
    /*series << QPointF(11, 1) << QPointF(13, 3) << QPointF(17, 6) << QPointF(18, 3) << QPointF(20, 2) << QPointF(22, 1);*/

    Chart *chart = new Chart(0, 0, NULL);

    //chart->addSeries(series);


    chart->setTitle("Drag'n drop to move data points");

    /*QValueAxis *axisX = new QValueAxis();
    chart->setAxisX(axisX, series);
    axisX->setRange(0, 10);

    QValueAxis *axisY = new QValueAxis();
    chart->setAxisY(axisY, series);
    axisY->setRange(0, 13);*/

    QList<QPointF> list, outValues;
    list.append(QPointF(0.0, 0.0));
    list.append(QPointF(0.1, 0.2));
    list.append(QPointF(0.2, 0.1));
    list.append(QPointF(0.5, 0.0));
    list.append(QPointF(0.8, 0.7));
    list.append(QPointF(1.0, 1.0));
    SplineRasterizator::rasterSpline(list, outValues, 655535);


    ChartView *chartView = new ChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    QMainWindow window;
    window.setCentralWidget(chartView);
    window.resize(400, 300);
    window.show();

    return a.exec();
}
