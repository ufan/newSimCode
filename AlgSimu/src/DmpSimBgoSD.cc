/*
 *  $Id: DmpSimBgoSD.cc, 2014-10-06 17:13:16 DAMPE $
 *  Author(s):
 *    Chi WANG (chiwang@mail.ustc.edu.cn) 03/03/2014
*/

#include "G4Step.hh"
#include "G4TouchableHistory.hh"

#include "DmpSimBgoSD.h"
#include "DmpEvtBgoHits.h"
#include "DmpDataBuffer.h"
#include "DmpBgoBase.h"

//-------------------------------------------------------------------
DmpSimBgoSD::DmpSimBgoSD()
 :G4VSensitiveDetector("BgoSD"),
  fEvtMCBgo(0)
{
  fEvtMCBgo = new DmpEvtBgoHits();
  gDataBuffer->RegisterObject("Event/MCTruth/Bgo",fEvtMCBgo,"DmpEvtBgoHits");
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
  barID = DmpBgoBase::ConstructGlobalBarID(barID/100,barID%100);
  AddThisG4Hit(barID,aStep->GetTotalEnergyDeposit()/MeV,aStep->GetPreStepPoint()->GetPosition());
  return true;
}

//-------------------------------------------------------------------
void DmpSimBgoSD::Initialize(G4HCofThisEvent*){
  fEvtMCBgo->Reset();
}

//-------------------------------------------------------------------
void DmpSimBgoSD::EndOfEvent(G4HCofThisEvent* HCE){
}

//-------------------------------------------------------------------
void DmpSimBgoSD::AddThisG4Hit(const short &id,const double &e,const G4ThreeVector &in){
  TVector3 pos(in.x(),in.y(),in.z());
  for(size_t i=0;i<fEvtMCBgo->fGlobalBarID.size();i++){
    if(fEvtMCBgo->fGlobalBarID.at(i) == id){
      double totE = e + fEvtMCBgo->fEnergy.at(i);
      fEvtMCBgo->fPosition.at(i) = fEvtMCBgo->fPosition.at(i) * (fEvtMCBgo->fEnergy.at(i) / totE);
      fEvtMCBgo->fPosition.at(i) += pos * (e / totE);
      fEvtMCBgo->fEnergy.at(i) = totE;
      return;   // if found gid, update and return
    }
  }
  // if not, creat a new one
  fEvtMCBgo->fGlobalBarID.push_back(id);
  fEvtMCBgo->fEnergy.push_back(e);
  fEvtMCBgo->fE0.push_back(0);  // TODO, to two sides?
  fEvtMCBgo->fE1.push_back(0);
  fEvtMCBgo->fPosition.push_back(pos);
}


