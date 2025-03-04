#ifndef _minievent_h_
#define _minievent_h_

#include "TTree.h"

struct MiniEvent_t
{
  MiniEvent_t()
  {
    ntrk=0; 
  }

  static const int MAXWEIGHTS   =  203;

  Bool_t isData;
  UInt_t run,lumi;
  ULong64_t event;

   
  //gen level event
  Float_t weight;

  //HL variables
  UInt_t ntrk;
  Float_t m, y, xiPos, xiNeg;
  Float_t lep1_pt, lep2_pt, lep1_eta, lep2_eta;
  Float_t Acopl;


};

void createMiniEventTree(TTree *t,MiniEvent_t &ev);
void attachToMiniEventTree(TTree *t, MiniEvent_t &ev);

#endif
