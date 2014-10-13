/*
 *  $Id: DmpSimPbGlassSD.cc, 2014-10-14 01:00:55 DAMPE $
 *  Author(s):
 *    Chi WANG (chiwang@mail.ustc.edu.cn) 14/10/2014
*/

#include "G4Step.hh"
#include "G4TouchableHistory.hh"

#include "DmpSimPbGlassSD.h"
#include "DmpEvtMCPbGlass.h"
#include "DmpDataBuffer.h"

//-------------------------------------------------------------------
DmpSimPbGlassSD::DmpSimPbGlassSD()
 :G4VSensitiveDetector("PbGlassSD"),
  fEvtMCPbGlass(0)
{
  fEvtMCPbGlass = new DmpEvtMCPbGlass();
  gDataBuffer->RegisterObject("Event/MCTruth/PbGlass",fEvtMCPbGlass,"DmpEvtMCPbGlass");
}

//-------------------------------------------------------------------
DmpSimPbGlassSD::~DmpSimPbGlassSD(){
}

//-------------------------------------------------------------------
#include <boost/lexical_cast.hpp>
G4bool DmpSimPbGlassSD::ProcessHits(G4Step *aStep,G4TouchableHistory*){
  fEvtMCPbGlass->fTotalE += aStep->GetTotalEnergyDeposit()/MeV;
  return true;
}

//-------------------------------------------------------------------
void DmpSimPbGlassSD::Initialize(G4HCofThisEvent*){
  fEvtMCPbGlass->Reset();
}

//-------------------------------------------------------------------
void DmpSimPbGlassSD::EndOfEvent(G4HCofThisEvent* HCE){
}


