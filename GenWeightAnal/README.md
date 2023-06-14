# GenWeightAnal
Produces output with all LHE weights

## Setup the code

```
#for 10.4 cycle
cmsrel CMSSW_10_6_32_patch1
cd CMSSW_10_6_32_patch1/src
cmsenv

#for 12.5 cycle
cmsrel CMSSW_12_5_0
cd CMSSW_12_5_0/src
cmsenv


git clone git@github.com:michael-pitt/GenAnalyzer.git
scram b -j2
```


## Running analyzer

To run the analyzer on a single file, execute
```
$CMSSW_BASE/src/GenAnalyzer/GenWeightAnal/scripts/run_job.sh inputfile outputfile $CMSSW_BASE
```

To run the analyzer on a folder, execute
```
mkdir output

$CMSSW_BASE/src/GenAnalyzer/GenWeightAnal/scripts/process_all.sh folder_with_intputfiles
```

You can set the `DOCONDOR` variable if you wish to run locally or on Condor
