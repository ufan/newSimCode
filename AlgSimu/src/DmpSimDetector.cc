/*
 *  $Id: DmpSimDetector.cc, 2014-09-30 14:07:44 DAMPE $
 *  Author(s):
 *    Chi WANG (chiwang@mail.ustc.edu.cn) 26/02/2014
 *    Yifeng Wei (weiyf@mail.ustc.edu.cn) 28/09/2014
*/

#include <stdlib.h>     // getenv()

//#include "G4RotationMatrix.hh"
#include "G4GDMLParser.hh"
#include "G4SDManager.hh"
#include "G4FieldManager.hh"
#include "G4VisAttributes.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4PhysicalVolumeStore.hh"

#include "DmpLog.h"
#include "DmpDataBuffer.h"
#include "DmpMetadata.h"
#include "DmpSimMagneticField.h"

#include "DmpSimDetector.h"
//#include "DmpSimPsdSD.h"
//#include "DmpSimStkSD.h"
#include "DmpSimBgoSD.h"
//#include "DmpSimNudSD.h"
#include "DmpSimPbGlassSD.h"
#include "DmpSimSSD_SD.h"

//-------------------------------------------------------------------
DmpSimDetector::DmpSimDetector()
 :fParser(0),
  fPhyVolume(0),
//  fPsdSD(0),
//  fStkSD(0),
  fBgoSD(0),
//  fNudSD(0)
  fPbGlassSD(0)
{
  fParser = new G4GDMLParser();
//  fPsdSD = new DmpSimPsdSD();
//  fStkSD = new DmpSimStkSD();
  fBgoSD = new DmpSimBgoSD();
//  fNudSD = new DmpSimNudSD();
  fPbGlassSD = new DmpSimPbGlassSD();
  fSSD_SD = new DmpSimSSD_SD();
  fMetadata = dynamic_cast<DmpMetadata*>(gDataBuffer->ReadObject("Metadata/MCTruth/JobOpt"));
}

//-------------------------------------------------------------------
DmpSimDetector::~DmpSimDetector(){
  delete fParser;
//  delete fPsdSD;
//  delete fStkSD;
  delete fBgoSD;
//  delete fNudSD;
  if(fPbGlassSD){
    delete fPbGlassSD;
  }
  if(fSSD_SD){
    delete fSSD_SD;
  }
}

//-------------------------------------------------------------------
#include <boost/filesystem.hpp>     // path
G4VPhysicalVolume* DmpSimDetector::Construct(){
  boost::filesystem::path   gdmlFile=fMetadata->GetValue("Gdml");
  if(gdmlFile.extension().string() != ".gdml"){    // argv = sub-directory
    gdmlFile = (std::string)getenv("DMPSWSYS")+"/share/Geometry/"+gdmlFile.string()+"/DAMPE.gdml";
  }
  char *dirTmp = getcwd(NULL,NULL);
  DmpLogInfo<<"Reading GDML:\t"<<gdmlFile.string()<<DmpLogEndl;
  chdir(gdmlFile.parent_path().string().c_str());
  fParser->Read(gdmlFile.filename().string().c_str());
  fPhyVolume = fParser->GetWorldVolume();
  fPhyVolume->GetLogicalVolume()->SetVisAttributes(G4VisAttributes::Invisible);
  chdir(dirTmp);

  Adjustment();

  // *
  // *  TODO: set SD of SubDet at here
  // *
  G4SDManager *mgrSD = G4SDManager::GetSDMpointer();
  if(fParser->GetVolume("Psd_Det")){
    DmpLogInfo<<"Setting Sensitive Detector of Psd"<<DmpLogEndl;
    //mgrSD->AddNewDetector(fPsdSD);
    //fParser->GetVolume("Psd_Strip0")->SetSensitiveDetector(fPsdSD);
    //fParser->GetVolume("Psd_Strip1")->SetSensitiveDetector(fPsdSD);
  }
  if(fParser->GetVolume("Stk_Det")){
          /*
    DmpLogInfo<<" Setting Sensitive Detector of Stk"<<DmpLogEndl;
    mgrSD->AddNewDetector(fStkSD);
    fParser->GetVolume("Stk_Strip")->SetSensitiveDetector(fStkSD);
    */
  }
  if(fParser->GetVolume("Bgo_Det")){
    DmpLogInfo<<"Setting Sensitive Detector of Bgo"<<DmpLogEndl;
    mgrSD->AddNewDetector(fBgoSD);
    fParser->GetVolume("Bgo_Bar")->SetSensitiveDetector(fBgoSD);
  }
  if(fParser->GetVolume("Nud_Det")){
    DmpLogInfo<<"Setting Sensitive Detector of Nud"<<DmpLogEndl;
    //mgrSD->AddNewDetector(fNudSD);
    //fParser->GetVolume("Nud_Block0")->SetSensitiveDetector(fNudSD);
    //fParser->GetVolume("Nud_Block1")->SetSensitiveDetector(fNudSD);
    //fParser->GetVolume("Nud_Block2")->SetSensitiveDetector(fNudSD);
    //fParser->GetVolume("Nud_Block3")->SetSensitiveDetector(fNudSD);
  }
  if(G4LogicalVolumeStore::GetInstance()->GetVolume("PbGlass_Det",false)){
    DmpLogInfo<<"Setting Sensitive Detector of Pb class"<<DmpLogEndl;
    mgrSD->AddNewDetector(fPbGlassSD);
    fParser->GetVolume("PbGlass_Det")->SetSensitiveDetector(fPbGlassSD);
  }
  if(G4LogicalVolumeStore::GetInstance()->GetVolume("S3_Det",false)){
    DmpLogInfo<<"Setting Sensitive Detector of SSD"<<DmpLogEndl;
    mgrSD->AddNewDetector(fSSD_SD);
    fParser->GetVolume("S3_Det")->SetSensitiveDetector(fSSD_SD);
  }

  return fPhyVolume;
}

//-------------------------------------------------------------------
void DmpSimDetector::ResetMagnetic(const double &x,const double &y,const double &z)const{
  G4LogicalVolume *LV = G4LogicalVolumeStore::GetInstance()->GetVolume("B_field",false);
  if(LV){
    static DmpSimMagneticField magneticField(x,y,z);
    static G4FieldManager fieldMgr;
    fieldMgr.SetDetectorField(&magneticField);
    fieldMgr.CreateChordFinder(&magneticField);
    LV->SetFieldManager(&fieldMgr,true);
  }
}

//-------------------------------------------------------------------
void DmpSimDetector::Adjustment()const{
  short nCmd = fMetadata->OptionSize();
  G4VPhysicalVolume *PV = PV = G4PhysicalVolumeStore::GetInstance()->GetVolume("AuxDet_PV",false);
  if(PV){
    PV->GetLogicalVolume()->SetVisAttributes(G4VisAttributes::Invisible);
  }
  for(short i =0; i<nCmd;++i){
    std::string command = fMetadata->GetCommand(i);
    if(command == "BT/DAMPE/Rotation"){
      double rad = 0;
      std::istringstream iss(fMetadata->GetValue(command));
      iss>>rad;
      rad = rad / 180 * 3.141592653;
      static G4RotationMatrix rot;
      rot.rotateY(-rad);
      PV->SetRotation(&rot);
    }else if(command == "BT/DAMPE/Translation"){
      double x=0,y=0.,z=0.0;
      std::istringstream iss(fMetadata->GetValue(command));
      iss>>x>>y>>z;
      G4ThreeVector move(x,y,z);
      G4ThreeVector par = PV->GetTranslation();
      par -= move;
      PV->SetTranslation(par);
    }else if(command == "BT/Magnetic"){
      double x=0,y=0.,z=0.0;
      std::istringstream iss(fMetadata->GetValue(command));
      iss>>x>>y>>z;
      ResetMagnetic(x,y,z);
    }
  }
}


