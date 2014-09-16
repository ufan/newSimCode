/*
 *  $Id: DmpEvtMCBgo.cc, 2014-08-18 17:30:54 DAMPE/USTC $
 *  Author(s):
 *    Chi WANG (chiwang@mail.ustc.edu.cn) 16/12/2013
*/

#include "DmpEvtMCBgo.h"
#include "DmpLog.h"
#include "DmpBgoBase.h"

ClassImp(DmpEvtMCBgo)

//------------------------------------------------------------------------------
DmpEvtMCBgo::DmpEvtMCBgo(){
}

//------------------------------------------------------------------------------
DmpEvtMCBgo::~DmpEvtMCBgo(){
}

//-------------------------------------------------------------------
void DmpEvtMCBgo::AddG4Hit(const short &id,const double &e,const double &x,const double &y,const double &z){

    double totE = 0;
    bool NewBarFlag = true;

    for (int i=0;i<fGlobalBarID.size();i++){
        if (fGlobalBarID.at(i) == id){
            totE = e + fEnergy.at(i);
            fPosX.at(i) = (e*x + fEnergy.at(i)*fPosX.at(i))/totE;
            fPosY.at(i) = (e*y + fEnergy.at(i)*fPosY.at(i))/totE;
            fPosZ.at(i) = (e*z + fEnergy.at(i)*fPosZ.at(i))/totE;
            fEnergy.at(i) = totE;
            NewBarFlag = false;
        }
    }

    if (NewBarFlag == true){
        DmpLogDebug<<"\thit a new bar: "<<id<<DmpLogEndl;
        fGlobalBarID.push_back(id);
        fEnergy.push_back(e);
        fPosX.push_back(x);
        fPosY.push_back(y);
        fPosZ.push_back(z);
    }

}


//-------------------------------------------------------------------
const double DmpEvtMCBgo::GetPosition(short entry,const std::string &direction){

    if (direction == "X"){
        return fPosX.at(entry);
    }
    else if (direction == "Y"){
        return fPosY.at(entry);
    }
    else if (direction == "Z"){
        return fPosZ.at(entry);
    }
    else {
        DmpLogError << "Wrong direction type! Please choose 'X','Y',or 'Z' in GetPosition()." << DmpLogEndl;
        return -10000;
    }
}
//-------------------------------------------------------------------
const short DmpEvtMCBgo::GetLayerID(short entry){
    short layerID = DmpBgoBase::GetLayerID(fGlobalBarID.at(entry));
    return layerID;
}
//-------------------------------------------------------------------
const short DmpEvtMCBgo::GetBarID(short entry){
    short barID = DmpBgoBase::GetBarID(fGlobalBarID.at(entry));
    return barID;
}
//-------------------------------------------------------------------
void DmpEvtMCBgo::Clear(){

    fGlobalBarID.clear();
    fEnergy.clear();
    fPosX.clear();
    fPosY.clear();
    fPosZ.clear();

}
