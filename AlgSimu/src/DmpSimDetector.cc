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
double DmpSimDetector::fMagneticFieldPosZ  = 0;

//-------------------------------------------------------------------
DmpSimDetector::DmpSimDetector()
 :fParser(0),
  fPhyVolume(0),
//  fPsdSD(0),
//  fStkSD(0),
  fBgoSD(0),
//  fNudSD(0),
  fMagneticLogical(0)
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
G4VPhysicalVolume* DmpSimDetector::Construct(){
  char *dirTmp = getcwd(NULL,NULL);
  if(fGdmlPath == "NO"){
     G4cout<<"Error: must setup detector in job option file. SetGdml(GdmlFilePath)"<<G4endl;
     return 0;
  }else{
    chdir("../../"); //this will be set $DMPSWSYS + "/share" in official version 
    fParser->Read(fGdmlPath.c_str());
    fPhyVolume = fParser->GetWorldVolume();
  }
  chdir(dirTmp);

  if(fSimBT2014_On==true){

    //Set magnetic field
    if(fParser->GetVolume("B_field")){
    /*
      int daughterNo = fParser->GetVolume("AuxDet_box")->GetNoDaughters();
      G4cout << "total daughter no = " << daughterNo << G4endl;
      for (int i=0;i<daughterNo;i++){
        G4cout << "The " << i << " th daughter is " << fParser->GetVolume("AuxDet_box")->GetDaughter(i)->GetName() << G4endl; 
      }
    */
      fParser->GetVolume("AuxDet_box")->GetDaughter(13)->SetTranslation(G4ThreeVector(0,0,fMagneticFieldPosZ));  //the 13th daughter of auxiliary detector is magnetic field

      fMagneticLogical = fParser->GetVolume("B_field");
      DmpSimMagneticField* fMagneticField = new DmpSimMagneticField();
      G4FieldManager* fFieldMgr = new G4FieldManager();//G4TransportationManager::GetTransportationManager()->GetFieldManager();
      fFieldMgr->SetDetectorField(fMagneticField);
      fFieldMgr->CreateChordFinder(fMagneticField);
      fMagneticLogical->SetFieldManager(fFieldMgr,true);
    }
    else{G4cout << "Error: Cannot find magnetic filed logical volume!" << G4endl;}
  }

    if(fParser->GetVolume("AuxDet_box")){
      fParser->GetVolume("AuxDet_box")->SetVisAttributes(G4VisAttributes::Invisible);
      /*
      int daughterNo = fParser->GetVolume("World")->GetNoDaughters();
      G4cout << "total daughter no = " << daughterNo << G4endl;
      for (int i=0;i<daughterNo;i++){
        G4cout << "The " << i << " th daughter is " << fParser->GetVolume("World")->GetDaughter(i)->GetName() << G4endl; 
      }
      */
      fParser->GetVolume("World")->GetDaughter(0)->SetTranslation(G4ThreeVector(fAuxOffsetX+300,fAuxOffsetY,0)); //the 0th daughter of World is auxiliary detector, default offset X in GDML file is 300.
    }
    else{G4cout << "Error: Cannot find auxiliary detector volume!" << G4endl;}

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

  return fPhyVolume;
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


