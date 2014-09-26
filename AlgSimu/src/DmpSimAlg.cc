/*
 *  $Id: DmpSimAlg.cc, 2014-09-25 22:53:04 DAMPE $
 *  Author(s):
 *    Chi WANG (chiwang@mail.ustc.edu.cn) 10/06/2014
*/

#include <time.h>   // time_t

#include "DmpSimAlg.h"
#include "DmpSimRunManager.h"
#include "G4PhysListFactory.hh"
#include "DmpSimDetector.h"
#include "DmpSimPrimaryGeneratorAction.h"
#include "DmpSimTrackingAction.h"
#include "DmpCore.h"
#include "DmpRootIOSvc.h"
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
  fBatchMode(true),
  fPhyListName("QGSP_BIC"),
  fSeed(time((time_t*)NULL))
{
  fPhyFactory = new G4PhysListFactory();
  OptMap.insert(std::make_pair("Physics",0));
  OptMap.insert(std::make_pair("Gdml",1));
  OptMap.insert(std::make_pair("Nud/DeltaTime",2));
  OptMap.insert(std::make_pair("Seed",3));
  OptMap.insert(std::make_pair("BT/AuxOffsetX",4));
  OptMap.insert(std::make_pair("BT/AuxOffsetY",5));
  OptMap.insert(std::make_pair("BT/MagneticFieldValue",6));
  OptMap.insert(std::make_pair("BT/MagneticFieldPosZ",7));
  // mode check
  if(".mac" != gRootIOSvc->GetInputExtension()){
    fBatchMode = false; // then will active visualization mode
    if(gRootIOSvc->GetOutputStem() == ""){
      gRootIOSvc->Set("Output/FileName","DmpSimVis.root");
    }
  }
  gRootIOSvc->Set("Output/Key","sim");
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
    DmpLogError<<"[DmpSimAlg::Set] No argument type:\t"<<type<<DmpLogEndl;
    std::cout<<"\tPossible options are:"<<DmpLogEndl;
    for(std::map<std::string,short>::iterator anOpt= OptMap.begin();anOpt!=OptMap.end();anOpt++){
      std::cout<<"\t\t"<<anOpt->first<<DmpLogEndl;
    }
    throw;
  }
  switch (OptMap[type]){
    case 0: // Physics
    {
      fPhyListName = argv;
      break;
    }
    case 1: // Gdml
    {
      DmpSimDetector::SetGdml(argv);
      break;
    }
    case 2: // Nud/DeltaTime
    {
      //DmpEvtMCNudBlock::SetDeltaTime(boost::lexical_cast<short>(argv));
      break;
    }
    case 3: // Seed
    {
      fSeed = boost::lexical_cast<long>(argv);
      break;
    }
    case 4: // Auxiliary detector offset X
    {
      DmpSimDetector::SetAuxDetOffsetX(boost::lexical_cast<double>(argv));
      break;
    }
    case 5: // Auxiliary detector offset Y
    {
      DmpSimDetector::SetAuxDetOffsetY(boost::lexical_cast<double>(argv));
      break;
    }
    case 6: // Magnetic field value
    {
      DmpSimDetector::SetMagneticFieldValue(boost::lexical_cast<double>(argv));
      break;
    }
    case 7: // Magnetic field position z
    {
      DmpSimDetector::SetMagneticFieldPosition(boost::lexical_cast<double>(argv));
      break;
    }
  }
}

//-------------------------------------------------------------------
#include <stdlib.h>     // getenv()
bool DmpSimAlg::Initialize(){
// set seed
  std::cout<<"\tRandom seed: "<<fSeed<<DmpLogEndl;      // keep this information in any case
  CLHEP::HepRandom::setTheSeed(fSeed);
// set G4 kernel
  fSimRunMgr = new DmpSimRunManager();
  fSimRunMgr->SetUserInitialization(fPhyFactory->GetReferencePhysList(fPhyListName));
  fSimRunMgr->SetUserAction(new DmpSimPrimaryGeneratorAction());      // only Primary Generator is mandatory
  fSimRunMgr->SetUserInitialization(new DmpSimDetector());
  fSimRunMgr->SetUserAction(new DmpSimTrackingAction());
  fSimRunMgr->Initialize();
// boot simulation
  G4UImanager *uiMgr = G4UImanager::GetUIpointer();
  if(fBatchMode){    // batch mode
    uiMgr->ApplyCommand("/control/execute "+gRootIOSvc->GetInputFileName());
    if(fSimRunMgr->ConfirmBeamOnCondition()){   // if not vis mode, do some prepare for this run. refer to G4RunManagr::BeamOn()
      fSimRunMgr->SetNumberOfEventsToBeProcessed(gCore->GetMaxEventNumber());
      fSimRunMgr->ConstructScoringWorlds();
      fSimRunMgr->RunInitialization();
      fSimRunMgr->InitializeEventLoop(gCore->GetMaxEventNumber());
    }else{
      DmpLogError<<"G4RunManager::Initialize() failed"<<DmpLogEndl;
      return false;
    }
  }else{    // visualization mode
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
#ifdef G4VIS_USE_OPENGLQT
    delete vis;
#endif
    gRootIOSvc->FillData("Event");
    gCore->TerminateRun();  // just for check GDML, or run one event while debuging
#endif
  }
  return true;
}

//-------------------------------------------------------------------
bool DmpSimAlg::ProcessThisEvent(){
  if(fSimRunMgr->SimOneEvent(gCore->GetCurrentEventID())){
    return true;
  }
  return false;
}

//-------------------------------------------------------------------
bool DmpSimAlg::Finalize(){
  if(fBatchMode){
    fSimRunMgr->TerminateEventLoop();
    fSimRunMgr->RunTermination();
  }
  return true;
}

