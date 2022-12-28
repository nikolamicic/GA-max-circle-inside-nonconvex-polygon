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
    void ReducingRectangleArea();
    void CurrentPointWithCenterOfMaxCircle();
    bool PointInPolygon(QPointF point, std::vector<QPointF> polygon);
    float MinDistanceOfPoint(QPointF point, std::vector<QPointF> polygon);

    bool onLine(QLineF l1, QPointF p);
    int direction(QPointF a, QPointF b, QPointF c);
    bool isIntersect(QLineF l1, QLineF l2);
    bool checkInside(std::vector<QPointF> poly, int nvert, QPointF p);

    std::vector<QPointF> ucitajPodatkeIzDatoteke(std::string);
    std::vector<QPointF> _tacke;
    QPointF _leftBottomRectPoint;
    QPointF _leftTopRectPoint;
    QPointF _rightTopRectPoint;
    QPointF _rightBottomRectPoint;

    QPointF _final_max_circle_point;
    float _final_max_dist = 0.0;

    std::vector<float> _rectXs;
    std::vector<float> _rectYs;
    std::vector<QPointF> _allPoints;
    std::vector<QPointF> _polygonPoints;

    unsigned int _k;

    float _xmin =  std::numeric_limits<float>::max();;
    float _xmax = 0;
    float _ymin =  std::numeric_limits<float>::max();;
    float _ymax = 0;
    std::vector<QLineF> _naivniOmotac;
};

#endif // MAX_CIRCLE_INSIDE_NONCONVEX_POLYGON_H
