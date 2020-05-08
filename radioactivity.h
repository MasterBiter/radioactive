#ifndef RADIOACTIVITY_H
#define RADIOACTIVITY_H

#include <QObject>
#include "ttmath.h"

typedef ttmath::Big<TTMATH_BITS(64), TTMATH_BITS(256)> big_shtuka;

class radioactivity : public QObject
{
    Q_OBJECT
public:
    big_shtuka ln2;

    explicit radioactivity(QObject *parent = 0);
    //screen geometry
    int wdth;
    int hght;
    Q_INVOKABLE int wdth_q()
    {
        return wdth;
    }
    Q_INVOKABLE int hght_q()
    {
        return hght;
    }
    //---------------
    struct Isotope
    {
        int charge;
        int mass;
    };
    struct Isotope_group
    {
        Isotope isotope;
        big_shtuka numofmolecules;
        big_shtuka half_life; // in seconds
        double alpha_prob;
        double beta_prob;
    };
    std::vector <Isotope_group> Mix;

    big_shtuka mix_radioactivity();
    big_shtuka isotope_group_radioactivity(Isotope isotope,big_shtuka numofmolecules,big_shtuka half_life,big_shtuka timeIt);
    void doMixDecayIter(big_shtuka timeIt);
    void doIsotopeGroupDecayIter(Isotope_group & isotope_group, big_shtuka timeIt);
    void doAlphaDecay(Isotope_group isotope_group,big_shtuka num_of_decays);
    void doBetaDecay(Isotope_group isotope_group,big_shtuka num_of_decays);
    void getIsotopeInfo(Isotope_group & isotope_group);
signals:
public slots:

};

#endif // RADIOACTIVITY_H
