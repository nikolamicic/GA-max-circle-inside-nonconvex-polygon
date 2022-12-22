#include "max_circle_inside_nonconvex_polygon.h"
#include "pomocnefunkcije.h"

#include <algorithm>
#include <QPainterPath>
#include <iostream>
#include <fstream>

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
    qDebug("xmin: %d \txmax: %d \tymin: %d \tymax: %d \t",_xmin,_xmax,_ymin,_ymax);

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
        qDebug("Element rectYs-a: %d", tacka);
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

const std::vector<QPointF> &MaxKrugProstogPoligona::getMaxKrugProstogPoligona() const
{
    return _maxKrugProstogPoligona;
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
