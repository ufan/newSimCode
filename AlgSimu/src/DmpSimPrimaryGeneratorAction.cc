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
#include "DmpMetadata.h"

DmpSimPrimaryGeneratorAction::DmpSimPrimaryGeneratorAction()
 :fPrimaryParticle(0),
  fGPS(0),
  fMetadata(0)
{
  fMetadata = dynamic_cast<DmpMetadata*>(gDataBuffer->ReadObject("Metadata/MCTruth/JobOpt"));
  double tmp[3]={0,0,0};
  std::istringstream iss_dir(fMetadata->Option["gps/direction"]);
  iss_dir>>tmp[0]>>tmp[1]>>tmp[2];
  fDirection.setX(tmp[0]);
  fDirection.setY(tmp[1]);
  fDirection.setZ(tmp[2]);
  std::istringstream iss_cen(fMetadata->Option["gps/centre"]);
  std::string unit="cm";
  iss_cen>>tmp[0]>>tmp[1]>>tmp[2]>>unit;
  fCentre.setX(tmp[0]);
  fCentre.setY(tmp[1]);
  fCentre.setZ(tmp[2]);
  if("cm" ==unit){
    fCentre *=10;
  }else if("m" == unit){
    fCentre *=100;
  }
  fPrimaryParticle = new DmpEvtMCPrimaryParticle();
  gDataBuffer->RegisterObject("Event/MCTruth/PrimaryParticle",fPrimaryParticle,"DmpEvtMCPrimaryParticle");
  fGPS = new G4GeneralParticleSource();
}

//-------------------------------------------------------------------
DmpSimPrimaryGeneratorAction::~DmpSimPrimaryGeneratorAction(){
  delete fGPS;
}

//-------------------------------------------------------------------
void DmpSimPrimaryGeneratorAction::ApplyGPSCommand(){
  for(std::map<std::string,std::string>::iterator it=fMetadata->Option.begin();it!=fMetadata->Option.end();++it){
std::cout<<"DEBUG: "<<__FILE__<<"("<<__LINE__<<")"<<it->first<<std::endl;
    if(it->first.find("BT/DAMPE") != std::string::npos){
      if(it->first.find("Rotation") != std::string::npos){
        double rad = 0;
        std::istringstream iss(it->second);
        iss>>rad;
        rad = rad / 180 * 3.141592653;
        AdjustmentRotation(rad);
      }else if(it->first.find("Translation") != std::string::npos){
        double tmp[3]={0,0,0};
        std::istringstream iss(it->second);
        iss>>tmp[0]>>tmp[1]>>tmp[2];
        AdjustmentTranslation(G4ThreeVector(tmp[0],tmp[1],tmp[2]));
      }
    }
  }
  G4UImanager *uiMgr = G4UImanager::GetUIpointer();
  for(std::map<std::string,std::string>::iterator it=fMetadata->Option.begin();it!=fMetadata->Option.end();++it){
    if(it->first.find("gps/") != std::string::npos){
      std::string cmd = "/" + it->first + " " + it->second;
      uiMgr->ApplyCommand(cmd);
    }
  }
}

//-------------------------------------------------------------------
void DmpSimPrimaryGeneratorAction::GeneratePrimaries(G4Event *anEvent){
  fGPS->GeneratePrimaryVertex(anEvent);
  // Set information of current event
  fPrimaryParticle->SetEventID(anEvent->GetEventID());
  fPrimaryParticle->SetTime(fGPS->GetParticleTime());
  fPrimaryParticle->SetPosition(fGPS->GetParticlePosition().x(),fGPS->GetParticlePosition().y(),fGPS->GetParticlePosition().z());
  std::cout<<"DEBUG: "<<__FILE__<<"("<<__LINE__<<")"<<fGPS->GetParticlePosition().x()<<std::endl;
  std::cout<<"DEBUG: "<<__FILE__<<"("<<__LINE__<<")"<<fGPS->GetParticlePosition().y()<<std::endl;
  std::cout<<"DEBUG: "<<__FILE__<<"("<<__LINE__<<")"<<fGPS->GetParticlePosition().z()<<std::endl;
  G4ThreeVector direction(fGPS->GetParticleMomentumDirection());
  std::cout<<"DEBUG: "<<__FILE__<<"("<<__LINE__<<")"<<direction.x()<<"\t"<<direction.y()<<"\t"<<direction.z()<<std::endl;
  fPrimaryParticle->SetDirection(direction.x(),direction.y(),direction.z());
  fPrimaryParticle->SetKineticEnergy(fGPS->GetParticleEnergy());
  G4ParticleDefinition *primaryParticle = fGPS->GetParticleDefinition();
  fPrimaryParticle->SetPDGCode(primaryParticle->GetPDGEncoding());
  fPrimaryParticle->SetMass(primaryParticle->GetPDGMass());
  fPrimaryParticle->SetCharge(primaryParticle->GetPDGCharge());
  fPrimaryParticle->SetComponent(primaryParticle->GetLeptonNumber(),primaryParticle->GetBaryonNumber());
  DmpLogDebug<<" Mass = "<<primaryParticle->GetPDGMass() << ", Kinetic=" << fGPS->GetParticleEnergy() << ", Charge=" << primaryParticle->GetPDGCharge() << std::endl;
}

//-------------------------------------------------------------------
void DmpSimPrimaryGeneratorAction::AdjustmentRotation(const double &rad){
  fDirection.rotateY(rad);
  std::ostringstream oss;
  oss<<fDirection.x()<<" "<<fDirection.y()<<" "<<fDirection.z();
  std::cout<<"DEBUG: "<<__FILE__<<"("<<__LINE__<<")"<<oss.str()<<std::endl;
  fMetadata->SetOption("gps/direction",oss.str());

  fCentre.rotateY(rad);
  std::ostringstream oss_1;
  oss_1<<fCentre.x()<<" "<<fCentre.y()<<" "<<fCentre.z()<<" mm";
  std::cout<<"DEBUG: "<<__FILE__<<"("<<__LINE__<<")"<<oss_1.str()<<std::endl;
  fMetadata->SetOption("gps/centre",oss_1.str());
}

//-------------------------------------------------------------------
void DmpSimPrimaryGeneratorAction::AdjustmentTranslation(const G4ThreeVector &v){
  fCentre -= v;
  std::ostringstream oss;
  oss<<fCentre.x()<<" "<<fCentre.y()<<" "<<fCentre.z()<<" mm";
  std::cout<<"DEBUG: "<<__FILE__<<"("<<__LINE__<<")"<<oss.str()<<std::endl;
  fMetadata->SetOption("gps/centre",oss.str());
}


