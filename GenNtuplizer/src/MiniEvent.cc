#include "GenAnalyzer/GenNtuplizer/interface/MiniEvent.h"
#include <iostream>

//
void createMiniEventTree(TTree *t,MiniEvent_t &ev)
{
 
  //event header
  t->Branch("isData",      &ev.isData,   "isData/O");
  t->Branch("run",         &ev.run,      "run/i");
  t->Branch("event",       &ev.event,    "event/l");
  t->Branch("lumi",        &ev.lumi,     "lumi/i");

  // MC truth info:
  //generator level information
  t->Branch("weight",      &ev.weight,     "weight/F");

  //HL variables
  t->Branch("ntrk",        &ev.ntrk,     "ntrk/i");

  t->Branch("m", &ev.m, "m/F");
  t->Branch("y", &ev.y, "y/F");
  t->Branch("xiPos",   &ev.xiPos,   "xiPos/F");
  t->Branch("xiNeg",   &ev.xiNeg,   "xiNeg/F");

  t->Branch("lep1_pt",   &ev.lep1_pt,   "lep1_pt/F");
  t->Branch("lep2_pt",   &ev.lep2_pt,   "lep2_pt/F");
  t->Branch("lep1_eta",   &ev.lep1_eta,   "lep1_eta/F");
  t->Branch("lep2_eta",   &ev.lep2_eta,   "lep2_eta/F");

}

//
void attachToMiniEventTree(TTree *t,MiniEvent_t &ev)
{

  //event header
  
  t->SetBranchAddress("isData",    &ev.isData);
  t->SetBranchAddress("run",       &ev.run);
  t->SetBranchAddress("event",     &ev.event);
  t->SetBranchAddress("lumi",      &ev.lumi);

  
  //generator level
  t->SetBranchAddress("weight",      &ev.weight);

  //HL variables
  t->SetBranchAddress("ntrk",      &ev.ntrk);

  t->SetBranchAddress("m",      &ev.m);
  t->SetBranchAddress("y",      &ev.y);
  t->SetBranchAddress("xiPos",        &ev.xiPos);
  t->SetBranchAddress("xiNeg",        &ev.xiNeg);
  
  t->SetBranchAddress("lep1_pt",        &ev.lep1_pt);
  t->SetBranchAddress("lep2_pt",        &ev.lep2_pt);
  t->SetBranchAddress("lep1_eta",        &ev.lep1_eta);
  t->SetBranchAddress("lep2_eta",        &ev.lep2_eta);
  
}
