/*
 *  $Id: DmpSimSSD_SD.h, 2014-10-14 02:23:51 DAMPE $
 *  Author(s):
 *    Chi WANG (chiwang@mail.ustc.edu.cn) 14/10/2014
*/

#ifndef DmpSimSSD_SD_H
#define DmpSimSSD_SD_H

#include "G4VSensitiveDetector.hh"

class DmpEvtMCSSD;

class DmpSimSSD_SD : public G4VSensitiveDetector{
/*
 *  DmpSimSSD_SD
 *
 */
public:
  DmpSimSSD_SD();
  ~DmpSimSSD_SD();
  G4bool ProcessHits(G4Step*,G4TouchableHistory*);
  
  void Initialize(G4HCofThisEvent*);
  void EndOfEvent(G4HCofThisEvent*);

private:
  DmpEvtMCSSD     *fEvtMCSSD;

};

#endif

