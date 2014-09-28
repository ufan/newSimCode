/*
 *  $Id: DmpSimMagneticField.h, 2014-09-28 15:50:27 DAMPE $
 *  Author(s):
 *    Yifeng Wei (weiyf@mail.ustc.edu.cn) 05/09/2014
 *    Chi WANG (chiwang@mail.ustc.edu.cn) 26/09/2014
*/

#ifndef DmpSimMagneticField_H
#define DmpSimMagneticField_H 1

#include "G4SystemOfUnits.hh"
#include "G4MagneticField.hh"

class DmpSimMagneticField : public G4MagneticField{
/*
 *
 */
public:
    DmpSimMagneticField();
    virtual ~DmpSimMagneticField();
    virtual void GetFieldValue(const G4double point[4],double* bField ) const;

public:
    static void SetFieldValue(const double &x,const double &y,const double &z){
      fMField[0]=x*tesla;
      fMField[1]=y*tesla;
      fMField[2]=z*tesla;
    }
    G4double GetField(const short &i) const {return i<3?fMField[i]:-0.9090909;}
    
private:
    static G4double fMField[3];     // magnetic field x(0),y(1),z(2)

};

#endif


