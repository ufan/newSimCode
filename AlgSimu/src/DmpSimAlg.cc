/*
 *  $Id: DmpSimAlg.cc, 2014-06-10 16:04:34 DAMPE $
 *  Author(s):
 *    Chi WANG (chiwang@mail.ustc.edu.cn) 10/06/2014
*/

#include "DmpSimAlg.h"
#include "DmpSimRunManager.h"
#include "G4PhysListFactory.hh"
#include "DmpSimDetector.h"
#include "DmpSimPrimaryGeneratorAction.h"
#include "DmpSimTrackingAction.h"
#include "DmpCore.h"
#include "G4UImanager.hh"
#ifdef G4UI_USE_QT
#include "G4UIExecutive.hh"
#endif
#ifdef G4VIS_USE_OPENGLQT
#include "G4VisExecutive.hh"
#endif

//-------------------------------------------------------------------
DmpSimAlg::DmpSimAlg()
 :DmpVAlg("Sim/Alg/RunManager"),
  fSimRunMgr(0),
  fPhyFactory(0),
  fMacFile("VIS"),
  fPhyListName("QGSP_BIC"),
  fBeamTestOption("OFF"),
  fAuxOffsetX(0),
  fAuxOffsetY(0),
  fMagneticFieldValue(0),
  fMagneticFieldPosZ(-5000)
  //fEventID(0)
{
  fPhyFactory = new G4PhysListFactory();
  OptMap.insert(std::make_pair("Physics",0));
  OptMap.insert(std::make_pair("Gdml",1));
  OptMap.insert(std::make_pair("Nud/DeltaTime",2));
  OptMap.insert(std::make_pair("MacFile",3));
  //These option below is for beam test
  OptMap.insert(std::make_pair("BeamTestOption",4));
  OptMap.insert(std::make_pair("AuxOffsetX",5));
  OptMap.insert(std::make_pair("AuxOffsetY",6));
  OptMap.insert(std::make_pair("MagneticFieldValue",7));
  OptMap.insert(std::make_pair("MagneticFieldPosZ",8));
}

//-------------------------------------------------------------------
DmpSimAlg::~DmpSimAlg(){
  delete fPhyFactory;
  //delete fSimRunMgr;
}

//-------------------------------------------------------------------
#include <boost/lexical_cast.hpp>
//#include "DmpEvtMCNudBlock.h"
void DmpSimAlg::Set(const std::string &type,const std::string &argv){
  if(OptMap.find(type) == OptMap.end()){
    DmpLogError<<" No argument type "<<type<<DmpLogEndl;
  }
  switch (OptMap[type]){
    case 0:
    {// Physics
      fPhyListName = argv;
      break;
    }
    case 1:
    {// Gdml
      DmpSimDetector::SetGdml(argv);
      break;
    }
    case 2:
    {// Nud/DeltaTime
      //DmpEvtMCNudBlock::SetDeltaTime(boost::lexical_cast<short>(argv));
      break;
    }
    case 3:
    {// MacFile
      fMacFile = argv;
      break;
    }
    case 4:
    {//beam test option, if it is set "OFF", case 5~8 will not be used in simulation
      fBeamTestOption = argv;
      break;
    }
    case 5:
    {//Auxiliary detector offset X
      fAuxOffsetX = atof(argv.c_str());
      break;
    }
    case 6:
    {//Auxiliary detector offset Y
      fAuxOffsetY = atof(argv.c_str());
      break;
    }
    case 7:
    {//Magnetic field value
      fMagneticFieldValue = atof(argv.c_str());
      break;
    }
    case 8:
    {//Magnetic field position z
      fMagneticFieldPosZ = atof(argv.c_str());
      break;
    }
 
  }
}

//-------------------------------------------------------------------
#include <stdlib.h>     // getenv()
#include <time.h>
#include "CLHEP/Random/Random.h"
bool DmpSimAlg::Initialize(){
// set random seed
  G4long seed = time((time_t*)NULL);
  CLHEP::HepRandom::setTheSeed(seed);
  DmpLogInfo<<"[seed] "<<seed<<DmpLogEndl;
// set G4 kernel
  fSimRunMgr = new DmpSimRunManager();
  fSimRunMgr->SetUserInitialization(fPhyFactory->GetReferencePhysList(fPhyListName));
  fSimRunMgr->SetUserAction(new DmpSimPrimaryGeneratorAction());      // only Primary Generator is mandatory
  DmpSimDetector *DmpDetectorConstruction = new DmpSimDetector();
  if (fBeamTestOption == "ON"){
    DmpDetectorConstruction->SetBeamTestOption(true);
    DmpDetectorConstruction->SetMagneticField(fMagneticFieldValue,fMagneticFieldPosZ);
    DmpDetectorConstruction->SetAuxDetOffset(fAuxOffsetX,fAuxOffsetY);
  }
  else if (fBeamTestOption == "OFF"){
    DmpDetectorConstruction->SetBeamTestOption(false);
  }
  else {
    DmpLogError << "DmpSimDetector::Wrong beam test option type!" << DmpLogEndl;
  }
  fSimRunMgr->SetUserInitialization(DmpDetectorConstruction);
  fSimRunMgr->SetUserAction(new DmpSimTrackingAction());
  fSimRunMgr->Initialize();
// boot simulation
  if("VIS" == fMacFile){    // vis mode
    G4UImanager *uiMgr = G4UImanager::GetUIpointer();
#ifdef G4UI_USE_QT
    char *dummyargv[20]={"visual"};
    G4UIExecutive *ui = new G4UIExecutive(1,dummyargv);
#ifdef G4VIS_USE_OPENGLQT
    G4VisExecutive *vis = new G4VisExecutive();
    vis->Initialize();
    //G4String prefix = (G4String)getenv("DMPSWSYS")+"/share/Simulation/";
    G4String prefix = "./";
    uiMgr->ApplyCommand("/control/execute "+prefix+"DmpSimVis.mac");
#endif
    if (ui->IsGUI()){
      uiMgr->ApplyCommand("/control/execute "+prefix+"DmpSimGUI.mac");
    }
    ui->SessionStart();
    delete ui;
#endif
  }else{    // batch mode
    // if not vis mode, do some prepare for this run. refer to G4RunManagr::BeamOn()
    if(fSimRunMgr->ConfirmBeamOnCondition()){
      fSimRunMgr->ConstructScoringWorlds();
      fSimRunMgr->RunInitialization();
      // *
      // *  TODO:  check G4RunManager::InitializeEventLoop(the third argument right?)
      // *
      fSimRunMgr->InitializeEventLoop(gCore->GetMaxEventNumber(),fMacFile.c_str(),gCore->GetMaxEventNumber());
    }else{
      DmpLogError<<"G4RunManager::Initialize() failed"<<DmpLogEndl;
      return false;
    }
  }
  return true;
}

//-------------------------------------------------------------------
bool DmpSimAlg::ProcessThisEvent(){
  if("VIS" == fMacFile){
    return true;
  }
  
  if(fSimRunMgr->SimOneEvent(gCore->GetCurrentEventID())){
    //++fEventID;
    return true;
  }
  
  return false;
}

//-------------------------------------------------------------------
bool DmpSimAlg::Finalize(){

  if("VIS" != fMacFile){
    fSimRunMgr->TerminateEventLoop();
    fSimRunMgr->RunTermination();
  }
  return true;
}
