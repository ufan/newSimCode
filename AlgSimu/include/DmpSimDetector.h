/* 
 *  $Id: DmpSimDetector.h, 2014-05-08 11:44:48 DAMPE $
 *  Author(s):
 *    Chi WANG (chiwang@mail.ustc.edu.cn) 26/02/2014
*/

#ifndef DmpSimDetector_H
#define DmpSimDetector_H

#include "G4VUserDetectorConstruction.hh"
#include "DmpEDetectorID.h"

#include "G4LogicalVolume.hh"

class G4GDMLParser;
//class DmpSimPsdSD;
//class DmpSimStkSD;
class DmpSimBgoSD;
//class DmpSimNudSD;

class DmpSimDetector : public G4VUserDetectorConstruction{
/*
 *
 *
 */
public:
  DmpSimDetector();
  ~DmpSimDetector();
  G4VPhysicalVolume* Construct();
  static void SetGdml(const std::string &argv) {fGdmlPath = argv;}
  void ConstructMaterials();
  void SetBeamTestOption(bool opt){fBTOption = opt;}
  void SetMagneticField(double v,double p){fMagneticFieldValue = v; fMagneticFieldPosZ = p;}
  void SetAuxDetOffset(double x,double y){fAuxOffsetX = x; fAuxOffsetY = y;}

private:
  static std::string       fGdmlPath;          // must set it in JobOpt file
  bool fBTOption;  //beam test simulation option
  double fAuxOffsetX;
  double fAuxOffsetY;
  double fMagneticFieldValue;
  double fMagneticFieldPosZ;
  G4GDMLParser      *fParser;
  G4GDMLParser      *fBTAuxParser[15];
  G4VPhysicalVolume *fPhyVolume;
  G4VPhysicalVolume *fBTAuxPhyVolume[15];
  G4VPhysicalVolume *fWorldPhyVolume;

private:
//  DmpSimPsdSD       *fPsdSD;
//  DmpSimStkSD       *fStkSD;
  DmpSimBgoSD       *fBgoSD;
//  DmpSimNudSD       *fNudSD;

};

#endif


