/*
<<<<<<< HEAD
 *  $Id: Analysis.C, 2014-10-22 15:10:35 DAMPE $
=======
 *  $Id: Analysis.C, 2014-10-27 22:42:23 DAMPE $
>>>>>>> 0153ee5099094011a84601f0a78e4875b6d8280d
 *  Author(s):
 *    Chi WANG (chiwang@mail.ustc.edu.cn) 22/10/2014
 *
 *    Usage:
 *        $root -l Analysis.C
 *  root[0]LoadInputFile("aInputFile")
 *  root[1]VertexOfPhoton()
*/

TFile *f = 0;
TTree *eventTree = 0;
TTree *metaTree = 0;

void Analysis(){
  gSystem->Load("$DMPSWSYS/lib/libDmpEvent.so");
  gSystem->Load("$DMPSWWORK/lib/libDmpEvtSim.so");
}

void LoadInput(std::string files,std::string treeName){
  eventTree = new TChain(treeName.c_str());
  eventTree->AddFile(files.c_str());
}

void VertexOfPhoton(){
  TCanvas *c1 = new TCanvas("VertexOfPhoton","VertexOfPhoton");
  c1->Divide(2,2);
  c1->cd(1);
  eventTree->Draw("TrackVertex.fPosition.fZ","(TrackVertex.fPDGCode==22)&&(TrackVertex.fParentID == 1)");
  c1->cd(2);
  eventTree->Draw("TrackVertex.fDirection.Theta()","(TrackVertex.fPDGCode==22)&&(TrackVertex.fParentID == 1)");
  c1->cd(3);
  eventTree->Draw("TrackVertex.fEnergy:TrackVertex.fPosition.fZ","(TrackVertex.fPDGCode==22)&&(TrackVertex.fParentID == 1)");
  c1->cd(4);
  eventTree->Draw("TrackVertex.fDirection.Theta():TrackVertex.fEnergy","(TrackVertex.fPDGCode==22)&&(TrackVertex.fParentID == 1)");
}

