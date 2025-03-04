startMsg='Job started on '`date`
echo $startMsg

if [ "$#" -ne 3 ]; then
  echo ERROR: $# input variables
  echo "Usage: $0 INPUTFILE OUTPUTILE CMSSW" >&2
  exit 1
fi

inputfile=$1
outputfile=$2
release=$3

echo process file:
cmssw-el7 -- "cd $release/src; eval \`scram runtime -sh\`; cd -; cmsRun $release/src/GenAnalyzer/GenNtuplizer/python/make_ntuple.py inputFiles=file:$inputfile outputFile=file:$outputfile"

echo dome
echo $startMsg
echo job finished on `date`


