#ifndef MAX_CIRCLE_INSIDE_NONCONVEX_POLYGON_H
#define MAX_CIRCLE_INSIDE_NONCONVEX_POLYGON_H

#include "algoritambaza.h"

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
    QPoint _maxTacka;

    int _povrsina;
    unsigned long _i, _j, _k;
    unsigned int _n = 0;
    std::vector<QLine> _naivniOmotac;
};

#endif // MAX_CIRCLE_INSIDE_NONCONVEX_POLYGON_H
