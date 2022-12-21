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


    const std::vector<QPoint> &getMaxKrugProstogPoligona() const;

private:
    std::vector<QPoint> ucitajPodatkeIzDatoteke(std::string);
    std::vector<QPoint> _tacke;
    std::vector<QPoint> _maxKrugProstogPoligona;
    QPoint _leftBottomRectPoint;
    QPoint _leftTopRectPoint;
    QPoint _rightTopRectPoint;
    QPoint _rightBottomRectPoint;

    std::vector<int> _rectXs;
    std::vector<int> _rectYs;
    std::vector<QPoint> _allPoints;

    int _povrsina;
    unsigned long _i, _j, _k;
    unsigned int _n = 0;
    int _xmin =  std::numeric_limits<int>::max();;
    int _xmax = 0;
    int _ymin =  std::numeric_limits<int>::max();;
    int _ymax = 0;
    std::vector<QLine> _naivniOmotac;
};

#endif // MAX_CIRCLE_INSIDE_NONCONVEX_POLYGON_H
