// -*- C++ -*-
//
// Package:    GenAnalyzer/GenNtuplizer
// Class:      GenNtuplizer
//
/**\class GenNtuplizer GenNtuplizer.cc GenAnalyzer/GenNtuplizer/plugins/GenNtuplizer.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Michael Pitt
//         Created:  Tue, 04 Mar 2025 12:39:34 GMT
//
//


// system include files
#include <memory>
#include <iostream>
#include <string>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/Run.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/LuminosityBlock.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "GenAnalyzer/GenNtuplizer/interface/MiniEvent.h"

// GEN INFO
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/LHEEventProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/LHERunInfoProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/GenFilterInfo.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

// ROOT includes
#include "TTree.h"
#include "TLorentzVector.h"
#include "TString.h"
#include "TObjArray.h"

//
// class declaration
//


class GenNtuplizer : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
   public:
      explicit GenNtuplizer(const edm::ParameterSet&);
      ~GenNtuplizer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;

      // ----------member data ---------------------------
      edm::EDGetTokenT<GenEventInfoProduct> generatorToken_; 
      edm::EDGetTokenT<LHEEventProduct> generatorlheToken_; 
	  edm::EDGetTokenT<reco::GenParticleCollection> genParticlesToken_;
	  
	  TH1F *counter_;
	  TTree *tree_;
	  MiniEvent_t ev_;
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
GenNtuplizer::GenNtuplizer(const edm::ParameterSet& iConfig)
 :
  generatorToken_(consumes<GenEventInfoProduct>(iConfig.getParameter<edm::InputTag>("generatorToken"))),
  generatorlheToken_(consumes<LHEEventProduct>(iConfig.getParameter<edm::InputTag>("generatorlheToken"))),
  genParticlesToken_(consumes<reco::GenParticleCollection>(iConfig.getParameter<edm::InputTag>("genParticlesToken")))

{
   //now do what ever initialization is needed

}


GenNtuplizer::~GenNtuplizer()
{

   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
GenNtuplizer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
   using namespace std;

   counter_->Fill(1);
   
   // Event info

   ev_.run     = iEvent.id().run();
   ev_.lumi    = iEvent.luminosityBlock();
   ev_.event   = iEvent.id().event(); 
   ev_.isData  = iEvent.isRealData();
   
   // Read LHE weights
   edm::Handle<GenEventInfoProduct> evt;
   iEvent.getByToken(generatorToken_, evt);
   ev_.weight = evt.isValid() ? evt->weight() : 0;
   
   //edm::Handle<LHEEventProduct> evet;
   //iEvent.getByToken(generatorlheToken_, evet);
   //ev_.weight = evet.isValid() ? evet->originalXWGTUP() : 0;
   

   // Read MCParticles
   edm::Handle<reco::GenParticleCollection> genParticles;
   iEvent.getByToken(genParticlesToken_,genParticles);
   TLorentzVector lep_p, lep_m;
   TLorentzVector in_p, in_m;

   for (size_t i = 0; i < genParticles->size(); ++i){
	   const reco::GenParticle & genIt = (*genParticles)[i];
       int id=genIt.pdgId();
       int absid=abs(id);
	   
	   // Incoming partons
	   if(genIt.status()==21 && id==22){
		   if(genIt.pz()>0) in_p.SetXYZM(genIt.px(),genIt.py(),genIt.pz(),genIt.mass());
		   else in_m.SetXYZM(genIt.px(),genIt.py(),genIt.pz(),genIt.mass());
	   }

	   if( !(genIt.status()==1 && genIt.isLastCopy()) ) continue;

	   if(absid==13){
		   if(genIt.pz()>0) lep_p.SetPtEtaPhiM(genIt.pt(),genIt.eta(),genIt.phi(),genIt.mass());
		   else lep_m.SetPtEtaPhiM(genIt.pt(),genIt.eta(),genIt.phi(),genIt.mass());
	   }

   }
   
   ev_.m = (in_m+in_p).M();
   ev_.y = (in_p+in_m).Rapidity();
   ev_.xiPos = (+1)*in_p.Pz() / 6500.;
   ev_.xiNeg = (-1)*in_m.Pz() / 6500.;
   
   ev_.lep1_pt = (lep_p.Pt() > lep_m.Pt()) ? lep_p.Pt() : lep_m.Pt();
   ev_.lep2_pt = (lep_p.Pt() > lep_m.Pt()) ? lep_m.Pt() : lep_p.Pt();
   ev_.lep1_eta = (lep_p.Pt() > lep_m.Pt()) ? lep_p.Eta() : lep_m.Eta();
   ev_.lep2_eta = (lep_p.Pt() > lep_m.Pt()) ? lep_m.Eta() : lep_p.Eta();
   
   
   // Fill event tree
   tree_->Fill();

#ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
   ESHandle<SetupData> pSetup;
   iSetup.get<SetupRecord>().get(pSetup);
#endif
}


// ------------ method called once each job just before starting event loop  ------------
void
GenNtuplizer::beginJob()
{
	
	// register to the TFileService
	edm::Service<TFileService> fs;
	
	// book histograms / tree:
	counter_ = fs->make<TH1F>("counter", ";Counter;Events",2,0,2);
	tree_ = fs->make<TTree>("tree","Tree with all GenLevel info");
	createMiniEventTree(tree_,ev_);
	
	// initiate histogram with the total number of events
	counter_->SetBinContent(1,50000);
}

// ------------ method called once each job just after ending the event loop  ------------
void
GenNtuplizer::endJob()
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
GenNtuplizer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);

  //Specify that only 'tracks' is allowed
  //To use, remove the default given above and uncomment below
  //ParameterSetDescription desc;
  //desc.addUntracked<edm::InputTag>("tracks","ctfWithMaterialTracks");
  //descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(GenNtuplizer);
