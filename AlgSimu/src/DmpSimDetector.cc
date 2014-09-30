/*
 *  $Id: DmpSimDetector.cc, 2014-09-30 14:07:44 DAMPE $
 *  Author(s):
 *    Chi WANG (chiwang@mail.ustc.edu.cn) 26/02/2014
 *    Yifeng Wei (weiyf@mail.ustc.edu.cn) 28/09/2014
*/

#include <stdlib.h>     // getenv()

#include "G4LogicalVolume.hh"
#include "G4GDMLParser.hh"
#include "G4SDManager.hh"
#include "G4FieldManager.hh"
#include "G4VisAttributes.hh"
#include "G4LogicalVolumeStore.hh"

#include "DmpLog.h"
#include "DmpSimMagneticField.h"

#include "DmpSimDetector.h"
//#include "DmpSimPsdSD.h"
//#include "DmpSimStkSD.h"
#include "DmpSimBgoSD.h"
//#include "DmpSimNudSD.h"

//-------------------------------------------------------------------
boost::filesystem::path DmpSimDetector::fGdmlPath = "NO";
double DmpSimDetector::fAuxOffset[3] = {0.0,0.0,0.0};

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
  if(argv.find(".gdml") != std::string::npos){  // argv = path/filename.gdml
    fGdmlPath = argv;
  }else{        // argv = sub-directory
    fGdmlPath = (std::string)getenv("DMPSWSYS")+"/share/Geometry/"+argv+"/DAMPE.gdml";
  }
}

//-------------------------------------------------------------------
G4VPhysicalVolume* DmpSimDetector::Construct(){
  char *dirTmp = getcwd(NULL,NULL);
  if(fGdmlPath == "NO"){
    DmpLogError<<"Must setup GDML file."<<DmpLogEndl;
    throw;
  }else{
    DmpLogInfo<<"Reading GDML:\t"<<fGdmlPath.string()<<DmpLogEndl;
    chdir(fGdmlPath.parent_path().string().c_str());
    fParser->Read(fGdmlPath.filename().string().c_str());
    fPhyVolume = fParser->GetWorldVolume();
  }
  chdir(dirTmp);

  fAuxiliaryDet_LV = G4LogicalVolumeStore::GetInstance()->GetVolume("AuxDet_box",false);    // must use G4LogicalVolumeStore::GetVolume(), fParser->GetVolume() will throw exception and quit all
  if(fAuxiliaryDet_LV){
    DmpLogInfo<<"Constructing auxiliary detector for beam test"<<DmpLogEndl;
    AdjustAuxiliaryDetectorOfTestBeam();
  }

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
void DmpSimDetector::AdjustAuxiliaryDetectorOfTestBeam(){
  fAuxiliaryDet_LV->SetVisAttributes(G4VisAttributes::Invisible);
// *
// *  TODO:  get translation vector from mac file, correspond to particle source
// *
  fParser->GetVolume("World")->GetDaughter(0)->SetTranslation(G4ThreeVector(fAuxOffset[0]+300,fAuxOffset[1],fAuxOffset[2])); //the 0th daughter of World is auxiliary detector, default offset X in GDML file is 300.
  G4LogicalVolume *magnetic_LV = fParser->GetVolume("B_field");
  if(magnetic_LV){    //Set magnetic field
    DmpLogInfo<<"Setting magnetic filed"<<DmpLogEndl;
    DmpSimMagneticField *magneticField = new DmpSimMagneticField();   // when delete it?
    G4FieldManager *fieldMgr = new G4FieldManager();//G4TransportationManager::GetTransportationManager()->GetFieldManager(); when delete it?
    fieldMgr->SetDetectorField(magneticField);
    fieldMgr->CreateChordFinder(magneticField);
    magnetic_LV->SetFieldManager(fieldMgr,true);
  }
}

