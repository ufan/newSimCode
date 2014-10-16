/*
 *  $Id: DmpEvtMCSSD.h, 2014-10-16 02:12:56 DAMPE $
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
  std::vector<TVector3>     fPosition;          // unit mm, position x,y,z
  std::vector<TVector3>     fDirection;         // momentum direction
  std::vector<double>       fKineticEnergy;     // unit MeV, the kinetic engergy of current particle at current step

  ClassDef(DmpEvtMCSSD,1)
};

#endif


