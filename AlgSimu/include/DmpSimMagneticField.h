/*
 *  $Id: DmpSimMagneticField.h, 2014-09-28 15:50:27 DAMPE $
 *  Author(s):
 *    Yifeng Wei (weiyf@mail.ustc.edu.cn) 05/09/2014
 *    Chi WANG (chiwang@mail.ustc.edu.cn) 26/09/2014
*/

#ifndef DmpSimMagneticField_H
#define DmpSimMagneticField_H 1

#include "G4MagneticField.hh"

class DmpSimMagneticField : public G4MagneticField{
/*
 *
 */
public:
    DmpSimMagneticField(double,double,double);
    virtual ~DmpSimMagneticField();
    virtual void GetFieldValue(const G4double point[4],double* bField ) const;

private:
     double fMField[3];     // magnetic field x(0),y(1),z(2)

};

#endif


