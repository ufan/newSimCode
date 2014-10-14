/*
 *  $Id: DmpSimPbGlassSD.h, 2014-10-14 02:27:28 DAMPE $
 *  Author(s):
 *    Chi WANG (chiwang@mail.ustc.edu.cn) 14/10/2014
*/

#ifndef DmpSimPbGlassSD_H
#define DmpSimPbGlassSD_H

#include "G4VSensitiveDetector.hh"

class DmpEvtMCPbGlass;

class DmpSimPbGlassSD : public G4VSensitiveDetector{
/*
 *  DmpSimPbGlassSD
 *
 */
public:
  DmpSimPbGlassSD();
  ~DmpSimPbGlassSD();
  G4bool ProcessHits(G4Step*,G4TouchableHistory*);
  
  void Initialize(G4HCofThisEvent*);
  void EndOfEvent(G4HCofThisEvent*);

private:
  DmpEvtMCPbGlass     *fEvtMCPbGlass;

};

#endif

