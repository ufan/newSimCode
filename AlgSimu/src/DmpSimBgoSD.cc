/*
 *  $Id: DmpSimBgoSD.cc, 2014-08-20 15:44:18 DAMPE/USTC $
 *  Author(s):
 *    Chi WANG (chiwang@mail.ustc.edu.cn) 03/03/2014
*/

#include "TClonesArray.h"

#include "G4Step.hh"
#include "G4TouchableHistory.hh"

#include "DmpSimBgoSD.h"
#include "DmpEvtMCBgo.h"
#include "DmpDataBuffer.h"

//-------------------------------------------------------------------
DmpSimBgoSD::DmpSimBgoSD()
 :G4VSensitiveDetector("BgoSD"),
  fEvtMCBgo(0)
{
  fEvtMCBgo = new DmpEvtMCBgo();
// *
// *  TODO:  check Register status
// *
  gDataBuffer->RegisterObject("Event/MCTruth/Bgo",fEvtMCBgo,"DmpEvtMCBgo");
}

//-------------------------------------------------------------------
DmpSimBgoSD::~DmpSimBgoSD(){
  delete fEvtMCBgo;
}

//-------------------------------------------------------------------
#include <boost/lexical_cast.hpp>
G4bool DmpSimBgoSD::ProcessHits(G4Step *aStep,G4TouchableHistory*){
  G4TouchableHistory *theTouchable = (G4TouchableHistory*)(aStep->GetPreStepPoint()->GetTouchable());
  std::string barName = theTouchable->GetVolume(1)->GetName();
  barName.assign(barName.end()-4,barName.end());        // get ID
  short barID = boost::lexical_cast<int>(barName);

  G4ThreeVector position = aStep->GetPreStepPoint()->GetPosition();
  fEvtMCBgo->AddG4Hit(barID,aStep->GetTotalEnergyDeposit()/MeV,position.x()/mm,position.y()/mm,position.z()/mm);
/*  
  bool NewBarFlag = true;

  for(int i=0;i<(fEvtMCBgo->GetGlobalBarIDCollection()).size();++i){
    if((fEvtMCBgo->GetGlobalBarIDCollection()).at(i)==barID){
      fEvtMCBgo->AddG4Hit(i,aStep->GetTotalEnergyDeposit()/MeV,position.x()/mm,position.y()/mm,position.z()/mm);
      NewBarFlag = false;
      break;
    }
  }
  if(NewBarFlag==true){
    DmpLogDebug<<"\this is a new bar: "<<barID<<DmpLogEndl;
  }
  aBar->AddG4Hit(aStep->GetTotalEnergyDeposit()/MeV,position.x()/mm,position.y()/mm,position.z()/mm);
*/  
  return true;
}

//-------------------------------------------------------------------
void DmpSimBgoSD::Initialize(G4HCofThisEvent*){
// *
// *  TODO:  after DmpRootIOSvc filled this event?
// *
  fEvtMCBgo->Clear();
}

//-------------------------------------------------------------------
void DmpSimBgoSD::EndOfEvent(G4HCofThisEvent* HCE){
}


