/*
 *  $Id: DmpEvtMCSSD.cc, 2014-10-16 02:12:54 DAMPE $
 *  Author(s):
 *    Chi WANG (chiwang@mail.ustc.edu.cn) 14/10/2014
*/

#include "DmpEvtMCSSD.h"

ClassImp(DmpEvtMCSSD)

//-------------------------------------------------------------------
DmpEvtMCSSD::DmpEvtMCSSD(){
}

//-------------------------------------------------------------------
DmpEvtMCSSD::~DmpEvtMCSSD(){
}

//-------------------------------------------------------------------
DmpEvtMCSSD& DmpEvtMCSSD::operator=(const DmpEvtMCSSD &r){
  Reset();
  fPosition = r.fPosition;
  fDirection = r.fDirection;
  fKineticEnergy = r.fKineticEnergy;
}

//-------------------------------------------------------------------
void DmpEvtMCSSD::LoadFrom(DmpEvtMCSSD *r){
  Reset();
  fPosition = r->fPosition;
  fDirection = r->fDirection;
  fKineticEnergy = r->fKineticEnergy;
}

//-------------------------------------------------------------------
void DmpEvtMCSSD::Reset(){
  fPosition.clear();
  fDirection.clear();
  fKineticEnergy.clear();
}


