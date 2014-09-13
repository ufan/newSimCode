/*
 *  $Id: DmpSimMagneticField.h, 2014-09-05 11:09:32 DAMPE/USTC $
 *  Author(s):
 *    Yifeng Wei (weiyf@mail.ustc.edu.cn) 05/09/2014
*/


#ifndef DmpSimMagneticField_H
#define DmpSimMagneticField_H 1

#include "globals.hh"
#include "G4MagneticField.hh"

class G4GenericMessenger;

/// Magnetic field

class DmpSimMagneticField : public G4MagneticField
{
public:
    DmpSimMagneticField();
    virtual ~DmpSimMagneticField();
    
    virtual void GetFieldValue(const G4double point[4],double* bField ) const;
    
    void SetField(G4double val) { fBy = val; }
    G4double GetField() const { return fBy; }
    
private:
    void DefineCommands();

    G4GenericMessenger* fMessenger;
    G4double fBy;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
