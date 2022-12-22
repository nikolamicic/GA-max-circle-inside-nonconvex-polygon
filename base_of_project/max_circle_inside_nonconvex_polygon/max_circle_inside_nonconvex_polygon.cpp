#include "max_circle_inside_nonconvex_polygon.h"
#include "pomocnefunkcije.h"

#include <algorithm>
#include <QPainterPath>
#include <iostream>
#include <fstream>
#include <cmath>
#include <limits>

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
            bool is_point_in_polygon = PointInPolygon(p, _tacke);
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
    /*)
    auto pen = painter->pen();
    pen.setColor(Qt::red);

    pen->drawLine(QLineF(_leftBottomRectPoint,_leftTopRectPoint));
    pen->drawLine(QLineF(_leftTopRectPoint,_rightTopRectPoint));
    pen->drawLine(QLineF(_rightTopRectPoint,_rightBottomRectPoint));
    pen->drawLine(QLineF(_rightBottomRectPoint,_leftBottomRectPoint));
    */

    /*)
    for (auto i = 1ul; i < _tacke.size(); i++) {
        if (_tacke[i].x() > _maxTacka.x() || (_tacke[i].x() == _maxTacka.x() && _tacke[i].y() < _maxTacka.y()))
            _maxTacka = _tacke[i];
    }
    AlgoritamBaza_updateCanvasAndBlock()

    std::sort(_tacke.begin(), _tacke.end(), [&](const auto& lhs, const auto& rhs) {
        int P = pomocneFunkcije::povrsinaTrougla(_maxTacka, lhs, rhs);
        return  (P < 0) ||  (P == 0 && pomocneFunkcije::distanceKvadrat(_maxTacka, lhs)
                             < pomocneFunkcije::distanceKvadrat(_maxTacka, rhs));
    });

    _maxKrugProstogPoligona.push_back(_maxTacka);
    _maxKrugProstogPoligona.push_back(_tacke[1]);
    unsigned pom = 2;
    unsigned j = 2;

    while(j < _tacke.size()) {
        if(pomocneFunkcije::povrsinaTrougla(_maxKrugProstogPoligona[pom-2],
                                            _maxKrugProstogPoligona[pom-1],
                                            _tacke[j]) < 0)
        {
            _maxKrugProstogPoligona.push_back(_tacke[j]);
            ++pom;
            ++j;
        }
        else {
            _maxKrugProstogPoligona.pop_back();
            --pom;
            // Ne smemo da povecamo j u ovom slucaju, jer nismo zavrsili sa ovom tackom
        }
        AlgoritamBaza_updateCanvasAndBlock()
    }
    */
    //_maxKrugProstogPoligona.push_back(_maxTacka);
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
