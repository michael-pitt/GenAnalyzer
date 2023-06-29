startMsg='Job started on '`date`
echo $startMsg

if [ "$#" -ne 3 ]; then
  echo ERROR: $# input variables
  echo "Usage: $0 INPUTFILE OUTPUTILE CMSSW" >&2
  exit 1
fi

####### USER SETTINGS ###########
export HOME=/afs/cern.ch/user/m/mpitt
source /afs/cern.ch/user/m/mpitt/.zshrc
#################################

inputfile=$1
outputfile=$2

echo INFO: setup CMSSW
cd $3/src
eval `scram runtime -sh`

echo process file:
cmsRun $CMSSW_BASE/src/GenAnalyzer/GenWeightAnal/python/run_genWeight.py inputFiles=file:$inputfile outputFile=file:$outputfile

echo dome
echo $startMsg
echo job finished on `date`


