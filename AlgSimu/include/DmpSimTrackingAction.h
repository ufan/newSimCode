/*
 *  $Id: DmpSimTrackingAction.h, 2014-10-21 20:36:58 DAMPE $
 *  Author(s):
 *    Chi WANG (chiwang@mail.ustc.edu.cn) 04/03/2014
*/

#ifndef  DmpSimTrackingAction_H
#define  DmpSimTrackingAction_H

#include "G4UserTrackingAction.hh"

class DmpEvtMCTrack;

class  DmpSimTrackingAction : public G4UserTrackingAction {
public:
  DmpSimTrackingAction();
  ~DmpSimTrackingAction(){};

  void PreUserTrackingAction(const G4Track*);

private:
  DmpEvtMCTrack     *fTrackInfor;   // information of all tracks
};

#endif

