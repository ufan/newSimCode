/* 
 *  $Id: DmpSimDetector.h, 2014-09-30 00:15:26 DAMPE $
 *  Author(s):
 *    Chi WANG (chiwang@mail.ustc.edu.cn) 26/02/2014
 *    Yifeng Wei (weiyf@mail.ustc.edu.cn) 28/09/2014
*/

#ifndef DmpSimDetector_H
#define DmpSimDetector_H

#include <boost/filesystem.hpp>     // path
#include "G4VUserDetectorConstruction.hh"

class G4GDMLParser;
class G4LogicalVolume;
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
  static void SetGdml(const std::string &argv);
  /*
   * 4 types:
   *    Path/Filename.gdml  // read it
   *    sud-directory       // read $DMPSWSYS/share/Geometry/sub-directory/DAMPE.gdml
   *
   */

private:
  static boost::filesystem::path   fGdmlPath;        // if not set, will use the default gdml files
  G4GDMLParser          *fParser;
  G4VPhysicalVolume     *fPhyVolume;

private:
//  DmpSimPsdSD       *fPsdSD;
//  DmpSimStkSD       *fStkSD;
  DmpSimBgoSD       *fBgoSD;
//  DmpSimNudSD       *fNudSD;

// for beam test simulation
public:
  static void SetAuxDetOffset(const double &x,const double &y,const double &z){
    fAuxOffset[0] = x;
    fAuxOffset[1] = y;
    fAuxOffset[2] = z;
  }

private:
  void AdjustAuxiliaryDetectorOfTestBeam();

private:
  static double     fAuxOffset[3];          // unit: mm
  G4LogicalVolume   *fAuxiliaryDet_LV;      // logical volume of auxiliary detector for test beam

};

#endif


