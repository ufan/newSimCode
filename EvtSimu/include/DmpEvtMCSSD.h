/*
 *  $Id: DmpEvtMCSSD.h, 2014-10-14 02:21:07 DAMPE $
 *  Author(s):
 *    Chi WANG (chiwang@mail.ustc.edu.cn) 14/10/2014
*/

#ifndef DmpEvtMCSSD_H
#define DmpEvtMCSSD_H

#include "TVector3.h"

class DmpEvtMCSSD : public TObject{
/*
 *  for simulation, Pb class calorimeter
 *
 */
public:
  DmpEvtMCSSD();
  ~DmpEvtMCSSD();
  void Reset();
  DmpEvtMCSSD &operator=(const DmpEvtMCSSD &r);
  void LoadFrom(DmpEvtMCSSD *r);

public:
  std::vector<TVector3>     fPosition;        // unit mm, position x,y,z

  ClassDef(DmpEvtMCSSD,1)
};

#endif


