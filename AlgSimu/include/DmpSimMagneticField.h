/*
 *  $Id: DmpSimMagneticField.h, 2014-09-28 15:50:27 DAMPE $
 *  Author(s):
 *    Yifeng Wei (weiyf@mail.ustc.edu.cn) 05/09/2014
*/

#ifndef DmpSimMagneticField_H
#define DmpSimMagneticField_H 1

#include "globals.hh"
#include "G4MagneticField.hh"

class DmpSimMagneticField : public G4MagneticField
{
/*
 *
 */
public:
    DmpSimMagneticField();
    virtual ~DmpSimMagneticField();
    
    virtual void GetFieldValue(const G4double point[4],double* bField ) const;
    
    static void SetFieldValue(const double &val) {fBy = val*tesla;}
    G4double GetField() const { return fBy; }
    
private:
    static G4double fBy;

};

#endif


