#!/bin/sh
#./GenAnalyzer/GenWeightAnal/scripts/process_all.sh /eos/cms/store/cmst3/group/taug2/MiniAOD/GGToTauTau_Elastic_M_50GeV_PTFilter_ktSmear_RunIISummer20UL18_TuneCP5_madgraphLO_reweight/

# set if you will run locally or using condor
DOCONDOR=1

# check number of arguments
if [ "$#" -ne 1 ]; then
  echo ERROR: $# input variables
  echo "Usage: $0 PATH_TO_INPUT" >&2
  echo "Example: $0 /eos/cms/store/cmst3/group/taug2/MiniAOD/GGToTauTau_Elastic_M_50GeV_PTFilter_ktSmear_RunIISummer20UL18_TuneCP5_madgraphLO_reweight/" >&2
  exit 1
fi

extime="testmatch" #testmatch tomorrow workday
condor="submit_condor.sub"
if [ "$DOCONDOR" -eq 1  ]; then
echo "executable  = $CMSSW_BASE/src/GenAnalyzer/GenWeightAnal/scripts/run_job.sh" > $condor
echo "output      = $PWD/${condor}.out" >> $condor
echo "error       = $PWD/${condor}.err" >> $condor
echo "log         = $PWD/${condor}.log" >> $condor
echo "+JobFlavour =\"${extime}\"">> $condor
echo "requirements = (OpSysAndVer =?= \"CentOS7\")" >> $condor  # SLCern6 CentOS7
fi

sourceFolder=$1
outputfolder=$PWD/output

echo INFO: found `ls $sourceFolder | wc -l` files to process...

mkdir -pv $outputfolder
for f in `ls ${sourceFolder}/*root`; do
  fname=`basename $f`;
  fname=`echo $fname | rev | cut -d. -f2- | rev`
  outfile=${outputfolder}/${fname}.root
 
  # check if the output file exists:
  if test -f "$outfile"; then
    continue
	#echo file $outfile exists
  fi

  if [ "$DOCONDOR" -eq 1 ]; then
    echo submit $f
    echo "arguments = `readlink -f $f` $outputfolder/${fname}.root $CMSSW_BASE " >> $condor
    echo "queue 1" >> $condor
  else
    echo process $f
    source $CMSSW_BASE/src/GenAnalyzer/GenWeightAnal/scripts/run_job.sh $f $outfile $CMSSW_BASE
  fi
  
done
if [ "$DOCONDOR" -eq 1  ]; then
  echo "Submitting $condor"
  echo condor_submit $condor
fi

