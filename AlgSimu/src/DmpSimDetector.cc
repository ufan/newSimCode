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

#include "DmpSimDetector.h"
//#include "DmpSimPsdSD.h"
//#include "DmpSimStkSD.h"
#include "DmpSimBgoSD.h"
//#include "DmpSimNudSD.h"

//-------------------------------------------------------------------
DmpSimDetector::DmpSimDetector()
 :fParser(0),
  fPhyVolume(0),
//  fPsdSD(0),
//  fStkSD(0),
  fBgoSD(0)
//  fNudSD(0)
{
  fParser = new G4GDMLParser();
//  fPsdSD = new DmpSimPsdSD();
//  fStkSD = new DmpSimStkSD();
  fBgoSD = new DmpSimBgoSD();
//  fNudSD = new DmpSimNudSD();
  fMetadata = dynamic_cast<DmpMetadata*>(gDataBuffer->ReadObject("Metadata/MCTruth/JobOpt"));
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
  SetAncillarySD();
  return fPhyVolume;
}

//-------------------------------------------------------------------
void DmpSimDetector::Adjustment()const{
  short nCmd = fMetadata->OptionSize();
  G4VPhysicalVolume *PV = PV = G4PhysicalVolumeStore::GetInstance()->GetVolume("Ancillary_Det_PV",false);
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
    }else if(command == "BT/PbGlass/Delete"){
      if(G4PhysicalVolumeStore::GetInstance()->GetVolume("Pb_glass_PV",false)){
        G4PhysicalVolumeStore::GetInstance()->DeRegister(G4PhysicalVolumeStore::GetInstance()->GetVolume("Pb_glass_PV",false));
      }
    }
  }

}

//-------------------------------------------------------------------
#include "DmpSimMagneticField.h"
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
#include "DmpSimSSD_SD.h"
void DmpSimDetector::SetAncillarySD(){
  G4SDManager *mgrSD = G4SDManager::GetSDMpointer();
  if(G4LogicalVolumeStore::GetInstance()->GetVolume("AMS_LDR_1",false)){
    DmpLogInfo<<"Setting Sensitive Detector of SSD_1"<<DmpLogEndl;
    static DmpSimSSD_SD AMS_LDR_1_SD("1");
    mgrSD->AddNewDetector(&AMS_LDR_1_SD);
    fParser->GetVolume("AMS_LDR_1")->SetSensitiveDetector(&AMS_LDR_1_SD);
  }
  if(G4LogicalVolumeStore::GetInstance()->GetVolume("AMS_LDR_2",false)){
    DmpLogInfo<<"Setting Sensitive Detector of SSD_2"<<DmpLogEndl;
    static DmpSimSSD_SD AMS_LDR_2_SD("2");
    mgrSD->AddNewDetector(&AMS_LDR_2_SD);
    fParser->GetVolume("AMS_LDR_2")->SetSensitiveDetector(&AMS_LDR_2_SD);
  }
  if(G4LogicalVolumeStore::GetInstance()->GetVolume("AMS_LDR_3",false)){
    DmpLogInfo<<"Setting Sensitive Detector of SSD_3"<<DmpLogEndl;
    static DmpSimSSD_SD AMS_LDR_3_SD("3");
    mgrSD->AddNewDetector(&AMS_LDR_3_SD);
    fParser->GetVolume("AMS_LDR_3")->SetSensitiveDetector(&AMS_LDR_3_SD);
  }
  if(G4LogicalVolumeStore::GetInstance()->GetVolume("AMS_LDR_4",false)){
    DmpLogInfo<<"Setting Sensitive Detector of SSD_4"<<DmpLogEndl;
    static DmpSimSSD_SD AMS_LDR_4_SD("4");
    mgrSD->AddNewDetector(&AMS_LDR_4_SD);
    fParser->GetVolume("AMS_LDR_4")->SetSensitiveDetector(&AMS_LDR_4_SD);
  }
}

