/*
 *  $Id: DmpEvtMCBgo.h, 2014-08-18 17:30:53 DAMPE/USTC $
 *  Author(s):
 *    Chi WANG (chiwang@mail.ustc.edu.cn) 16/12/2013
*/

#ifndef DmpEvtMCBgo_H
#define DmpEvtMCBgo_H

#include "TObject.h"

class DmpEvtMCBgo : public TObject{
/*
 *  DmpEvtMCBgo
 *  
 *  One DmpEvtMCBgo is a response of one Event.
*/
public:
  DmpEvtMCBgo();
  ~DmpEvtMCBgo();
  void  AddG4Hit(const short &id,const double &e,const double &x,const double &y,const double &z);     // invoke from G4Step or Sensitive Detector
  const short GetFiredBarNo() const {return fGlobalBarID.size();} 
  const short GetLayerID(short entry) const {return (fGlobalBarID.at(entry))/100;}
  const short GetBarID(short entry) const {return (fGlobalBarID.at(entry))%100;}   // in one layer
  const short GetGlobalBarID(short entry) const {return fGlobalBarID.at(entry);} // in whole layer
  const double GetEnergy(short entry) const  {return fEnergy.at(entry);}
  const double GetPosition(short entry,const std::string &direction);
  //std::vector <short>  GetGlobalBarIDCollection() {return fGlobalBarID;}

private:
  std::vector <short>     fGlobalBarID;   // unique sensitive detector(minimum detector unit) ID. Bgo bar ID. fGlobalBarID =  layerID*100 + barID
  std::vector <double>    fEnergy;        // unit MeV
  std::vector <double>    fPosX;          // unit mm, position x
  std::vector <double>    fPosY;          // unit mm, position y
  std::vector <double>    fPosZ;          // unit mm, position z

  ClassDef(DmpEvtMCBgo,1)
};

#endif

