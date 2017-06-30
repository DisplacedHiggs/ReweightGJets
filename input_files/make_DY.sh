eosls /store/user/lpchbb/kreis/AnalysisTrees_PhotonTest_May11/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8 | grep "root" | awk '{print "root://cmsxrootd.fnal.gov//store/user/lpchbb/kreis/AnalysisTrees_PhotonTest_May11/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/"$1}' > DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt

hadd -T histOnly_DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.root `xrdfsls -u /store/user/lpchbb/kreis/AnalysisTrees_PhotonTest_May11/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8 | grep '\.root'`

