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

#include "chart.h"
#include <QtCharts/QValueAxis>
#include <QtCharts/QAbstractAxis>
#include <QtCore/QtMath>
#include <TBezierInterpolation.h>

Chart::Chart(QGraphicsItem *parent, Qt::WindowFlags wFlags, QScatterSeries *series)
    : QChart(QChart::ChartTypeCartesian, parent, wFlags), m_series(series)
{
    m_clicked = false;
    m_lineSeries = new QLineSeries();
    m_series = new QScatterSeries();
    this->addSeries(m_lineSeries);
    this->addSeries(m_series);
    QObject::connect(m_series, SIGNAL(pressed(QPointF)), this, SLOT(clickPoint(QPointF)));
    legend()->hide();
    createDefaultAxes();


    m_axisX = new QValueAxis();
    setAxisX(m_axisX, m_series);
    setAxisX(m_axisX, m_lineSeries);

    //axisX->setRange(0, 10);
    //m_axisX->applyNiceNumbers();

    m_axisY = new QValueAxis();
    this->setAxisY(m_axisY, m_series);
    this->setAxisY(m_axisY, m_lineSeries);
    //axisY->setRange(0, 13);
    //m_axisY->applyNiceNumbers();
}

Chart::~Chart()
{

}

void Chart::clickPoint(const QPointF &point)
{
    // Find the closes data point
    m_movingPoint = QPoint();
    m_clicked = false;
    foreach (QPointF p, m_series->points()) {
        if (distance(p, point) < distance(m_movingPoint, point)) {
            m_movingPoint = p;
            m_clicked = true;
        }
    }
}

qreal Chart::distance(const QPointF &p1, const QPointF &p2)
{
    return qSqrt((p1.x() - p2.x()) * (p1.x() - p2.x())
                + (p1.y() - p2.y()) * (p1.y() - p2.y()));
}

void Chart::setPointClicked(bool clicked)
{
    m_clicked = clicked;
}

void Chart::addSeries(QAbstractSeries *series){
    QChart::addSeries(series);
    updateLineSeries();
}

void Chart::handlePointMove(const QPoint &point)
{
    if (m_clicked) {
        //Map the point clicked from the ChartView
        //to the area occupied by the chart.
        QPoint mappedPoint = point;
        mappedPoint.setX(point.x() - this->plotArea().x());
        mappedPoint.setY(point.y() - this->plotArea().y());

        //Get the x- and y axis to be able to convert the mapped
        //coordinate point to the charts scale.
        QAbstractAxis *axisx = this->axisX();
        QValueAxis *haxis = 0;
        if (axisx->type() == QAbstractAxis::AxisTypeValue)
            haxis = qobject_cast<QValueAxis *>(axisx);

        QAbstractAxis *axisy = this->axisY();
        QValueAxis *vaxis = 0;
        if (axisy->type() == QAbstractAxis::AxisTypeValue)
            vaxis = qobject_cast<QValueAxis *>(axisy);

        if (haxis && vaxis) {
            //Calculate the "unit" between points on the x
            //y axis.
            double xUnit = this->plotArea().width() / haxis->max();
            double yUnit = this->plotArea().height() / vaxis->max();

            //Convert the mappedPoint to the actual chart scale.
            double x = mappedPoint.x() / xUnit;
            double y = vaxis->max() - mappedPoint.y() / yUnit;

            //Replace the old point with the new one.
            x = qBound(0.0, x, 1.0);
            m_series->replace(m_movingPoint, QPointF(x, y));

            //Update the m_movingPoint so we are able to
            //do the replace also during mousemoveEvent.
            m_movingPoint.setX(x);
            m_movingPoint.setY(y);
            updateLineSeries();
        }
    }
}

void Chart::addPoint(const QPoint &point){
    if (m_clicked) {
        m_series->remove(m_movingPoint);
        updateLineSeries();
    }
    else{
    QPoint mappedPoint = point;
    mappedPoint.setX(point.x() - this->plotArea().x());
    mappedPoint.setY(point.y() - this->plotArea().y());

    //Get the x- and y axis to be able to convert the mapped
    //coordinate point to the charts scale.
    QAbstractAxis *axisx = this->axisX();
    QValueAxis *haxis = 0;
    if (axisx->type() == QAbstractAxis::AxisTypeValue)
        haxis = qobject_cast<QValueAxis *>(axisx);

    QAbstractAxis *axisy = this->axisY();
    QValueAxis *vaxis = 0;
    if (axisy->type() == QAbstractAxis::AxisTypeValue)
        vaxis = qobject_cast<QValueAxis *>(axisy);

    if (haxis && vaxis) {
        //Calculate the "unit" between points on the x
        //y axis.
        double xUnit = this->plotArea().width() / haxis->max();
        double yUnit = this->plotArea().height() / vaxis->max();

        //Convert the mappedPoint to the actual chart scale.
        double x = mappedPoint.x() / xUnit;
        double y = vaxis->max() - mappedPoint.y() / yUnit;

        //Replace the old point with the new one.
        //m_series->replace(m_movingPoint, QPointF(x, y));
        x = qBound(0.0, x, 1.0);
        m_series->append(QPointF(x, y));

        //Update the m_movingPoint so we are able to
        //do the replace also during mousemoveEvent.

        updateLineSeries();
    }
    }
}


void Chart::updateLineSeries(){  
    m_lineSeries->clear();
    QList<QPointF> list = m_series->points();

    if(list.length() > 1){
        qSort(list.begin(), list.end(), [](const QPointF & a, const QPointF & b) -> bool {
            return a.x() < b.x(); }
        );

        QList<QPointF> outValues;
        SplineRasterizator::rasterSpline(list, outValues, 100);
        m_lineSeries->append(outValues);
    }
}
