#ifndef BEZIER_H
#define BEZIER_H

#include <vector>
#include <iostream>
#include <cmath>

#include <QPointF>
#include <QVector>
#include <QList>
#include <QDebug>

using namespace std;

#define EPSILON 1.0e-5
#define RESOLUTION 32

inline void normalize(QPointF & p){
    float x = p.x();
    float y = p.y();
    float l = sqrt(x*x+y*y);
    if(l > EPSILON){
        p.setX(x / l);
        p.setY(y / l);
    }
}

class Segment{
public:
    QPointF points[4];
    
    void calc(double t, QPointF &p){
        double t2 = t * t;
        double t3 = t2 * t;
        double nt = 1.0 - t;
        double nt2 = nt * nt;
        double nt3 = nt2 * nt;
        float x = nt3 * points[0].x() + 3.0 * t * nt2 * points[1].x() + 3.0 * t2 * nt * points[2].x() + t3 * points[3].x();
        float y = nt3 * points[0].y() + 3.0 * t * nt2 * points[1].y() + 3.0 * t2 * nt * points[2].y() + t3 * points[3].y();
        p.setX(x);
        p.setY(y);
    }
};


class SplineRasterizator{
public:
    static void rasterSpline(QList<QPointF> pointList,  QList<QPointF> & outValues){
        int resolution = 10;
        vector<QPointF> testValues;
        vector<Segment> spline;
        QPointF p;

        outValues.clear();

        QListIterator<QPointF> iter(pointList);
        while(iter.hasNext()){
            QPointF pF = iter.next();
            testValues.push_back(pF);
        }

        calculateSpline(testValues, spline);
        for (auto s : spline){
            for (int i = 0; i < resolution; ++i){
                s.calc((double)i / (double)resolution, p);
                outValues.append(p);
            }
        }

    }

    static void rasterSpline(QList<QPointF> pointList,  QList<QPointF> & outValues, int count){
        int resolution = 10;
        vector<QPointF> testValues;
        vector<Segment> spline;
        QPointF p;

        outValues.clear();

        QListIterator<QPointF> iter(pointList);
        while(iter.hasNext()){
            QPointF pF = iter.next();
            testValues.push_back(pF);
        }

        calculateSpline(testValues, spline);
        int splineCount = spline.size();

        count--;
        if(splineCount){
            for(int i = 0; i <= count; ++i){
                float f = (float)i / (float)count * (float)splineCount;
                int splineNumber = qMin((int)f, splineCount-1);
                float interpolatedValue = f - (float)splineNumber;
                auto s = spline[splineNumber];
                s.calc(interpolatedValue, p);
                outValues.append(p);
            }
        }

    }

private:
    static bool calculateSpline(const vector<QPointF> &values, vector<Segment> &bezier){
        int n = values.size() - 1;

        if (n < 2)
            return false;

        bezier.resize(n);

        QPointF tgL;
        QPointF tgR;
        QPointF cur;
        QPointF next = values[1] - values[0];
        normalize(next);

        --n;

        double l1, l2, tmp, x;
        for (int i = 0; i < n; ++i){
            bezier[i].points[0] = bezier[i].points[1] = values[i];
            bezier[i].points[2] = bezier[i].points[3] = values[i + 1];

            cur = next;
            next = values[i + 2] - values[i + 1];
            normalize(next);

            tgL = tgR;

            tgR = cur + next;
            normalize(tgR);

            if (abs(values[i + 1].y() - values[i].y()) < EPSILON){
                l1 = l2 = 0.0;
            }
            else{
                tmp = values[i + 1].x() - values[i].x();
                l1 = abs(tgL.x()) > EPSILON ? tmp / (2.0 * tgL.x()) : 1.0;
                l2 = abs(tgR.x()) > EPSILON ? tmp / (2.0 * tgR.x()) : 1.0;
            }

            if (abs(tgL.x()) > EPSILON && abs(tgR.x()) > EPSILON){
                tmp = tgL.y() / tgL.x() - tgR.y() / tgR.x();
                if (abs(tmp) > EPSILON){
                    x = (values[i + 1].y() - tgR.y() / tgR.x() * values[i + 1].x() - values[i].y() + tgL.y() / tgL.x() * values[i].x()) / tmp;
                    if (x > values[i].x() && x < values[i + 1].x()){
                        if (tgL.y() > 0.0){
                            if (l1 > l2)
                                l1 = 0.0;
                            else
                                l2 = 0.0;
                        }
                        else{
                            if (l1 < l2)
                                l1 = 0.0;
                            else
                                l2 = 0.0;
                        }
                    }
                }
            }

            bezier[i].points[1] += tgL * l1;
            bezier[i].points[2] -= tgR * l2;
        }

        l1 = abs(tgL.x()) > EPSILON ? (values[n + 1].x() - values[n].x()) / (2.0 * tgL.x()) : 1.0;

        bezier[n].points[0] = bezier[n].points[1] = values[n];
        bezier[n].points[2] = bezier[n].points[3] = values[n + 1];
        bezier[n].points[1] += tgR * l1;

        return true;
    }

};


#endif
