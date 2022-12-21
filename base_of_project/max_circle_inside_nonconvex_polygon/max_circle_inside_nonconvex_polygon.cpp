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
        _tacke = generisiNasumicneTacke(brojTacaka);
    else
        _tacke = ucitajPodatkeIzDatoteke(imeDatoteke);
    _k = _tacke.size();
}

void MaxKrugProstogPoligona::pokreniAlgoritam() {
    // Slozenost ovakvog (Gremovog) algoritma: O(nlogn).
    // Dominira sortiranje, dok su ostali koraci linearni.
    _maxTacka = _tacke[0];

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

    _maxKrugProstogPoligona.push_back(_maxTacka);
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
    pen.setColor(Qt::blue);
    painter->setPen(pen);
    for(auto i = 1ul; i < _tacke.size(); i++) {
        qDebug("crtanje linije za tacku: %lul",i);
        painter->drawLine(QLine(_tacke[i-1],_tacke[i]));
       //painter->drawLine(_tacke[i-1], _tacke[i]);
    }
    painter->drawLine(QLine(_tacke[_tacke.size()-1],_tacke[0]));
    /*)
    pen.setColor(Qt::blue);
    painter->setPen(pen);

    painter->drawLine(_tacke[_tacke.size()-1], _tacke[0]);
    */
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

const std::vector<QPoint> &MaxKrugProstogPoligona::getMaxKrugProstogPoligona() const
{
    return _maxKrugProstogPoligona;
}

/* Citanje pravougaonika iz datoteke */
std::vector<QPoint> MaxKrugProstogPoligona::ucitajPodatkeIzDatoteke(std::string imeDatoteke)
{
    /* Otvaranje zadatog fajla */
    std::ifstream inputFile(imeDatoteke);
    std::vector<QPoint> points;

    float x, y;
    while(inputFile >> x >> y)
    {
        QPoint p(x,y);
        points.push_back(p);
    }

    return points;
}
