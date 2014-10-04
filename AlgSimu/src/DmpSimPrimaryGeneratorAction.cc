/*
 *  $Id: DmpSimPrimaryGeneratorAction.cc, 2014-10-05 00:06:15 DAMPE $
 *  Author(s):
 *    Chi WANG (chiwang@mail.ustc.edu.cn) 03/03/2014
*/

#include "G4UImanager.hh"
#include "G4GeneralParticleSource.hh"

#include "DmpSimPrimaryGeneratorAction.h"
#include "DmpEvtMCPrimaryParticle.h"
#include "DmpDataBuffer.h"
#include "DmpCore.h"
#include "DmpAlgorithmManager.h"
#include "DmpSimAlg.h"
#include "DmpMetadata.h"

DmpSimPrimaryGeneratorAction::DmpSimPrimaryGeneratorAction()
 :fPrimaryParticle(0),
  fGPS(0),
  fRotation(0)
{
  fPrimaryParticle = new DmpEvtMCPrimaryParticle();
  if(not gDataBuffer->RegisterObject("Event/MCTruth/PrimaryParticle",fPrimaryParticle,"DmpEvtMCPrimaryParticle")){
    throw;
  }
  fGPS = new G4GeneralParticleSource();
  fTranslation[0] = 0.0;
  fTranslation[1] = 0.0;
  fTranslation[2] = 0.0;
  fRotation = new G4RotationMatrix;
}

//-------------------------------------------------------------------
DmpSimPrimaryGeneratorAction::~DmpSimPrimaryGeneratorAction(){
  delete fGPS;
  delete fRotation;
}

//-------------------------------------------------------------------
void DmpSimPrimaryGeneratorAction::ApplyGPSCommand(){
  G4UImanager *uiMgr = G4UImanager::GetUIpointer();
  DmpMetadata *simMetadata = ((DmpSimAlg*)gCore->AlgorithmManager()->Get("Sim/BootAlg"))->GetMetadata();
  for(std::map<std::string,std::string>::iterator it=simMetadata->Option.begin();it!=simMetadata->Option.end();++it){
    if(it->first.find("gps/") != std::string::npos){
      std::string cmd = "/" + it->first + " " + it->second;
      uiMgr->ApplyCommand(cmd);
    }
  }
  //-------------------------------------------------------------------
  std::istringstream iss(simMetadata->Option["BT/DAMPE/Translation"]);
  iss>>fTranslation[0]>>fTranslation[1]>>fTranslation[2];
//-------------------------------------------------------------------
  std::istringstream iss_1(simMetadata->Option["BT/DAMPE/Rotation"]);
// *
// *  TODO:  update fRotation
// *
}

//-------------------------------------------------------------------
void DmpSimPrimaryGeneratorAction::GeneratePrimaries(G4Event *anEvent){
  fGPS->GeneratePrimaryVertex(anEvent);
  // Set information of current event
  fPrimaryParticle->SetEventID(anEvent->GetEventID());
  fPrimaryParticle->SetTime(fGPS->GetParticleTime());
  fPrimaryParticle->SetPosition(fGPS->GetParticlePosition().x()-fTranslation[0],fGPS->GetParticlePosition().y()-fTranslation[1],fGPS->GetParticlePosition().z()-fTranslation[3]);
// *
// *  TODO: update direction by fRotation
// *
  fPrimaryParticle->SetDirection(fGPS->GetParticleMomentumDirection().x(),fGPS->GetParticleMomentumDirection().y(),fGPS->GetParticleMomentumDirection().z());
  fPrimaryParticle->SetKineticEnergy(fGPS->GetParticleEnergy());
  G4ParticleDefinition *primaryParticle = fGPS->GetParticleDefinition();
  fPrimaryParticle->SetPDGCode(primaryParticle->GetPDGEncoding());
  fPrimaryParticle->SetMass(primaryParticle->GetPDGMass());
  fPrimaryParticle->SetCharge(primaryParticle->GetPDGCharge());
  fPrimaryParticle->SetComponent(primaryParticle->GetLeptonNumber(),primaryParticle->GetBaryonNumber());
  
  //debug
  std::cout << "This is GPS debug message: Particle mass=" << primaryParticle->GetPDGMass() << ", Kinetic=" << fGPS->GetParticleEnergy() << ", Charge=" << primaryParticle->GetPDGCharge() << std::endl;
}

