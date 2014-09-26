/*
 *  $Id: DmpEvtMCBgo.cc, 2014-09-24 14:31:28 DAMPE $
 *  Author(s):
 *    Chi WANG (chiwang@mail.ustc.edu.cn) 16/12/2013
*/

#include "DmpEvtMCBgo.h"

ClassImp(DmpEvtMCBgo)

//------------------------------------------------------------------------------
DmpEvtMCBgo::DmpEvtMCBgo(){
}

//------------------------------------------------------------------------------
DmpEvtMCBgo::~DmpEvtMCBgo(){
}

//-------------------------------------------------------------------
void DmpEvtMCBgo::AddG4Hit(const short &id,const double &e,const double &x,const double &y,const double &z){
  for(size_t i=0;i<fGlobalBarID.size();i++){
    if(fGlobalBarID.at(i) == id){
      double totE = e + fEnergy.at(i);
      fPosX.at(i) = (e*x + fEnergy.at(i)*fPosX.at(i))/totE;
      fPosY.at(i) = (e*y + fEnergy.at(i)*fPosY.at(i))/totE;
      fPosZ.at(i) = (e*z + fEnergy.at(i)*fPosZ.at(i))/totE;
      fEnergy.at(i) = totE;
      return;   // if found gid, update and return
    }
  }
  // if not, creat a new one
  fGlobalBarID.push_back(id);
  fEnergy.push_back(e);
  fPosX.push_back(x);
  fPosY.push_back(y);
  fPosZ.push_back(z);
}

//-------------------------------------------------------------------
void DmpEvtMCBgo::Reset(){
  fGlobalBarID.clear();
  fEnergy.clear();
  fPosX.clear();
  fPosY.clear();
  fPosZ.clear();
}


