import FWCore.ParameterSet.Config as cms
from FWCore.ParameterSet.VarParsing import VarParsing
options = VarParsing ('analysis')
options.parseArguments()

process = cms.Process("WeightAnalysis")

# import of standard configurations
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.load('Configuration.StandardSequences.Services_cff')
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(100)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(options.maxEvents) )
process.source = cms.Source("PoolSource",fileNames = cms.untracked.vstring(options.inputFiles))

process.analysis = cms.EDAnalyzer('GenWeightAnal')
process.analysis.externalLHE = cms.InputTag("externalLHEProducer")

process.TFileService = cms.Service("TFileService", 
      fileName = cms.string("output.root"),
      closeFileFast = cms.untracked.bool(True)
  )

process.p = cms.Path(process.analysis)
