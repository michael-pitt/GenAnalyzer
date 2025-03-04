# GenWeightAnal
Produces ntuples from GEN files (SIM step in cmsDriver.py)

## Setup the code

```
#for 10.6 cycle

cmssw-el7
cmsrel CMSSW_10_6_46
cd CMSSW_10_6_46/src
cmsenv

#for 12.5 cycle
cmsrel CMSSW_12_5_0
cd CMSSW_12_5_0/src
cmsenv


git clone git@github.com:michael-pitt/GenAnalyzer.git
scram b -j2
```


## Running ntuplizer

To run the ntuplizer on a single file, execute

```bash
cmsRun $CMSSW_BASE/src/GenAnalyzer/GenNtuplizer/python/make_ntuple.py inputFiles=file:inputfile outputFile=file:outputfile
```


To run the ntuplizer on a folder, execute
```
$CMSSW_BASE/src/GenAnalyzer/GenNtuplizer/scripts/process_all.sh folder_with_intputfiles
```

You can set the `DOCONDOR` variable if you wish to run locally or on Condor

