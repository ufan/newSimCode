/*
 *  $Id: DmpSimBgoSD.cc, 2014-10-06 17:13:16 DAMPE $
 *  Author(s):
 *    Chi WANG (chiwang@mail.ustc.edu.cn) 03/03/2014
*/

#include "G4Step.hh"
#include "G4TouchableHistory.hh"

#include "DmpSimBgoSD.h"
#include "DmpEvtBgoHits.h"
#include "DmpDataBuffer.h"
#include "DmpBgoBase.h"
#include "DmpLog.h"

#include "TMath.h"

//-------------------------------------------------------------------
DmpSimBgoSD::DmpSimBgoSD()
 :G4VSensitiveDetector("BgoSD"),
  fEvtMCBgo(0)
{
  GetMipPar();
  GetAttPar();
  for (int i=0;i<616;i++){RanGaus[i] = new TRandom();}
  fEvtMCBgo = new DmpEvtBgoHits();
  gDataBuffer->RegisterObject("Event/MCTruth/Bgo",fEvtMCBgo,"DmpEvtBgoHits");
  fDigitBgo = new DmpEvtBgoHits();
  gDataBuffer->RegisterObject("Event/MCTruth/BgoFDigit",fDigitBgo,"DmpEvtBgoHits");
}

//-------------------------------------------------------------------
DmpSimBgoSD::~DmpSimBgoSD(){
}

//-------------------------------------------------------------------
#include <boost/lexical_cast.hpp>
G4bool DmpSimBgoSD::ProcessHits(G4Step *aStep,G4TouchableHistory*){
  G4TouchableHistory *theTouchable = (G4TouchableHistory*)(aStep->GetPreStepPoint()->GetTouchable());
  std::string barName = theTouchable->GetVolume(1)->GetName();
  barName.assign(barName.end()-4,barName.end());        // get ID
  short barID = boost::lexical_cast<short>(barName);
  barID = DmpBgoBase::ConstructGlobalBarID(barID/100,barID%100);
  if (aStep->GetTotalEnergyDeposit()>0){
    AddThisG4Hit(barID,aStep->GetTotalEnergyDeposit()/MeV,aStep->GetPreStepPoint()->GetPosition());
    Eny2ADC(barID,aStep->GetTotalEnergyDeposit()/MeV,(aStep->GetPreStepPoint()->GetPosition()).x()/mm,(aStep->GetPreStepPoint()->GetPosition()).y()/mm);
  }
  return true;
}

//-------------------------------------------------------------------
void DmpSimBgoSD::Initialize(G4HCofThisEvent*){
  fEvtMCBgo->Reset();
  fDigitBgo->Reset();
  Position.Clear();
  memset(TotalE,0,sizeof(TotalE));
}

//-------------------------------------------------------------------
void DmpSimBgoSD::EndOfEvent(G4HCofThisEvent* HCE){
  //sampling & save
  Sampling();
}

//-------------------------------------------------------------------
void DmpSimBgoSD::AddThisG4Hit(const short &id,const double &e,const G4ThreeVector &in){
  TVector3 pos(in.x(),in.y(),in.z());
  for(size_t i=0;i<fEvtMCBgo->fGlobalBarID.size();i++){
    if(fEvtMCBgo->fGlobalBarID.at(i) == id){
      double totE = e + fEvtMCBgo->fEnergy.at(i);
      fEvtMCBgo->fPosition.at(i) = fEvtMCBgo->fPosition.at(i) * (fEvtMCBgo->fEnergy.at(i) / totE);
      fEvtMCBgo->fPosition.at(i) += pos * (e / totE);
      fEvtMCBgo->fEnergy.at(i) = totE;
      return;   // if found gid, update and return
    }
  }
  // if not, creat a new one

  fEvtMCBgo->fGlobalBarID.push_back(id);
  fEvtMCBgo->fEnergy.push_back(e);
  fEvtMCBgo->fES0.push_back(0);  // TODO, to two sides?
  fEvtMCBgo->fES1.push_back(0);
  fEvtMCBgo->fPosition.push_back(pos);
}
//-------------------------------------------------------------------
void DmpSimBgoSD::GetAttPar(){

  //Get Attenuation coefficients 
  ifstream Apar;
  Apar.open("../CaliParameter/Attenuation/AttPar");
   if(!Apar.good()){
    std::cout<<"Can not open Att Par file!"<<std::endl;
    exit(1);
  } 
  int nGbar=14*22;
   for(int i=0; i<nGbar;i++){
     for(int j=0 ;j<2;j++){ 
      Apar>>AttPar[i][j]; 
      //std::cout<<AttPar[i][j]<<"\t";
    }
    //std::cout<<std::endl;
  }
  Apar.close();
}
//-------------------------------------------------------------------
void DmpSimBgoSD::GetMipPar(){

  //Get MIPs parameters
  ifstream Mpar;
  Mpar.open("../CaliParameter/MIPs/MIPsPar");
  if(!Mpar.good()){
    std::cout<<"Can not open MIPs Par file!"<<std::endl;
    exit(1);
  }  
  int nGpmt=14*2*22;
  for(int i=0;i<nGpmt;i++){
    for(int j=0;j<4;j++){
      Mpar>>MipPar[i][j];  
      //std::cout<<MipPar[i][j]<<"\t";
    }
    //std::cout<<std::endl;
  }
  Mpar.close();
}
//-------------------------------------------------------------------
void DmpSimBgoSD::Eny2ADC(const short &id, const double &e, const double &x,const double &y){
  //step Energy to ADC
  //Get iGpmt, iGbar
  short layer=DmpBgoBase::GetLayerID(id);
  short bar=DmpBgoBase::GetBarID(id);
  short iGbar=layer*22+bar;
  short iGpmt[2]={layer*2*22+bar,(layer*2+1)*22+bar};

  //Set Att coe, MIPs coversion ratio;
  double Dis[2]; //distance between energy deposition and BGO end.
  if(layer%2==0){
    Dis[1]=x+300;
    Dis[0]=300-x;
  }
  else{
    Dis[1]=y+300;
    Dis[0]=300-y;
  }
  double AttCoe[2];  //AttPar[][0]=2/lambda; lambda :cm Dis mm;
  AttCoe[0]=1/TMath::Exp(AttPar[iGbar][0]*Dis[0]/2/10);
  AttCoe[1]=1/TMath::Exp(AttPar[iGbar][0]*Dis[1]/2/10);
  double AttHit[2];
  AttHit[0]=e*AttCoe[0];
  AttHit[1]=e*AttCoe[1];
  TotalE[iGpmt[0]]+=AttHit[0];
  TotalE[iGpmt[1]]+=AttHit[1];
}
//-------------------------------------------------------------------
void DmpSimBgoSD::Sampling(){
  //Sampling with calibrated paramneters
  //Get iGpmt, iGbar
  for(short layer=0;layer<14;layer++){
    for(short bar=0;bar<22;bar++){
      short iGbar=layer*22+bar;
      short iGpmt[2]={layer*2*22+bar,(layer*2+1)*22+bar};
	  short gid_bar=DmpBgoBase::ConstructGlobalBarID(layer,bar);
      if(TotalE[iGpmt[0]]>0){
//	  std::cout<<iGpmt[0]<<std::endl;
//	  std::cout<<TotalE[iGpmt[0]]<<std::endl;
//	  std::cout<<iGpmt[1]<<std::endl;
//	  std::cout<<TotalE[iGpmt[1]]<<std::endl;
        double MipCov[2];
        MipCov[0]=MipPar[iGpmt[0]][1]*TMath::Exp(AttPar[iGbar][0]*30/2)/22.5;   //non-att normalized ADC counts/MeV;
        MipCov[1]=MipPar[iGpmt[1]][1]*TMath::Exp(AttPar[iGbar][0]*30/2)/22.5;   //non-att normalized ADC counts/MeV;
        double Mean[2]={TotalE[iGpmt[0]]*MipCov[0],TotalE[iGpmt[1]]*MipCov[1]};
        double Sigma[2]={MipPar[iGpmt[0]][3]*TMath::Sqrt(Mean[0]/MipPar[iGpmt[0]][1]),MipPar[iGpmt[1]][3]*TMath::Sqrt(Mean[1]/MipPar[iGpmt[1]][1])};
	  
        //  TRandom *s0=new TRandom();
        //  TRandom *s1=new TRandom();
	    fDigitBgo->fGlobalBarID.push_back(gid_bar);
	    double ES0=RanGaus[iGpmt[0]]->Gaus(Mean[0],Sigma[0]);
	    double ES1=RanGaus[iGpmt[1]]->Gaus(Mean[1],Sigma[1]);

        fDigitBgo->fES0.push_back(ES0);
        fDigitBgo->fES1.push_back(ES1);
        fDigitBgo->fEnergy.push_back(TMath::Sqrt(ES0*ES1));
	    double pos=(AttPar[iGbar][0]*TMath::Log(TotalE[iGpmt[0]]/TotalE[iGpmt[1]])+AttPar[iGbar][1])*10-300;//unit: mm
	    if(layer%2==0){
	      Position.SetX(pos);	  
	    }
	    else{
	      Position.SetY(pos);
	 
	    }
        fDigitBgo->fPosition.push_back(Position);
      }
    }
  }
}
