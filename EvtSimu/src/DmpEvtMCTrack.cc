/*
 *  $Id: DmpEvtMCTrack.cc, 2014-10-20 22:23:41 DAMPE $
 *  Author(s):
 *    Chi WANG (chiwang@mail.ustc.edu.cn) 17/10/2014
*/

#include "DmpEvtMCTrack.h"

ClassImp(DmpEvtMCTrack)

//-------------------------------------------------------------------
DmpEvtMCTrack::DmpEvtMCTrack(){
}

//-------------------------------------------------------------------
DmpEvtMCTrack::~DmpEvtMCTrack(){
}

//-------------------------------------------------------------------
DmpEvtMCTrack& DmpEvtMCTrack::operator=(const DmpEvtMCTrack &r){
  Reset();
  fTrackID = r.fTrackID;
  fParentID = r.fParentID;
  fPDGCode = r.fPDGCode;
  fPosition = r.fPosition;
  fDirection = r.fDirection;
  fEnergy = r.fEnergy;
}

//-------------------------------------------------------------------
void DmpEvtMCTrack::LoadFrom(DmpEvtMCTrack *r){
  Reset();
  fTrackID = r->fTrackID;
  fParentID = r->fParentID;
  fPDGCode = r->fPDGCode;
  fPosition = r->fPosition;
  fDirection = r->fDirection;
  fEnergy = r->fEnergy;
}

//-------------------------------------------------------------------
void DmpEvtMCTrack::Reset(){
  fTrackID.clear();
  fParentID.clear();
  fPDGCode.clear();
  fPosition.clear();
  fDirection.clear();
  fEnergy.clear();
}


