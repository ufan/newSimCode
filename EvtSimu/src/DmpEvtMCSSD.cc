/*
 *  $Id: DmpEvtMCSSD.cc, 2014-10-14 00:46:32 DAMPE $
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
}

//-------------------------------------------------------------------
void DmpEvtMCSSD::LoadFrom(DmpEvtMCSSD *r){
  Reset();
  fPosition = r->fPosition;
}

//-------------------------------------------------------------------
void DmpEvtMCSSD::Reset(){
  fPosition.clear();
}


