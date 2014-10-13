/*
 *  $Id: DmpEvtMCPbGlass.h, 2014-10-14 00:40:41 DAMPE $
 *  Author(s):
 *    Chi WANG (chiwang@mail.ustc.edu.cn) 14/10/2014
*/

#ifndef DmpEvtMCPbGlass_H
#define DmpEvtMCPbGlass_H

#include "TObject.h"

class DmpEvtMCPbGlass : public TObject{
/*
 *  for simulation, Pb class calorimeter
 *
 */
public:
  DmpEvtMCPbGlass();
  ~DmpEvtMCPbGlass();
  void Reset();
  DmpEvtMCPbGlass &operator=(const DmpEvtMCPbGlass &r);
  void LoadFrom(DmpEvtMCPbGlass *r);

public:
  double    fTotalE;        // unit MeV

  ClassDef(DmpEvtMCPbGlass,1)
};

#endif


