#include "max_circle_inside_nonconvex_polygon.h"
#include "pomocnefunkcije.h"

#include <algorithm>
#include <QPainterPath>
#include <iostream>
#include <fstream>
#include <cmath>
#include <limits>
#include <stdlib.h>

MaxKrugProstogPoligona::MaxKrugProstogPoligona(QWidget *pCrtanje,
                                 int pauzaKoraka,
                                 const bool &naivni,
                                 std::string imeDatoteke,
                                 int brojTacaka)
    : AlgoritamBaza(pCrtanje, pauzaKoraka, naivni)
{
    if (imeDatoteke == "")
        qDebug("Potrebno je upisati ime datoteke iz koje se čitaju podaci.");
    else{
        _tacke = ucitajPodatkeIzDatoteke(imeDatoteke);
        //_allPoints = ucitajPodatkeIzDatoteke(imeDatoteke);
    }
    _k = _tacke.size();
}

bool MaxKrugProstogPoligona::onLine(QLineF l1, QPointF p)
{
    // Check whether p is on the line or not
    if (p.rx() <= fmax(l1.p1().rx(), l1.p2().rx())
        && p.rx() <= fmin(l1.p1().rx(), l1.p2().rx())
        && (p.ry() <= fmax(l1.p1().ry(), l1.p2().ry())
            && p.ry() <= fmin(l1.p1().ry(), l1.p2().ry())))
        return true;

    return false;
}

int MaxKrugProstogPoligona::direction(QPointF a, QPointF b, QPointF c)
{
    int val = (b.ry() - a.ry()) * (c.rx() - b.rx())
              - (b.rx() - a.rx()) * (c.ry() - b.ry());

    if (val == 0)
        // Colinear
        return 0;

    else if (val < 0)
        // Anti-clockwise direction
        return 2;

    // Clockwise direction
    return 1;
}

bool MaxKrugProstogPoligona::isIntersect(QLineF l1, QLineF l2)
{
    // Four direction for two lines and points of other line
    int dir1 = direction(l1.p1(), l1.p2(), l2.p1());
    int dir2 = direction(l1.p1(), l1.p2(), l2.p2());
    int dir3 = direction(l2.p1(), l2.p2(), l1.p1());
    int dir4 = direction(l2.p1(), l2.p2(), l1.p2());

    // When intersecting
    if (dir1 != dir2 && dir3 != dir4)
        return true;

    // When p2 of line2 are on the line1
    if (dir1 == 0 && onLine(l1, l2.p1()))
        return true;

    // When p1 of line2 are on the line1
    if (dir2 == 0 && onLine(l1, l2.p2()))
        return true;

    // When p2 of line1 are on the line2
    if (dir3 == 0 && onLine(l2, l1.p1()))
        return true;

    // When p1 of line1 are on the line2
    if (dir4 == 0 && onLine(l2, l1.p2()))
        return true;

    return false;
}

// New algorithm for checking if point is inside of Polygon.
// Time Complexity: O(n) where n is the number of vertices in the given polygon.
// Auxiliary Space: O(1), since no extra space has been taken.
bool MaxKrugProstogPoligona::checkInside(std::vector<QPointF> poly, int nvert, QPointF p)
{

    // When polygon has less than 3 edge, it is not polygon
    if (nvert < 3)
        return false;

    // Create a point at infinity, y is same as point p
    QPointF inf_p(9999,p.ry());
    QLineF exline(p, inf_p);

    int count = 0;
    int i = 0;
    do {

        // Forming a line from two consecutive points of
        // poly
        QLineF side(poly[i], poly[(i + 1) % nvert]);
        if (isIntersect(side, exline)) {

            // If side is intersects exline
            if (direction(side.p1(), p, side.p2()) == 0)
                return onLine(side, p);
            count++;
        }
        i = (i + 1) % nvert;
    } while (i != 0);

    // When count is odd
    return count & 1;
}


// Old version of checking if point is inside of Polygon.
bool MaxKrugProstogPoligona::PointInPolygon(QPointF point, std::vector<QPointF> polygon) {

      int i, j;
      int nvert = polygon.size();
      bool c = false;

      for(i = 0, j = nvert - 1; i < nvert; j = i++) {
        if( ( (polygon[i].ry() > point.ry() ) != (polygon[j].ry() > point.ry()) ) &&
            (point.rx() < (polygon[j].rx() - polygon[i].rx()) * (point.ry() - polygon[i].ry()) / (polygon[j].ry() - polygon[i].ry()) + polygon[i].rx())
          )
          c = !c;
      }

      return c;
}

float MaxKrugProstogPoligona::MinDistanceOfPoint(QPointF point, std::vector<QPointF> polygon) {

    int i, j;
    int nvert = polygon.size();
    float r = 0.0;
    float min_dist = std::numeric_limits<float>::max();
    float curr_dist = 0.0;

    for(i = 0, j = nvert - 1; i < nvert; j = i++) {
        QPointF point1 = polygon[i];
        QPointF point2 = polygon[j];

        // vector p1 to p2 = p2-p1
        QPointF vector_p1_p2(point2.rx()-point1.rx(), point2.ry()-point1.ry());

        // vector p1 to p  = p -p1
        QPointF vector_p1_p(point.rx()-point1.rx(), point.ry()-point1.ry());

        // vector x to p2 = vector(p2-x)
        QPointF vector_p_p2(point2.rx()-point.rx(), point2.ry()-point.ry());

        // dot_product between vector_p1_p2 and vector_p1_p
        float dot_product = vector_p1_p2.rx()*vector_p1_p.rx() + vector_p1_p2.ry()*vector_p1_p.ry();

        // ||P2-P1|| magnitude of vector p1 to p2: c² = a² + b²
        // magnitude(vector(p2 - p1))
        float magnitude_p1_p2 = sqrt((vector_p1_p2.rx()*vector_p1_p2.rx()) + (vector_p1_p2.ry()*vector_p1_p2.ry()));
        r = dot_product / (magnitude_p1_p2*magnitude_p1_p2);

        // magnitude(vector(x - p1))
        float magnitude_p1_p = sqrt((vector_p1_p.rx()*vector_p1_p.rx()) + (vector_p1_p.ry()*vector_p1_p.ry()));

        // magnitude(vector(p2 - x))
        float mangitude_p_p2 = sqrt((vector_p_p2.rx()*vector_p_p2.rx()) + (vector_p_p2.ry()*vector_p_p2.ry()));


        if (r < 0) {
            // curr_dist = magnitude(vector(x - p1))
            curr_dist = magnitude_p1_p;
        }
        else if (r > 1) {
            // curr_dist = magnitude(vector(p2 - x))
            curr_dist = mangitude_p_p2;
        }
        else{
            // sqrt(||P0-P1||^2 - (r * ||P2-P1||)^2)
            curr_dist = sqrt((magnitude_p1_p)*(magnitude_p1_p) - (r*magnitude_p1_p2)*(r*magnitude_p1_p2));
        }

        if(curr_dist < min_dist){
            min_dist = curr_dist;
        }

    }

    return min_dist;

}

void MaxKrugProstogPoligona::ReducingRectangleArea(){
    // reducing Rectangle Area by a factor of the square root of 2
    float x_diff = sqrt(_xmax-_xmin);
    float y_diff = sqrt(_ymax-_ymin);


    // define a new Rectangle with smaller intervals and bounds around _final_max_circle_point (that is current center of max circle)

    _xmin = _final_max_circle_point.rx() - x_diff/2;
    _xmax = _final_max_circle_point.rx() + x_diff/2;

    _ymin = _final_max_circle_point.ry() - y_diff/2;
    _ymax = _final_max_circle_point.ry() + y_diff/2;

}

void MaxKrugProstogPoligona::CurrentPointWithCenterOfMaxCircle() {

    _rectXs.clear();
    _rectYs.clear();

    _polygonPoints.clear();
    _allPoints.clear();

    _leftBottomRectPoint = QPointF(_xmin,_ymin);
    _leftTopRectPoint = QPointF(_xmin,_ymax);
    _rightTopRectPoint = QPointF(_xmax,_ymax);
    _rightBottomRectPoint = QPointF(_xmax,_ymin);

    float divider = 20;

    float x_div_res = (_xmax-_xmin)/divider;
    float y_div_res = (_ymax-_ymin)/divider;

    float curr_x = _xmin;
    _rectXs.push_back(curr_x);
    for(auto i = 1; i < divider; i++) {
        curr_x = curr_x + x_div_res;
        _rectXs.push_back(curr_x);
    }
    _rectXs.push_back(_xmax);

    float curr_y = _ymin;
    _rectYs.push_back(curr_y);
    for(auto i = 1; i < divider; i++) {
        curr_y = curr_y + y_div_res;
        _rectYs.push_back(curr_y);
    }
    _rectYs.push_back(_ymax);

    for(auto tacka: _rectYs){
        qDebug("Element rectYs-a: %f", tacka);
    }

    for(auto i = 0ul; i < _rectXs.size(); i++) {
        for(auto j = 0ul; j < _rectYs.size(); j++){
            QPointF p(_rectXs[i],_rectYs[j]);
            //bool is_point_in_polygon = PointInPolygon(p, _tacke);
            bool is_point_in_polygon = checkInside(_tacke, _tacke.size(), p);
            if(is_point_in_polygon==true){
                _polygonPoints.push_back(p);
            }
            _allPoints.push_back(p);
        }
    }

    _final_max_dist = 0.0;
    _final_max_circle_point = _polygonPoints[0];

    for(auto i = 1ul; i < _polygonPoints.size(); i++){
        float curr_min_dist = MinDistanceOfPoint(_polygonPoints[i],_tacke);
        if (curr_min_dist > _final_max_dist){
            _final_max_dist = curr_min_dist;
            _final_max_circle_point = _polygonPoints[i];
            qDebug("trenutni final_max_circle_point: x %f, y %f. Distanca je: %f",_final_max_circle_point.rx(),_final_max_circle_point.ry(),_final_max_dist);
        }
    }

    qDebug("ZAVRSENA FOR PETLJA!");
}

void MaxKrugProstogPoligona::pokreniAlgoritam() {
    // Trazenje maksimalnih i minimalnih x,y osa

    for(auto &tacka: _tacke) {
        if (tacka.rx() < _xmin){
            _xmin = tacka.rx();
        }
        if (tacka.rx() > _xmax){
            _xmax = tacka.rx();
        }
        if (tacka.ry()< _ymin){
            _ymin = tacka.ry();
        }
        if (tacka.ry() > _ymax){
            _ymax = tacka.ry();
        }
    }
    qDebug("xmin: %f \txmax: %f \tymin: %f \tymax: %f \t",_xmin,_xmax,_ymin,_ymax);

    float previous_max_distance;
    float precision = 0.01;
    int i = 0;

    do{
        previous_max_distance = _final_max_dist;

        CurrentPointWithCenterOfMaxCircle();
        ReducingRectangleArea();
        qDebug("xmin: %f \txmax: %f \tymin: %f \tymax: %f \t",_xmin,_xmax,_ymin,_ymax);

        i++;
        qDebug("%d. iteracija, max_distance: %f", i, _final_max_dist);

        AlgoritamBaza_updateCanvasAndBlock();
    }while((_final_max_dist-previous_max_distance) > precision);

    qDebug("zavrsen do while!");

    AlgoritamBaza_updateCanvasAndBlock()
    emit animacijaZavrsila();
}

void MaxKrugProstogPoligona::crtajAlgoritam(QPainter *painter) const {
    if (!painter) return;

    auto pen = painter->pen();
    pen.setColor(Qt::red);
    painter->setPen(pen);
    for(auto &tacka: _tacke) {
        painter->drawPoint(tacka);
    }
    pen.setColor(Qt::red);
    painter->setPen(pen);
    for(auto i = 1ul; i < _tacke.size(); i++) {
        qDebug("crtanje linije za tacku: %lul",i);
        painter->drawLine(QLineF(_tacke[i-1],_tacke[i]));
    }
    painter->drawLine(QLineF(_tacke[_tacke.size()-1],_tacke[0]));

    pen.setColor(Qt::blue);
    painter->setPen(pen);
    painter->drawLine(QLineF(_leftBottomRectPoint,_leftTopRectPoint));
    painter->drawLine(QLineF(_leftTopRectPoint,_rightTopRectPoint));
    painter->drawLine(QLineF(_rightTopRectPoint,_rightBottomRectPoint));
    painter->drawLine(QLineF(_rightBottomRectPoint,_leftBottomRectPoint));


    /*
    for(auto i = 0ul; i < _allPoints.size(); i++) {
        for(auto j = 0ul; j < _allPoints[i].size(); j++){
            painter->drawPoint(_allPoints[i][j]);
        }
    }
    */
    pen.setColor(Qt::red);
    painter->setPen(pen);

    for(auto &tacka: _allPoints){
        painter->drawPoint(tacka);
    }

    pen.setColor(Qt::green);
    painter->setPen(pen);

    for(auto &tacka: _polygonPoints){
        painter->drawPoint(tacka);
    }



   // QPointF max_circle = getMaxCirclePointCenter();

    pen.setColor(Qt::cyan);
    painter->setPen(pen);
    painter->drawPoint(_final_max_circle_point);


    painter->drawEllipse(_final_max_circle_point, _final_max_dist, _final_max_dist);
    //qDebug("crtanje KRAJNJE TACKE painter: %f %f", max_circle.rx(), max_circle.ry());
}

void MaxKrugProstogPoligona::pokreniNaivniAlgoritam() {
    //Slozenost naivnog algoritma: O(n^3).
    // Prolazi se kroz svaki par tacaka.
    return;
}

void MaxKrugProstogPoligona::crtajNaivniAlgoritam(QPainter *painter) const
{
    // Odustajanje u slucaju greske
    if (!painter) return;

    // Dohvatanje olovke
    auto pen = painter->pen();
    return;
}

QPointF MaxKrugProstogPoligona::getMaxCirclePointCenter() const
{
    return _final_max_circle_point;
}

/* Citanje pravougaonika iz datoteke */
std::vector<QPointF> MaxKrugProstogPoligona::ucitajPodatkeIzDatoteke(std::string imeDatoteke)
{
    /* Otvaranje zadatog fajla */
    std::ifstream inputFile(imeDatoteke);
    std::vector<QPointF> points;

    float x, y;
    while(inputFile >> x >> y)
    {
        QPointF p(x,y);
        points.push_back(p);
    }

    return points;
}
