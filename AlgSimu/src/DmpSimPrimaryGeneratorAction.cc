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
  fGPS(0)
{
  fPrimaryParticle = new DmpEvtMCPrimaryParticle();
  if(not gDataBuffer->RegisterObject("Event/MCTruth/PrimaryParticle",fPrimaryParticle,"DmpEvtMCPrimaryParticle")){
    throw;
  }
  fGPS = new G4GeneralParticleSource();
  fTranslation[0] = 0.0;
  fTranslation[1] = 0.0;
  fTranslation[2] = 0.0;
}

//-------------------------------------------------------------------
DmpSimPrimaryGeneratorAction::~DmpSimPrimaryGeneratorAction(){
  delete fGPS;
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
  double degree = 0.0;
  std::istringstream iss_1(simMetadata->Option["BT/DAMPE/Rotation"]);
  iss_1>>degree;
  fRotation.rotateY(degree/180*3.141592653);
}

//-------------------------------------------------------------------
void DmpSimPrimaryGeneratorAction::GeneratePrimaries(G4Event *anEvent){
  fGPS->GeneratePrimaryVertex(anEvent);
  // Set information of current event
  fPrimaryParticle->SetEventID(anEvent->GetEventID());
  fPrimaryParticle->SetTime(fGPS->GetParticleTime());
  fPrimaryParticle->SetPosition(fGPS->GetParticlePosition().x()-fTranslation[0],fGPS->GetParticlePosition().y()-fTranslation[1],fGPS->GetParticlePosition().z()-fTranslation[3]);
  G4ThreeVector direction(fGPS->GetParticleMomentumDirection());
  //std::cout<<"DEBUG: "<<__FILE__<<"("<<__LINE__<<")"<<direction.x()<<"\t"<<direction.y()<<"\t"<<direction.z()<<std::endl;
  direction *= fRotation;
  fPrimaryParticle->SetDirection(direction.x(),direction.y(),direction.z());
  //std::cout<<"DEBUG: "<<__FILE__<<"("<<__LINE__<<")"<<direction.x()<<"\t"<<direction.y()<<"\t"<<direction.z()<<std::endl;
  fPrimaryParticle->SetKineticEnergy(fGPS->GetParticleEnergy());
  G4ParticleDefinition *primaryParticle = fGPS->GetParticleDefinition();
  fPrimaryParticle->SetPDGCode(primaryParticle->GetPDGEncoding());
  fPrimaryParticle->SetMass(primaryParticle->GetPDGMass());
  fPrimaryParticle->SetCharge(primaryParticle->GetPDGCharge());
  fPrimaryParticle->SetComponent(primaryParticle->GetLeptonNumber(),primaryParticle->GetBaryonNumber());
  DmpLogDebug<<" Mass = "<<primaryParticle->GetPDGMass() << ", Kinetic=" << fGPS->GetParticleEnergy() << ", Charge=" << primaryParticle->GetPDGCharge() << std::endl;
}

