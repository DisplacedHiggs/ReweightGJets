# ReweightGJets


condor_submit submit_num_den.condor

hadd output 

condor_submit submit_apply_weights.condor

hadd output

then you can run make_plots()