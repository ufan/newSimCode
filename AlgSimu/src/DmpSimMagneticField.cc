/*
 *  $Id: DmpSimMagneticField.cc, 2014-09-28 18:20:44 DAMPE $
 *  Author(s):
 *    Yifeng Wei (weiyf@mail.ustc.edu.cn) 05/09/2014
 *    Chi WANG (chiwang@mail.ustc.edu.cn) 26/09/2014
*/

#include "DmpSimMagneticField.h"

//-------------------------------------------------------------------
G4double DmpSimMagneticField::fMField[3] = {0*tesla,0*tesla,0*tesla};

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
    bField[0] = fMField[0];
    bField[1] = fMField[1];
    bField[2] = fMField[2];
}

