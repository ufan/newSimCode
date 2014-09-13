/*
 *  $Id: DmpSimMagneticField.cc, 2014-09-05 11:09:40 DAMPE/USTC $
 *  Author(s):
 *    Yifeng Wei (weiyf@mail.ustc.edu.cn) 05/09/2014
*/


#include "DmpSimMagneticField.h"

#include "G4GenericMessenger.hh"
#include "G4SystemOfUnits.hh"
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DmpSimMagneticField::DmpSimMagneticField()
: G4MagneticField(), fMessenger(0), fBy(-1.0*tesla)
{
    // define commands for this class
    DefineCommands();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DmpSimMagneticField::~DmpSimMagneticField()
{ 
    delete fMessenger; 
}

void DmpSimMagneticField::GetFieldValue(const G4double [4],double *bField) const
{
    bField[0] = fBy;
    bField[1] = 0.;
    bField[2] = 0.;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DmpSimMagneticField::DefineCommands()
{
        /*
    fMessenger = new G4GenericMessenger(this, 
                                        "/B5/field/", 
                                        "Field control");

    // fieldValue command 
    G4GenericMessenger::Command& valueCmd
      = fMessenger->DeclareMethodWithUnit("value","tesla",
                                  &DmpSimMagneticField::SetField, 
                                  "Set field strength.");
    valueCmd.SetParameterName("field", true);
    valueCmd.SetDefaultValue("1.");
    */
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
