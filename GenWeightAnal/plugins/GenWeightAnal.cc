// -*- C++ -*-
//
// Package:    GenAnalyzer/GenWeightAnal
// Class:      GenWeightAnal
//
/**\class GenWeightAnal GenWeightAnal.cc GenAnalyzer/GenWeightAnal/plugins/GenWeightAnal.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Michael Pitt
//         Created:  Wed, 29 Mar 2023 09:07:09 GMT
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
#include "GenAnalyzer/GenWeightAnal/interface/MiniEvent.h"

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

class GenWeightAnal : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
   public:
      explicit GenWeightAnal(const edm::ParameterSet&);
      ~GenWeightAnal();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;

      // ----------member data ---------------------------
      edm::EDGetTokenT<LHEEventProduct> externalLHE_; 
	  edm::EDGetTokenT<reco::GenParticleCollection> prunedGenParticlesToken_;
	  edm::EDGetTokenT<GenFilterInfo> genFilterInfoToken_;
	  
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
GenWeightAnal::GenWeightAnal(const edm::ParameterSet& iConfig)
 :
  externalLHE_(consumes<LHEEventProduct>(iConfig.getParameter<edm::InputTag>("externalLHE"))),
  prunedGenParticlesToken_(consumes<reco::GenParticleCollection>(edm::InputTag("prunedGenParticles"))),
  genFilterInfoToken_(consumes<GenFilterInfo, edm::InLumi>(edm::InputTag("genFilterEfficiencyProducer")))

{
   //now do what ever initialization is needed
   
}


GenWeightAnal::~GenWeightAnal()
{

   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
GenWeightAnal::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
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
   edm::Handle<LHEEventProduct> lheInfo;
   iEvent.getByToken(externalLHE_, lheInfo);
   vector<gen::WeightsInfo> weights = lheInfo->weights();
   ev_.g_nw = weights.size();
   
   if (ev_.g_nw > ev_.MAXWEIGHTS){
	   cout << "ERROR: number of weights = " << ev_.g_nw << " > MAXWEIGHTS = " << ev_.MAXWEIGHTS << endl;
	   for(unsigned int i=0  ; i<ev_.MAXWEIGHTS;i++) ev_.g_w[i] = 0;
   }
   else{
	for(unsigned int i=0  ; i<ev_.g_nw;i++){
//	   cout << weights[i].id << " = " << weights[i].wgt << endl;
	   ev_.g_w[i] = weights[i].wgt;
	   TObjArray * split_id = TString(weights[i].id.c_str()).Tokenize("_");
	   ev_.g_wval[i] = (split_id->GetEntries()==2) ? TString(split_id->At(1)->GetName()).ReplaceAll("p",".").ReplaceAll("m","-").Atof() : 0;
	}
   }

   // Read MCParticles
   edm::Handle<reco::GenParticleCollection> prunedGenParticles;
   iEvent.getByToken(prunedGenParticlesToken_,prunedGenParticles);
   TLorentzVector taup, taum; int ntau=0;
   for (size_t i = 0; i < prunedGenParticles->size(); ++i){
	   const reco::GenParticle & genIt = (*prunedGenParticles)[i];
       int id=genIt.pdgId();
       int absid=abs(id);
	   if(absid==15 && genIt.isLastCopy()){
		   ntau++;
		   if(genIt.charge()>0) taup.SetPtEtaPhiM(genIt.pt(),genIt.eta(),genIt.phi(),genIt.mass());
		   else taum.SetPtEtaPhiM(genIt.pt(),genIt.eta(),genIt.phi(),genIt.mass());
	   }
   }
   ev_.mtautau = (ntau==2) ? (taup+taum).M() : -1;
   ev_.ytautau = (ntau==2) ? (taup+taum).Rapidity() : -1;
   ev_.xiPos = (ntau==2) ? (taup.Pt()*exp( taup.Eta())+taum.Pt()*exp( taum.Eta()))/13000. : -1;
   ev_.xiNeg = (ntau==2) ? (taup.Pt()*exp(-taup.Eta())+taum.Pt()*exp(-taum.Eta()))/13000. : -1;
   
   // Fill event tree
   tree_->Fill();

#ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
   ESHandle<SetupData> pSetup;
   iSetup.get<SetupRecord>().get(pSetup);
#endif
}


// ------------ method called once each job just before starting event loop  ------------
void
GenWeightAnal::beginJob()
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
GenWeightAnal::endJob()
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
GenWeightAnal::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
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
DEFINE_FWK_MODULE(GenWeightAnal);
