/*
 *  $Id: DmpEvtMCTrack.h, 2014-10-21 20:33:48 DAMPE $
 *  Author(s):
 *    Chi WANG (chiwang@mail.ustc.edu.cn) 17/10/2014
*/

#ifndef DmpEvtMCTrack_H
#define DmpEvtMCTrack_H

#include "TVector3.h"

class DmpEvtMCTrack : public TObject{
/*
 *   This class used to record:
 *    1. vertex information of all secondray particle
 *          fPosition:  Where does the secondary particle generated?
 *          fDirection: the direction of the new secondary particle
 *          fEnergy:    kinetic energy of it
 *    2. information of a G4Step
 *          fPosition:  position of pre-step-point
 *          fDirection: momentum direction of pre-step-point
 *          fEnergy:    deposited energy of this step
 */
public:
  DmpEvtMCTrack();
  ~DmpEvtMCTrack();
  void Reset();
  DmpEvtMCTrack &operator=(const DmpEvtMCTrack &r);
  void LoadFrom(DmpEvtMCTrack *r);

public:
  std::vector<int>          fTrackID;           // track ID
  std::vector<int>          fParentID;          // ID of Parent track
  std::vector<int>          fPDGCode;           // pdg code
  std::vector<TVector3>     fPosition;          // unit mm, position x,y,z
  std::vector<TVector3>     fDirection;         // momentum direction
  std::vector<double>       fEnergy;            // unit MeV

  ClassDef(DmpEvtMCTrack,1)
};

#endif


