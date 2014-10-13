/*
 *  $Id: DmpEvtMCPbGlass.cc, 2014-10-14 00:46:32 DAMPE $
 *  Author(s):
 *    Chi WANG (chiwang@mail.ustc.edu.cn) 14/10/2014
*/

#include "DmpEvtMCPbGlass.h"

ClassImp(DmpEvtMCPbGlass)

//-------------------------------------------------------------------
DmpEvtMCPbGlass::DmpEvtMCPbGlass():fTotalE(0){
}

//-------------------------------------------------------------------
DmpEvtMCPbGlass::~DmpEvtMCPbGlass(){
}

//-------------------------------------------------------------------
DmpEvtMCPbGlass& DmpEvtMCPbGlass::operator=(const DmpEvtMCPbGlass &r){
  Reset();
  fTotalE = r.fTotalE;
}

//-------------------------------------------------------------------
void DmpEvtMCPbGlass::LoadFrom(DmpEvtMCPbGlass *r){
  Reset();
  fTotalE = r->fTotalE;
}

//-------------------------------------------------------------------
void DmpEvtMCPbGlass::Reset(){
  fTotalE = 0;
}


