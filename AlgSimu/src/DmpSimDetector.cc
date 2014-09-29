/*
 *  $Id: DmpSimDetector.cc, 2014-09-28 18:22:38 DAMPE $
 *  Author(s):
 *    Chi WANG (chiwang@mail.ustc.edu.cn) 26/02/2014
*/

#include <unistd.h>

#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4GDMLParser.hh"
#include "G4SDManager.hh"
//-------------------------------------------------------------------
#include "G4VSolid.hh"
#include "G4Box.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"
#include "G4VisAttributes.hh"
//-------------------------------------------------------------------

#include "DmpSimMagneticField.h"

#include "DmpSimDetector.h"
//#include "DmpSimPsdSD.h"
//#include "DmpSimStkSD.h"
#include "DmpSimBgoSD.h"
//#include "DmpSimNudSD.h"

//-------------------------------------------------------------------
std::string DmpSimDetector::fGdmlPath = "NO";
bool DmpSimDetector::fSimBT2014_On = false;
double DmpSimDetector::fAuxOffsetX = 0;
double DmpSimDetector::fAuxOffsetY = 0;
double DmpSimDetector::fMagneticFieldPosZ  = -5000;

//-------------------------------------------------------------------
DmpSimDetector::DmpSimDetector()
 :fParser(0),
  fPhyVolume(0),
//  fPsdSD(0),
//  fStkSD(0),
  fBgoSD(0),
//  fNudSD(0),
  fWorldPhyVolume(0)
{
  fParser = new G4GDMLParser();
//  fPsdSD = new DmpSimPsdSD();
//  fStkSD = new DmpSimStkSD();
  fBgoSD = new DmpSimBgoSD();
//  fNudSD = new DmpSimNudSD();
  for (int i=0;i<15;i++){
    fBTAuxParser[i] = new G4GDMLParser();
  }
}

//-------------------------------------------------------------------
DmpSimDetector::~DmpSimDetector(){
  delete fParser;
//  delete fPsdSD;
//  delete fStkSD;
  delete fBgoSD;
//  delete fNudSD;
  for (int i=0;i<15;i++){
      delete fBTAuxParser[i];
  }
}

//-------------------------------------------------------------------
G4VPhysicalVolume* DmpSimDetector::Construct(){
  char *dirTmp = getcwd(NULL,NULL);
  if(fGdmlPath == "NO"){
     G4cout<<"Error: must setup detector in job option file. SetGdml(GdmlFilePath)"<<G4endl;
     return 0;
  }else{
    chdir(fGdmlPath.c_str());
    fParser->Read("DAMPE.gdml");
    fPhyVolume = fParser->GetWorldVolume();
    if (fSimBT2014_On==true){
        fBTAuxParser[0]->Read("LDR0_Ascii.gdml");
        fBTAuxParser[1]->Read("NaI_Ascii.gdml");
        fBTAuxParser[2]->Read("S0_Ascii.gdml");
        fBTAuxParser[3]->Read("S1_Ascii.gdml");
        fBTAuxParser[4]->Read("S2_Ascii.gdml");
        fBTAuxParser[5]->Read("S3_Ascii.gdml");
        fBTAuxParser[6]->Read("Sh_Ascii.gdml");
        fBTAuxParser[7]->Read("SSD0_Ascii.gdml");
        fBTAuxParser[8]->Read("SSD1_Ascii.gdml");
    }
  }
  chdir(dirTmp);

  if(fSimBT2014_On==true){
    G4Material* defaultMaterial = G4Material::GetMaterial("Galactic");
    G4VSolid* WorldSolid = new G4Box("WorldBox",20.*m,20.*m,20.*m);
    G4LogicalVolume* WorldLog = new G4LogicalVolume(WorldSolid,defaultMaterial,"WorldLogical");
    fWorldPhyVolume = new G4PVPlacement(0,G4ThreeVector(),WorldLog,"World",0,false,0);
    WorldLog->SetVisAttributes(G4VisAttributes::Invisible);

    //Magnetic field volume construction
    G4VSolid* magneticSolid = new G4Box("magneticBox",0.6*m,0.6*m,0.3*m);
    G4LogicalVolume* fMagneticLogical = new G4LogicalVolume(magneticSolid,defaultMaterial,"magneticLogical");
    new G4PVPlacement(0,G4ThreeVector(fAuxOffsetX,fAuxOffsetY,fMagneticFieldPosZ),"magneticPhysical",fMagneticLogical,fWorldPhyVolume,false,0);

    //Set magnetic field
    DmpSimMagneticField* fMagneticField = new DmpSimMagneticField();
    G4FieldManager* fFieldMgr = new G4FieldManager();//G4TransportationManager::GetTransportationManager()->GetFieldManager();
    fFieldMgr->SetDetectorField(fMagneticField);
    fFieldMgr->CreateChordFinder(fMagneticField);
    fMagneticLogical->SetFieldManager(fFieldMgr,true);

    // 
    fPhyVolume = new G4PVPlacement(0,G4ThreeVector(0,0,0),"DAMPEPhysical",fParser->GetVolume("World"),fWorldPhyVolume,false,0);
    fBTAuxPhyVolume[0] = new G4PVPlacement(0,G4ThreeVector(fAuxOffsetX+3000,fAuxOffsetY,0),"LDR0",fBTAuxParser[0]->GetVolume("LDR0"),fWorldPhyVolume,false,0);
    fBTAuxPhyVolume[1] = new G4PVPlacement(0,G4ThreeVector(fAuxOffsetX+3000,fAuxOffsetY,0),"NaI",fBTAuxParser[1]->GetVolume("NaI"),fWorldPhyVolume,false,0);
    fBTAuxPhyVolume[2] = new G4PVPlacement(0,G4ThreeVector(fAuxOffsetX+3000,fAuxOffsetY,0),"S0",fBTAuxParser[2]->GetVolume("S0"),fWorldPhyVolume,false,0);
    fBTAuxPhyVolume[3] = new G4PVPlacement(0,G4ThreeVector(fAuxOffsetX+3000,fAuxOffsetY,0),"S1",fBTAuxParser[3]->GetVolume("S1"),fWorldPhyVolume,false,0);
    fBTAuxPhyVolume[4] = new G4PVPlacement(0,G4ThreeVector(fAuxOffsetX+3000,fAuxOffsetY,0),"S2",fBTAuxParser[4]->GetVolume("S2"),fWorldPhyVolume,false,0);
    fBTAuxPhyVolume[5] = new G4PVPlacement(0,G4ThreeVector(fAuxOffsetX+3000,fAuxOffsetY,0),"S3",fBTAuxParser[5]->GetVolume("S3"),fWorldPhyVolume,false,0);
    fBTAuxPhyVolume[6] = new G4PVPlacement(0,G4ThreeVector(fAuxOffsetX+3000,fAuxOffsetY,0),"Sh",fBTAuxParser[6]->GetVolume("Sh"),fWorldPhyVolume,false,0);
    fBTAuxPhyVolume[7] = new G4PVPlacement(0,G4ThreeVector(fAuxOffsetX+3000,fAuxOffsetY,0),"SSD0",fBTAuxParser[7]->GetVolume("SSD0"),fWorldPhyVolume,false,0);
    fBTAuxPhyVolume[8] = new G4PVPlacement(0,G4ThreeVector(fAuxOffsetX+3000,fAuxOffsetY,0),"SSD1",fBTAuxParser[8]->GetVolume("SSD1"),fWorldPhyVolume,false,0);
  }else{
    fWorldPhyVolume = fPhyVolume;
  }

// *
// *  TODO:  set structure invisable
// *

  // *
  // *  TODO: set SD of SubDet at here
  // *
  G4SDManager *mgrSD = G4SDManager::GetSDMpointer();
  if(fParser->GetVolume("Psd_DetLV")){
    std::cout<<" Setting Sensitive Detector of Psd"<<std::endl;
    //mgrSD->AddNewDetector(fPsdSD);
    //fParser->GetVolume("Psd_Strip0LV")->SetSensitiveDetector(fPsdSD);
    //fParser->GetVolume("Psd_Strip1LV")->SetSensitiveDetector(fPsdSD);
  }
  if(fParser->GetVolume("Stk_DetLV")){
          /*
    std::cout<<" Setting Sensitive Detector of Stk"<<std::endl;
    mgrSD->AddNewDetector(fStkSD);
    fParser->GetVolume("Stk_StripLV")->SetSensitiveDetector(fStkSD);
    */
  }
  if(fParser->GetVolume("Bgo_DetLV")){
    std::cout<<" Setting Sensitive Detector of Bgo"<<std::endl;
    mgrSD->AddNewDetector(fBgoSD);
    fParser->GetVolume("Bgo_BarLV")->SetSensitiveDetector(fBgoSD);
//-------------------------------------------------------------------
    //fParser->GetWorldVolume("Bgo_Det")->SetTranslation(G4ThreeVector(0,-400,300));
    //fParser->GetVolume("World")->GetDaughter(3)->SetTranslation(G4ThreeVector(0,400,300));
    //std::cout<<"xxxx\n\t"<<fParser->GetVolume("World")->GetDaughter(3)->GetName()<<std::endl;//SetTranslation(G4ThreeVector(0,400,300));
//-------------------------------------------------------------------
  }
  if(fParser->GetVolume("Nud_DetLV")){
    std::cout<<" Setting Sensitive Detector of Nud"<<std::endl;
    //mgrSD->AddNewDetector(fNudSD);
    //fParser->GetVolume("Nud_Block0LV")->SetSensitiveDetector(fNudSD);
    //fParser->GetVolume("Nud_Block1LV")->SetSensitiveDetector(fNudSD);
    //fParser->GetVolume("Nud_Block2LV")->SetSensitiveDetector(fNudSD);
    //fParser->GetVolume("Nud_Block3LV")->SetSensitiveDetector(fNudSD);
  }

  return fWorldPhyVolume;
}

void DmpSimDetector::ConstructMaterials(){
// *
// *  TODO: why needed? 
// *
  //G4NistManager* nistManager = G4NistManager::Instance(); 
  G4double a;  // mass of a mole;
  G4double z;  // z=mean number of protons;  
  G4double density; 
   // Vacuum
  new G4Material("Galactic", z=1., a=1.01*g/mole,density= universe_mean_density,kStateGas, 2.73*kelvin, 3.e-18*pascal);
}


