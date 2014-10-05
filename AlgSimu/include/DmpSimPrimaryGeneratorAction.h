/*
 *  $Id: DmpSimPrimaryGeneratorAction.h, 2014-10-05 00:06:18 DAMPE $
 *  Author(s):
 *    Chi WANG (chiwang@mail.ustc.edu.cn) 03/03/2014
*/

#ifndef DmpSimPrimaryGeneratorAction_H
#define DmpSimPrimaryGeneratorAction_H

#include "G4VUserPrimaryGeneratorAction.hh"

class G4GeneralParticleSource;
class DmpEvtMCPrimaryParticle;

class DmpSimPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction{
public:
  DmpSimPrimaryGeneratorAction();    
  ~DmpSimPrimaryGeneratorAction();
  
  void ApplyGPSCommand();
  void GeneratePrimaries(G4Event*);

private:
  G4GeneralParticleSource   *fGPS;
  DmpEvtMCPrimaryParticle   *fPrimaryParticle;

private:
  double        fTranslation[3];   // source fixed, translate payload
  G4RotationMatrix  fRotation;     // source fixed, rotate payload
};

#endif

