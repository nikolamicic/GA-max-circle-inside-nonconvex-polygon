#ifndef MAX_CIRCLE_INSIDE_NONCONVEX_POLYGON_H
#define MAX_CIRCLE_INSIDE_NONCONVEX_POLYGON_H

#include "algoritambaza.h"
#include <limits>

class MaxKrugProstogPoligona : public AlgoritamBaza
{
public:
    MaxKrugProstogPoligona(QWidget *pCrtanje,
                 int pauzaKoraka,
                 const bool &naivni = false,
                 std::string imeDatoteke = "",
                 int brojTacaka = BROJ_SLUCAJNIH_OBJEKATA);

    void pokreniAlgoritam() final;
    void crtajAlgoritam(QPainter *painter) const final;
    void pokreniNaivniAlgoritam() final;
    void crtajNaivniAlgoritam(QPainter *painter) const final;


    const std::vector<QPointF> &getMaxKrugProstogPoligona() const;

private:
    bool PointInPolygon(QPointF point, std::vector<QPointF> polygon);
    std::vector<QPointF> ucitajPodatkeIzDatoteke(std::string);
    std::vector<QPointF> _tacke;
    std::vector<QPointF> _maxKrugProstogPoligona;
    QPointF _leftBottomRectPoint;
    QPointF _leftTopRectPoint;
    QPointF _rightTopRectPoint;
    QPointF _rightBottomRectPoint;

    std::vector<int> _rectXs;
    std::vector<int> _rectYs;
    std::vector<QPointF> _allPoints;
    std::vector<QPointF> _polygonPoints;

    int _povrsina;
    unsigned long _i, _j, _k;
    unsigned int _n = 0;
    int _xmin =  std::numeric_limits<int>::max();;
    int _xmax = 0;
    int _ymin =  std::numeric_limits<int>::max();;
    int _ymax = 0;
    std::vector<QLineF> _naivniOmotac;
};

#endif // MAX_CIRCLE_INSIDE_NONCONVEX_POLYGON_H
