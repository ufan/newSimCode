/*
 *  $Id: DmpSimSSD_SD.cc, 2014-10-14 02:23:57 DAMPE $
 *  Author(s):
 *    Chi WANG (chiwang@mail.ustc.edu.cn) 14/10/2014
*/

#include "G4Step.hh"
#include "G4TouchableHistory.hh"

#include "DmpSimSSD_SD.h"
#include "DmpEvtMCSSD.h"
#include "DmpDataBuffer.h"

//-------------------------------------------------------------------
DmpSimSSD_SD::DmpSimSSD_SD()
 :G4VSensitiveDetector("SSD_SD"),
  fEvtMCSSD(0)
{
  fEvtMCSSD = new DmpEvtMCSSD();
  gDataBuffer->RegisterObject("Event/MCTruth/SSD",fEvtMCSSD,"DmpEvtMCSSD");
}

//-------------------------------------------------------------------
DmpSimSSD_SD::~DmpSimSSD_SD(){
}

//-------------------------------------------------------------------
#include <boost/lexical_cast.hpp>
G4bool DmpSimSSD_SD::ProcessHits(G4Step *aStep,G4TouchableHistory*){
  G4ThreeVector pos = aStep->GetPreStepPoint()->GetPosition();
  fEvtMCSSD->fPosition.push_back(TVector3(pos.x(),pos.y(),pos.z()));
  return true;
}

//-------------------------------------------------------------------
void DmpSimSSD_SD::Initialize(G4HCofThisEvent*){
  fEvtMCSSD->Reset();
}

//-------------------------------------------------------------------
void DmpSimSSD_SD::EndOfEvent(G4HCofThisEvent* HCE){
}


