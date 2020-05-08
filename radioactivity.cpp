#include "radioactivity.h"
radioactivity::radioactivity(QObject *parent) :
    QObject(parent)
{
    ln2 = 2;
    ttmath::Ln(ln2);
    Mix.clear();
}
big_shtuka radioactivity::mix_radioactivity()
{
    big_shtuka result=0;
    for(int i=0;i<Mix.size();i++)
    {
        result+=isotope_group_radioactivity(Mix[i].isotope,Mix[i].numofmolecules,Mix[i].half_life, 1);
    }
    return result;
}

big_shtuka radioactivity::isotope_group_radioactivity(Isotope isotope,big_shtuka numofmolecules,
                                                      big_shtuka half_life, big_shtuka timeIt=1 /*in seconds*/)
{
    big_shtuka pow2thing = 2;
    pow2thing.Pow(timeIt / half_life * (-1) );
    return numofmolecules*ln2/half_life*pow2thing;
}

void radioactivity::doMixDecayIter(big_shtuka timeIt=1 /*in seconds*/)
{
    int stsize=Mix.size();
    for(int i=0;i<stsize;i++)
    {
        doIsotopeGroupDecayIter(Mix[i],timeIt);
    }
}

void radioactivity::doIsotopeGroupDecayIter(Isotope_group & isotope_group, big_shtuka timeIt)
{
    big_shtuka num_of_decays = isotope_group_radioactivity(isotope_group.isotope,isotope_group.numofmolecules,isotope_group.half_life,timeIt);
    isotope_group.numofmolecules-=num_of_decays;
    if(isotope_group.numofmolecules>0.5)
    {
        doAlphaDecay(isotope_group,num_of_decays*isotope_group.alpha_prob);
        doBetaDecay(isotope_group,num_of_decays*isotope_group.beta_prob);
    }
    else
    {
        isotope_group.numofmolecules=0;
    }
}

void radioactivity::doAlphaDecay(Isotope_group isotope_group,big_shtuka num_of_decays)
{
    if(num_of_decays > 0.5)
    {
    Isotope_group new_isotope_group;
    new_isotope_group.isotope.charge = isotope_group.isotope.charge - 2;
    new_isotope_group.isotope.mass = isotope_group.isotope.mass - 4;
    new_isotope_group.numofmolecules = num_of_decays;
    }
}

void radioactivity::doBetaDecay(Isotope_group isotope_group,big_shtuka num_of_decays)
{
    if(num_of_decays>0.5)
    {
    Isotope_group new_isotope_group;
    new_isotope_group.isotope.charge = isotope_group.isotope.charge + 1;
    new_isotope_group.isotope.mass = isotope_group.isotope.mass;
    new_isotope_group.numofmolecules = num_of_decays;
    getIsotopeInfo(new_isotope_group);
    Mix.push_back(new_isotope_group);
    }
}

void radioactivity::getIsotopeInfo(Isotope_group & isotope_group)
{
    isotope_group.alpha_prob=0.999999999896;
    isotope_group.beta_prob=0.000000000104;
    isotope_group.half_life=7535983574398573;
}



