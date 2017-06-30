#!/bin/bash

MAINDIR=`pwd`
INPATH=/store/user/lpchbb/kreis/AnalysisTrees_PhotonTest_May11
OUTPATH=/store/user/kreis/reweight_gjets_test2_100
G_PT_CUT=100

LOGDIR=$MAINDIR/logs$G_PT_CUT

mkdir $LOGDIR
eos root://cmseos.fnal.gov mkdir $OUTPATH

condorFile=submit_all.condor
if [ -e $condorFile ]
then
    rm -rf $condorFile
fi
touch $condorFile

echo "universe = vanilla" >> $condorFile
echo "Requirements = OpSys == \"LINUX\" && (Arch != \"DUMMY\" )" >> $condorFile
echo "Executable = run_apply_weights.sh" >> $condorFile
echo "Should_Transfer_Files = YES" >> $condorFile
echo "request_disk = 10000000" >> $condorFile
echo "request_memory = 2100" >> $condorFile
echo "WhenTOTransferOutput  = ON_EXIT_OR_EVICT" >> $condorFile
echo "Notification=never" >> $condorFile
echo "notify_user = kreis@fnal.gov" >> $condorFile
echo "x509userproxy = \$ENV(X509_USER_PROXY)" >> $condorFile
echo "Transfer_Input_Files = run_apply_weights.sh, input_files.tgz, CMSSW_8_0_18_patch1.tar.gz, ReweightGJets.C, $LOGDIR/fout_num_den_hadded_QCD.root, $LOGDIR/fout_num_den_hadded_GJets.root" >> $condorFile
echo "" >> $condorFile

processlist=processes.list
while IFS='' read -r line || [[ -n "$line" ]]; do

    dir1=`eos root://cmseos.fnal.gov ls -l $INPATH/$line | awk '{print $9}'`
    tmplist=tmp.tmp
    #echo $dir1
    echo $dir1| tr " " "\n" > $tmplist

    while IFS='' read -r filename || [[ -n "$filename" ]]; do

	echo "output = $LOGDIR/\$(Cluster)_apply-weights_$filename.out" >> $condorFile
	echo "error = $LOGDIR/\$(Cluster)_apply-weights_$filename.err" >> $condorFile
	echo "log = $LOGDIR/\$(Cluster)_apply-weights_$filename.log" >> $condorFile
	echo "arguments = $OUTPATH root://cmsxrootd.fnal.gov/$INPATH/$line/ $filename apply_weights 1 $G_PT_CUT" >> $condorFile
	echo "queue" >> $condorFile	
	echo "" >> $condorFile

    done < "$tmplist"


done < "$processlist"
