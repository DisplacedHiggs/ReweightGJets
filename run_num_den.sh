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

root -l -b -q "ReweightGJets_QCD.C+(\"$1\",\"num_den\",$3)"
xrdcp fout_num_den_$1.root root://cmseos.fnal.gov//store/user/kreis/reweight_gjets/fout_num_den_$1.root

