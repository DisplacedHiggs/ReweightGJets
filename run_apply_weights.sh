#!/bin/bash

export VO_CMS_SW_DIR=/cvmfs/cms.cern.ch
export COIN_FULL_INDIRECT_RENDERING=1
echo $VO_CMS_SW_DIR
source $VO_CMS_SW_DIR/cmsset_default.sh
export SCRAM_ARCH=slc6_amd64_gcc493
tar xzf CMSSW_8_0_18_patch1.tar.gz
cd CMSSW_8_0_18_patch1/src
scram b ProjectRename
eval `scramv1 runtime -sh`
cd -

tar xzf input_files.tgz

echo "arg0 $0"
echo "arg1 $1"
echo "arg2 $2"
echo "arg3 $3"
echo "arg4 $4"

root -l -b -q "ReweightGJets.C+(\"$2\",\"$3\",\"unused\",\"apply_weights\",0)"
xrdcp fout_weighted_$3.root root://cmseos.fnal.gov/$1/fout_weighted_$3.root
