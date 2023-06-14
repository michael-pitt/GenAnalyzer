#ifndef _minievent_h_
#define _minievent_h_

#include "TTree.h"

struct MiniEvent_t
{
  MiniEvent_t()
  {
    g_nw=0; 
  }

  static const int MAXWEIGHTS   =  102;

  Bool_t isData;
  UInt_t run,lumi,fill;
  ULong64_t event;
  Float_t beamXangle, instLumi, betaStar;
   
  //gen level event
  UInt_t g_nw;
  Float_t g_w[MAXWEIGHTS], g_wval[MAXWEIGHTS];

  //HL variables
  Float_t mtautau, ytautau, xiPos, xiNeg;


};

void createMiniEventTree(TTree *t,MiniEvent_t &ev);
void attachToMiniEventTree(TTree *t, MiniEvent_t &ev);

#endif
