/*
 *  $Id: DmpSimDetector.cc, 2014-09-30 00:15:30 DAMPE $
 *  Author(s):
 *    Chi WANG (chiwang@mail.ustc.edu.cn) 26/02/2014
 *    Yifeng Wei (weiyf@mail.ustc.edu.cn) 28/09/2014
*/

#include <stdlib.h>     // getenv()

#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4GDMLParser.hh"
#include "G4SDManager.hh"
#include "G4FieldManager.hh"
#include "G4VisAttributes.hh"
//-------------------------------------------------------------------
#include "G4Material.hh"
//-------------------------------------------------------------------

#include "DmpLog.h"
#include "DmpSimMagneticField.h"

#include "DmpSimDetector.h"
//#include "DmpSimPsdSD.h"
//#include "DmpSimStkSD.h"
#include "DmpSimBgoSD.h"
//#include "DmpSimNudSD.h"

//-------------------------------------------------------------------
boost::filesystem::path DmpSimDetector::fGdmlPath = (std::string)getenv("DMPSWGEOMETRY")+"/DAMPE.gdml";
double DmpSimDetector::fAuxOffset[3] = {0.0,0.0,0.0};
double DmpSimDetector::fMagneticFieldPosZ  = 0.0;

//-------------------------------------------------------------------
DmpSimDetector::DmpSimDetector()
 :fParser(0),
  fPhyVolume(0),
//  fPsdSD(0),
//  fStkSD(0),
  fBgoSD(0),
//  fNudSD(0),
  fAuxiliaryDet_LV(0)
{
  fParser = new G4GDMLParser();
//  fPsdSD = new DmpSimPsdSD();
//  fStkSD = new DmpSimStkSD();
  fBgoSD = new DmpSimBgoSD();
//  fNudSD = new DmpSimNudSD();
}

//-------------------------------------------------------------------
DmpSimDetector::~DmpSimDetector(){
  delete fParser;
//  delete fPsdSD;
//  delete fStkSD;
  delete fBgoSD;
//  delete fNudSD;
}

//-------------------------------------------------------------------
void DmpSimDetector::SetGdml(const std::string &argv){
  std::string path=fGdmlPath.parent_path().string();
  std::string name=fGdmlPath.filename().string();
  boost::filesystem::path temp(argv);
  if(temp.extension().string() != ""){  // must before updating path
    name = temp.filename().string();
  }else{
    temp = temp.string()+"/";   // user may forgot the last slash of the path. (could has 2 //)
  }
  if((temp.parent_path().string() != "")){  // must after updating name
    path = temp.parent_path().string()+"/";
  }
  fGdmlPath = path+name;
}

//-------------------------------------------------------------------
G4VPhysicalVolume* DmpSimDetector::Construct(){
  char *dirTmp = getcwd(NULL,NULL);
  DmpLogInfo<<"Reading GDML:\t"<<fGdmlPath.string()<<DmpLogEndl;
  chdir(fGdmlPath.parent_path().string().c_str());
  fParser->Read(fGdmlPath.filename().string().c_str());
  fPhyVolume = fParser->GetWorldVolume();
  chdir(dirTmp);
  AdjustAuxiliaryDetectorOfTestBeam();

// *
// *  TODO:  set structure invisable
// *

  // *
  // *  TODO: set SD of SubDet at here
  // *
  G4SDManager *mgrSD = G4SDManager::GetSDMpointer();
  if(fParser->GetVolume("Psd_DetLV")){
    DmpLogInfo<<"Setting Sensitive Detector of Psd"<<DmpLogEndl;
    //mgrSD->AddNewDetector(fPsdSD);
    //fParser->GetVolume("Psd_Strip0LV")->SetSensitiveDetector(fPsdSD);
    //fParser->GetVolume("Psd_Strip1LV")->SetSensitiveDetector(fPsdSD);
  }
  if(fParser->GetVolume("Stk_DetLV")){
          /*
    DmpLogInfo<<" Setting Sensitive Detector of Stk"<<DmpLogEndl;
    mgrSD->AddNewDetector(fStkSD);
    fParser->GetVolume("Stk_StripLV")->SetSensitiveDetector(fStkSD);
    */
  }
  if(fParser->GetVolume("Bgo_DetLV")){
    DmpLogInfo<<"Setting Sensitive Detector of Bgo"<<DmpLogEndl;
    mgrSD->AddNewDetector(fBgoSD);
    fParser->GetVolume("Bgo_BarLV")->SetSensitiveDetector(fBgoSD);
  }
  if(fParser->GetVolume("Nud_DetLV")){
    DmpLogInfo<<"Setting Sensitive Detector of Nud"<<DmpLogEndl;
    //mgrSD->AddNewDetector(fNudSD);
    //fParser->GetVolume("Nud_Block0LV")->SetSensitiveDetector(fNudSD);
    //fParser->GetVolume("Nud_Block1LV")->SetSensitiveDetector(fNudSD);
    //fParser->GetVolume("Nud_Block2LV")->SetSensitiveDetector(fNudSD);
    //fParser->GetVolume("Nud_Block3LV")->SetSensitiveDetector(fNudSD);
  }

  return fPhyVolume;
}

//-------------------------------------------------------------------
void DmpSimDetector::ConstructMaterials(){
// *
// *  TODO: could we delete this function?
// *
  //G4NistManager* nistManager = G4NistManager::Instance(); 
  G4double a;  // mass of a mole;
  G4double z;  // z=mean number of protons;  
  G4double density; 
   // Vacuum
  new G4Material("Galactic", z=1., a=1.01*g/mole,density= universe_mean_density,kStateGas, 2.73*kelvin, 3.e-18*pascal);
}

//-------------------------------------------------------------------
void DmpSimDetector::AdjustAuxiliaryDetectorOfTestBeam(){
    return;
// *
// *  TODO:  will break at here, if not has "AuxDet_box" in other DAMPE.gdml files
// *
  fAuxiliaryDet_LV = fParser->GetVolume("AuxDet_box");
  if(not fAuxiliaryDet_LV){
    return;
  }
  DmpLogInfo<<"Constructing auxiliary detector for beam test"<<DmpLogEndl;
  fAuxiliaryDet_LV->SetVisAttributes(G4VisAttributes::Invisible);
  /*
  int daughterNo = fParser->GetVolume("World")->GetNoDaughters();
  G4cout << "total daughter no = " << daughterNo << G4endl;
  for (int i=0;i<daughterNo;i++){
    G4cout << "The " << i << " th daughter is " << fParser->GetVolume("World")->GetDaughter(i)->GetName() << G4endl; 
  }
  */
// *
// *  TODO:  get translation vector from mac file, correspond to particle source
// *
  fParser->GetVolume("World")->GetDaughter(0)->SetTranslation(G4ThreeVector(fAuxOffset[0]+300,fAuxOffset[1],fAuxOffset[2])); //the 0th daughter of World is auxiliary detector, default offset X in GDML file is 300.
// *
// *  TODO:  will break at here, if not has "B_field" in other DAMPE.gdml files
// *
  G4LogicalVolume *magnetic_LV = fParser->GetVolume("B_field");
  if(magnetic_LV){    //Set magnetic field
    DmpLogInfo<<"Setting magnetic filed"<<DmpLogEndl;
  /*
    int daughterNo = fParser->GetVolume("AuxDet_box")->GetNoDaughters();
    G4cout << "total daughter no = " << daughterNo << G4endl;
    for (int i=0;i<daughterNo;i++){
      G4cout << "The " << i << " th daughter is " << fParser->GetVolume("AuxDet_box")->GetDaughter(i)->GetName() << G4endl; 
    }
  */
    DmpSimMagneticField *magneticField = new DmpSimMagneticField();   // when delete it?
    G4FieldManager *fieldMgr = new G4FieldManager();//G4TransportationManager::GetTransportationManager()->GetFieldManager(); when delete it?
    fieldMgr->SetDetectorField(magneticField);
    fieldMgr->CreateChordFinder(magneticField);
    magnetic_LV->SetFieldManager(fieldMgr,true);
    fAuxiliaryDet_LV->GetDaughter(13)->SetTranslation(G4ThreeVector(0,0,fMagneticFieldPosZ));  //the 13th daughter of auxiliary detector is magnetic field, why should we need to set position? Does it fixed?
  }
}

