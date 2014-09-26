/*
 *  $Id: DmpSimRunManager.h, 2014-09-26 11:17:12 DAMPE $
 *  Author(s):
 *    Chi WANG (chiwang@mail.ustc.edu.cn) 12/04/2014
*/

#ifndef DmpSimRunManager_H
#define DmpSimRunManager_H

#include "G4RunManager.hh"

class DmpSimRunManager : public G4RunManager{
/*
 *  DmpSimRunManager
 *
 */
public:
  DmpSimRunManager();
  ~DmpSimRunManager();
  bool SimOneEvent(const int &id);

};

#endif

