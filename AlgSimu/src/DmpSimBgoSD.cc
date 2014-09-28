/*
 *  $Id: DmpSimBgoSD.cc, 2014-09-26 00:25:47 DAMPE $
 *  Author(s):
 *    Chi WANG (chiwang@mail.ustc.edu.cn) 03/03/2014
*/

#include "G4Step.hh"
#include "G4TouchableHistory.hh"

#include "DmpSimBgoSD.h"
#include "DmpEvtMCBgo.h"
#include "DmpDataBuffer.h"
#include "DmpBgoBase.h"

//-------------------------------------------------------------------
DmpSimBgoSD::DmpSimBgoSD()
 :G4VSensitiveDetector("BgoSD"),
  fEvtMCBgo(0)
{
  fEvtMCBgo = new DmpEvtMCBgo();
  if(not gDataBuffer->RegisterObject("Event/MCTruth/Bgo",fEvtMCBgo,"DmpEvtMCBgo")){
   DmpLogError<<"xxxxx how to terminate this run"<<DmpLogEndl;
   throw;
  }
}

//-------------------------------------------------------------------
DmpSimBgoSD::~DmpSimBgoSD(){
}

//-------------------------------------------------------------------
#include <boost/lexical_cast.hpp>
G4bool DmpSimBgoSD::ProcessHits(G4Step *aStep,G4TouchableHistory*){
  G4TouchableHistory *theTouchable = (G4TouchableHistory*)(aStep->GetPreStepPoint()->GetTouchable());
  std::string barName = theTouchable->GetVolume(1)->GetName();
  barName.assign(barName.end()-4,barName.end());        // get ID
  short barID = boost::lexical_cast<short>(barName);
  short GlobalID = DmpBgoBase::ConstructGlobalBarID(barID/100,barID%100);

  G4ThreeVector position = aStep->GetPreStepPoint()->GetPosition();
  fEvtMCBgo->AddG4Hit(GlobalID,aStep->GetTotalEnergyDeposit()/MeV,position.x()/mm,position.y()/mm,position.z()/mm);

  return true;
}

//-------------------------------------------------------------------
void DmpSimBgoSD::Initialize(G4HCofThisEvent*){
// *
// *  TODO:  after DmpRootIOSvc filled this event?
// *
  fEvtMCBgo->Reset();
}

//-------------------------------------------------------------------
void DmpSimBgoSD::EndOfEvent(G4HCofThisEvent* HCE){
}


