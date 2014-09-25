/*
 *  $Id: DmpSimAlg.h, 2014-09-24 23:16:08 DAMPE $
 *  Author(s):
 *    Chi WANG (chiwang@mail.ustc.edu.cn) 10/06/2014
*/

#ifndef DmpSimAlg_H
#define DmpSimAlg_H

#include "DmpVAlg.h"

class DmpSimRunManager;
class G4PhysListFactory;

class DmpSimAlg : public DmpVAlg{
/*
 *  DmpSimAlg
 *
 *
 */
public:
  DmpSimAlg();
  ~DmpSimAlg();

  void Set(const std::string &type,const std::string &argv);
  bool Initialize();
  bool ProcessThisEvent();
  bool Finalize();

private:
  DmpSimRunManager      *fSimRunMgr;    // run manager of simulation
  G4PhysListFactory     *fPhyFactory;
  std::string           fMacFile;
  std::string           fPhyListName;   // default is QGSP_BIC
};

#endif
