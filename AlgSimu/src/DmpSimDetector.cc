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
  boost::filesystem::path   gdmlFile=fMetadata->Option["Gdml"];
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

  return fPhyVolume;
}

//-------------------------------------------------------------------
void DmpSimDetector::AdjustmentRotation(const double &rad)const{
  G4VPhysicalVolume *PV = G4PhysicalVolumeStore::GetInstance()->GetVolume("AuxDet_PV",false);
  std::cout<<"DEBUG: "<<__FILE__<<"("<<__LINE__<<")"<<std::endl;
  if(PV){
  std::cout<<"DEBUG: "<<__FILE__<<"("<<__LINE__<<")"<<std::endl;
    PV->GetLogicalVolume()->SetVisAttributes(G4VisAttributes::Invisible);
    static G4RotationMatrix rot;
    rot.rotateY(rad);
    PV->SetRotation(&rot);
  }
}

//-------------------------------------------------------------------
void DmpSimDetector::AdjustmentTranslation(const G4ThreeVector &v)const{
  G4VPhysicalVolume *PV = G4PhysicalVolumeStore::GetInstance()->GetVolume("AuxDet_PV",false);
  std::cout<<"DEBUG: "<<__FILE__<<"("<<__LINE__<<")"<<std::endl;
  if(PV){
  std::cout<<"DEBUG: "<<__FILE__<<"("<<__LINE__<<")"<<std::endl;
    PV->GetLogicalVolume()->SetVisAttributes(G4VisAttributes::Invisible);
    G4ThreeVector par = PV->GetTranslation();
    par -= v;
    PV->SetTranslation(par);
  }
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
  for(std::map<std::string,std::string>::iterator it=fMetadata->Option.begin();it!=fMetadata->Option.end();++it){
std::cout<<"DEBUG: "<<__FILE__<<"("<<__LINE__<<")"<<it->first<<std::endl;
    if(it->first.find("BT/DAMPE") != std::string::npos){
      if(it->first.find("Rotation") != std::string::npos){
        double rad = 0;
        std::istringstream iss(it->second);
        iss>>rad;
        rad = rad / 180 * 3.141592653;
        AdjustmentRotation(-rad);
      }else if(it->first.find("Translation") != std::string::npos){
        double x=0,y=0.,z=0.0;
        std::istringstream iss(it->second);
        iss>>x>>y>>z;
        AdjustmentTranslation(G4ThreeVector(x,y,z));
      }
    }else if(it->first.find("BT/Magnetic") != std::string::npos){
      double x=0,y=0.,z=0.0;
      std::istringstream iss(it->second);
      iss>>x>>y>>z;
      ResetMagnetic(x,y,z);
    }
  }
}


