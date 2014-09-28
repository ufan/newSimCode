/*
 *  $Id: DmpSimMagneticField.cc, 2014-09-05 11:09:40 DAMPE/USTC $
 *  Author(s):
 *    Yifeng Wei (weiyf@mail.ustc.edu.cn) 05/09/2014
*/


#include "DmpSimMagneticField.h"

#include "G4SystemOfUnits.hh"
//#include "globals.hh"

//-------------------------------------------------------------------
G4double DmpSimMagneticField::fBy = -1.0*tesla;

//-------------------------------------------------------------------
DmpSimMagneticField::DmpSimMagneticField()
: G4MagneticField()
{
}

//-------------------------------------------------------------------
DmpSimMagneticField::~DmpSimMagneticField()
{
}

//-------------------------------------------------------------------
void DmpSimMagneticField::GetFieldValue(const G4double [4],double *bField) const
{
    bField[0] = fBy;
    bField[1] = 0.;
    bField[2] = 0.;
}

