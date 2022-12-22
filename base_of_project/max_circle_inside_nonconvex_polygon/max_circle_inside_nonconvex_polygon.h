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


    QPointF getMaxCirclePointCenter() const;

private:
    bool PointInPolygon(QPointF point, std::vector<QPointF> polygon);
    float MinDistanceOfPoint(QPointF point, std::vector<QPointF> polygon);
    std::vector<QPointF> ucitajPodatkeIzDatoteke(std::string);
    std::vector<QPointF> _tacke;
    std::vector<QPointF> _maxKrugProstogPoligona;
    QPointF _leftBottomRectPoint;
    QPointF _leftTopRectPoint;
    QPointF _rightTopRectPoint;
    QPointF _rightBottomRectPoint;

    QPointF _final_max_circle_point;
    float _final_max_dist;

    std::vector<float> _rectXs;
    std::vector<float> _rectYs;
    std::vector<QPointF> _allPoints;
    std::vector<QPointF> _polygonPoints;


    int _povrsina;
    unsigned long _i, _j, _k;
    unsigned int _n = 0;
    float _xmin =  std::numeric_limits<float>::max();;
    float _xmax = 0;
    float _ymin =  std::numeric_limits<float>::max();;
    float _ymax = 0;
    std::vector<QLineF> _naivniOmotac;
};

#endif // MAX_CIRCLE_INSIDE_NONCONVEX_POLYGON_H
