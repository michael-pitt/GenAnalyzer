#include "GenAnalyzer/GenWeightAnal/interface/MiniEvent.h"
#include <iostream>

//
void createMiniEventTree(TTree *t,MiniEvent_t &ev)
{
 
  //event header
  t->Branch("isData",      &ev.isData,   "isData/O");
  t->Branch("run",         &ev.run,      "run/i");
  t->Branch("event",       &ev.event,    "event/l");
  t->Branch("betaStar",    &ev.betaStar,    "betaStar/F");
  t->Branch("fill",        &ev.fill,    "fill/i");
  t->Branch("lumi",        &ev.lumi,     "lumi/i");
  t->Branch("beamXangle",  &ev.beamXangle,   "beamXangle/F");
  t->Branch("instLumi",    &ev.instLumi,     "instLumi/F");

  // MC truth info:
  //generator level information
  t->Branch("g_nw",      &ev.g_nw,     "g_nw/i");
  t->Branch("g_w",        ev.g_w,      "g_w[g_nw]/F");

  //HL variables
  t->Branch("mtautau", &ev.mtautau, "mtautau/F");

}

//
void attachToMiniEventTree(TTree *t,MiniEvent_t &ev)
{

  //event header
  
  t->SetBranchAddress("isData",    &ev.isData);
  t->SetBranchAddress("run",       &ev.run);
  t->SetBranchAddress("event",     &ev.event);
  t->SetBranchAddress("fill",      &ev.fill);
  t->SetBranchAddress("lumi",      &ev.lumi);
  t->SetBranchAddress("betaStar",      &ev.betaStar);
  t->SetBranchAddress("beamXangle",  &ev.beamXangle);
  t->SetBranchAddress("instLumi",    &ev.instLumi);
  
  //generator level
  t->SetBranchAddress("g_nw",      &ev.g_nw);
  t->SetBranchAddress("g_w",       ev.g_w);

  //HL variables
  t->SetBranchAddress("mtautau",      &ev.mtautau);

  
}