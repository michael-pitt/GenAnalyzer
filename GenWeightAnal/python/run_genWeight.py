import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing

options.parseArguments()
process = cms.Process("WeightAnalysis")

# import of standard configurations
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.load('Configuration.StandardSequences.Services_cff')
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(100)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(options.maxEvents) )
process.source = cms.Source("PoolSource",fileNames = cms.untracked.vstring(options.inputFiles))

process.genwanal = cms.EDAnalyzer('GenWeightAnal')

process.p = cms.Path(process.genwanal)