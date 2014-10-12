/*
 *  $Id: DmpEvtMCPrimaryParticle.h, 2014-10-12 00:53:42 DAMPE $
 *  Author(s):
 *    Chi WANG (chiwang@mail.ustc.edu.cn) 25/02/2014
*/

#ifndef DmpEvtMCPrimaryParticle_H
#define DmpEvtMCPrimaryParticle_H

#include "TVector3.h"

class DmpEvtMCPrimaryParticle : public TObject{
/*
 * DmpEvtMCPrimaryParticle
 *
 * This class used (in Simulation DataManager) to store informations of user defined PrimaryGeneratorAction
 *
 */
public:
  DmpEvtMCPrimaryParticle():fEventID(0),fTime(0),fKineticE(0),fPDGCode(0),fMass(0),fCharge(0),fLeptonNo(0),fBaryonNo(0){}
  ~DmpEvtMCPrimaryParticle(){}
  void SetEventID(const long &i) {fEventID = i;}
  void SetTime(const double &t) {fTime = t;}
  void SetPosition(const double &x,const double &y,const double &z) {fPosition.SetXYZ(x,y,z);}
  void SetDirection(const double &x,const double &y,const double &z) {fDirection.SetXYZ(x,y,z);}
  void SetKineticEnergy(const double &e) {fKineticE = e;}
  void SetPDGCode(const int &c) {fPDGCode = c;}
  void SetMass(const double &m) {fMass = m;}
  void SetCharge(const double &c) {fCharge = c;}
  void SetComponent(const int &lep, const int &bar) {fLeptonNo = lep; fBaryonNo = bar;}

  const long& EventID() const {return fEventID;}
  const double& Time() const {return fTime;}
  const TVector3& Position() const {return fPosition;}
  const TVector3& Direction() const {return fDirection;}
  const double& KineticEnergy() const {return fKineticE;}
  const int& PDGcode() const {return fPDGCode;}
  const double& Mass() const {return fMass;}
  const double& Charge() const {return fCharge;}
  const int& LeptonNumber() const {return fLeptonNo;}
  const int& BaryonNumber() const {return fBaryonNo;}

private:
  long      fEventID;       // event ID
  double    fTime;          //! nanosecond, always = 0
  TVector3  fPosition;      // source position. mm
  TVector3  fDirection;     // momentum direction
  double    fKineticE;      // kinetic energy. MeV
  int       fPDGCode;       // PDG code
  double    fMass;          // mass. MeV
  double    fCharge;        // charge
  int       fLeptonNo;      // lepton number
  int       fBaryonNo;      // baryon number

  ClassDef(DmpEvtMCPrimaryParticle,1)
};

#endif


