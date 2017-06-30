#include <iostream>

#include "TMath.h"
#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TFileCollection.h"
#include "TH1D.h"
#include "TString.h"
#include "TCanvas.h"
#include "TEntryList.h"
#include "TDirectory.h"
#include "THStack.h"
#include "TPad.h"
#include "TLegend.h"
#include "TPaveStats.h"
#include "TGaxis.h"

using namespace std;


//remember sumw2, cuts on dy and gj, no photons


struct plot_struct{
  TString name;
  TString var_dy;
  TString var_gj;
  unsigned int nbins;
  double min;
  double max;
};


void unit_normalize(TH1D &h){
  h.Scale(1.0/h.Integral());
}


void unit_normalize(TH1D *h){
  h->Scale(1.0/h->Integral());
}


void unit_normalize_vector(std::vector<TH1D> &v){
  for(unsigned int i=0; i<v.size(); i++){
    unit_normalize( v.at(i) );
  }
}


void write_vector(std::vector<TH1D> &v){
  for(unsigned int i=0; i<v.size(); i++){
    v.at(i).Write();
  }
}


double error_on_r(double a, double da, double b, double db){
  //error on a/b
  cout << "a " << a << " +- " << da << ", b " << b << " +- " << db << endl;
  cout << "r " << a/b << " " << (a/b)*sqrt(da*da/(a*a) + db*db/(b*b)) << endl;
  return (a/b)*sqrt(da*da/(a*a) + db*db/(b*b));

}

std::vector<TH1D> make_output_histos(TString name, int nbins, double min, double max, std::vector<plot_struct> my_weightings){
  TH1D    h_gj("h_"+name+"_gj",    "h_"+name+"_gj",    nbins, min, max);
  TH1D h_gj_w1("h_"+name+"_gj_w_"+my_weightings.at(0).name, "h_"+name+"_gj_w_"+my_weightings.at(0).name, nbins, min, max);
  //TH1D h_gj_w2("h_"+name+"_gj_w_"+my_weightings.at(1).name, "h_"+name+"_gj_w_"+my_weightings.at(1).name, nbins, min, max);
  //TH1D h_gj_w3("h_"+name+"_gj_w_"+my_weightings.at(2).name, "h_"+name+"_gj_w_"+my_weightings.at(2).name, nbins, min, max);
  //TH1D h_gj_w4("h_"+name+"_gj_w_"+my_weightings.at(3).name, "h_"+name+"_gj_w_"+my_weightings.at(3).name, nbins, min, max);
  //TH1D h_gj_w5("h_"+name+"_gj_w_"+my_weightings.at(4).name, "h_"+name+"_gj_w_"+my_weightings.at(4).name, nbins, min, max);
  h_gj.Sumw2();
  h_gj_w1.Sumw2();
  //h_gj_w2.Sumw2();
  //h_gj_w3.Sumw2();
  //h_gj_w4.Sumw2();
  //h_gj_w5.Sumw2();

  std::vector<TH1D> v_gj;
  v_gj.push_back(h_gj);
  v_gj.push_back(h_gj_w1);
  //v_gj.push_back(h_gj_w2);
  //v_gj.push_back(h_gj_w3);
  //v_gj.push_back(h_gj_w4);
  //v_gj.push_back(h_gj_w5);

  return v_gj;
}


std::vector<TH1D> get_histos(TFile* file, TString name, std::vector<plot_struct> my_weightings){

  std::vector<TH1D> v_gj;
  v_gj.push_back(*(TH1D*)file->Get("h_"+name+"_gj"));
  v_gj.push_back(*(TH1D*)file->Get("h_"+name+"_gj_w_"+my_weightings.at(0).name));
  //v_gj.push_back(*(TH1D*)file->Get("h_"+name+"_gj_w_"+my_weightings.at(1).name));
  //v_gj.push_back(*(TH1D*)file->Get("h_"+name+"_gj_w_"+my_weightings.at(2).name));
  //v_gj.push_back(*(TH1D*)file->Get("h_"+name+"_gj_w_"+my_weightings.at(3).name));
  //v_gj.push_back(*(TH1D*)file->Get("h_"+name+"_gj_w_"+my_weightings.at(4).name));
  
  return v_gj;
}


void fill_histo_vector( std::vector<TH1D>& v_histos, std::vector<double> v_weights, double x, double additional_weight){
  v_histos.at(0).Fill(x, additional_weight);
  v_histos.at(1).Fill(x, v_weights.at(0)*additional_weight);
  //v_histos.at(2).Fill(x, v_weights.at(1)*additional_weight);
  //v_histos.at(3).Fill(x, v_weights.at(2)*additional_weight);
  //v_histos.at(4).Fill(x, v_weights.at(3)*additional_weight);
  //v_histos.at(5).Fill(x, v_weights.at(4)*additional_weight);
}


void draw_histos(TH1D* h_gj, TH1D* h_dy, TString variable, TString weight){
  h_dy->SetLineColor(kBlack);
  h_gj->SetLineColor(kRed);
  h_dy->SetLineWidth(2);
  h_gj->SetLineWidth(2);

  h_dy->SetMinimum(1e-14);
  h_gj->SetMinimum(1e-14);
  //h_dy->GetYaxis()->SetLimits(1e-14, h_dy->GetMaximum()*1.25);
  //h_gj->GetYaxis()->SetLimits(1e-14, h_gj->GetMaximum()*1.25);
  h_dy->GetYaxis()->SetLimits(1e-14, 1e-5);
  h_gj->GetYaxis()->SetLimits(1e-14, 1e-5);

  TCanvas c("c_"+TString(h_gj->GetName())+"_"+TString(h_dy->GetName()), "c_"+TString(h_gj->GetName())+"_"+TString(h_dy->GetName()), 800, 800);
  
  TPad *pad1 = new TPad("pad1", "pad1", 0, 0.05, 1, 1.0);
  pad1->SetBottomMargin(0); // Upper and lower plot are joined
  //pad1->SetGridx();         // Vertical grid
  pad1->Draw();             // Draw the upper pad: pad1
  pad1->cd();               // pad1 becomes the current pad
  h_gj->Draw("HIST E");
  h_dy->Draw("HIST E SAMES");

  /*
  h_dy->GetYaxis()->SetLabelSize(0.);
  TGaxis *axis = new TGaxis( -5, 20, -5, 220, 20,220,510,"");
  //TGaxis *axis = new TGaxis( -5, 20, -5, 220, 20,220,512,"");
  //TGaxis *axis = new TGaxis( h_dy->GetXaxis()->GetBinLowEdge(1), h_dy->GetMinimum(), h_dy->GetXaxis()->GetBinUpEdge(h_dy->GetNbinsX()), h_dy->GetMaximum(), 20,220,512,"");
  axis->SetLabelFont(43); // Absolute font size in pixel (precision 3)
  axis->SetLabelSize(15);
  axis->Draw();
  */


  
  c.cd();          // Go back to the main canvas before defining pad2
  TPad *pad2 = new TPad("pad2", "pad2", 0, 0.05, 1, 0.3);
  pad2->SetTopMargin(0);
  pad2->SetBottomMargin(0.2);
  //pad2->SetGridx(); // vertical grid
  pad2->Draw();
  pad2->cd();       // pad2 becomes the current pad

  
  // Define the ratio plot
  TH1F *h3 = (TH1F*)h_gj->Clone("h3");
  h3->SetLineColor(kBlack);
  h3->SetMinimum(0);  // Define Y ..
  h3->SetMaximum(3); // .. range
  //h3->Sumw2();
  h3->SetStats(0);      // No statistics on lower plot
  h3->Divide(h_dy);
  h3->SetMarkerStyle(21);
  h3->Draw("ep");       // Draw the ratio plot

  for(int i=1; i<=h_dy->GetNbinsX(); i++){
    double r = error_on_r(h_gj->GetBinContent(i), h_gj->GetBinError(i), h_dy->GetBinContent(i), h_dy->GetBinError(i));
    cout << i << ", GJ: " << h_gj->GetBinContent(i) << ", DY: " << h_dy->GetBinContent(i) << ", R: " << h3->GetBinContent(i) << " +- " << r << endl;
  }

  
  // Y axis h1 plot settings
  //h_gj->GetYaxis()->SetTitleSize(20);
//  h_gj->GetYaxis()->SetTitleFont(43);
  //h_gj->GetYaxis()->SetTitleOffset(1.55);
 

  // Ratio plot (h3) settings
  h3->SetTitle(""); // Remove the ratio title
  
  // Y axis ratio plot settings
  h3->GetYaxis()->SetTitle("GJets/DY");
  h3->GetYaxis()->SetNdivisions(505);
  h3->GetYaxis()->SetTitleSize(20);
  h3->GetYaxis()->SetTitleFont(43);
  h3->GetYaxis()->SetTitleOffset(1.55);
  h3->GetYaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
  h3->GetYaxis()->SetLabelSize(15);
  
  // X axis ratio plot settings
  h3->GetXaxis()->SetTitle(variable);
  h3->GetXaxis()->SetTitleSize(20);
  h3->GetXaxis()->SetTitleFont(43);
  h3->GetXaxis()->SetTitleOffset(4.);
  h3->GetXaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
  h3->GetXaxis()->SetLabelSize(15);

  
  gPad->Update();
  TPaveStats *st = (TPaveStats*)h_gj->GetListOfFunctions()->FindObject("stats");
  cout << "st " << st << endl;
  st->SetY1NDC(0.5); st->SetY2NDC(0.7);
  st->SetTextColor(h_gj->GetLineColor());
  gPad->Modified();

  pad1->cd();
  TLegend leg(.45, .75, .65, .85);
  leg.SetBorderSize(0);
  leg.SetFillColor(0);
  leg.SetHeader("weight: "+weight);
  leg.AddEntry(h_dy, "DY", "L");
  leg.AddEntry(h_gj, "GJets", "L");
  leg.Draw();
  c.SaveAs("plots/h-"+variable+"_w-"+weight+".pdf");
  

  pad1->cd();
  gPad->SetLogy();
  gPad->Modified();
  c.SaveAs("plots/h-"+variable+"_w-"+weight+"_log.pdf");
  
}




plot_struct weighting1;
plot_struct weighting2;
plot_struct weighting3;
plot_struct weighting4;
plot_struct weighting5;
std::vector<plot_struct> my_weightings;


void setup_weights(){
  /*
  weighting2.name = "NJets";
  weighting2.var_dy = "NBASICCALOJETS1PT20";
  weighting2.var_gj = "NBASICCALOJETS1PT20";
  weighting2.nbins = 9;
  weighting2.min = 1;
  weighting2.max = 10;
    
  weighting1.name = "HT";
  weighting1.var_dy = "HT";
  weighting1.var_gj = "HT";
  weighting1.nbins = 30;
  weighting1.min = 40;
  weighting1.max = 500;
  */
  
  weighting3.name = "NJetsNoPhotons";
  weighting3.var_dy = "NBASICCALOJETS1PT20NOPHOTONS";
  weighting3.var_gj = "NBASICCALOJETS1PT20NOPHOTONS";
  weighting3.nbins = 9;
  weighting3.min = 1;
  weighting3.max = 9;
  
  /*
  weighting4.name = "VE";
  weighting4.var_dy = "TMath::Sqrt((1+TMath::SinH(ETAOSSF)*TMath::SinH(ETAOSSF))*PTOSSF*PTOSSF+MOSSF*MOSSF)";
  weighting4.var_gj = "TMath::Sqrt((1+TMath::SinH(ETA_MEDIUMPHOTONS[0])*TMath::SinH(ETA_MEDIUMPHOTONS[0]))*PT_MEDIUMPHOTONS[0]*PT_MEDIUMPHOTONS[0])";
  weighting4.nbins = 30;
  weighting4.min = 0;
  weighting4.max = 400;
  
  weighting5.name = "PT";
  weighting5.var_dy = "PTOSSF";
  weighting5.var_gj = "PT_MEDIUMPHOTONS[0]";
  weighting5.nbins = 10;
  weighting5.min = 10;
  weighting5.max = 250;
  */
  
  my_weightings.clear();
  //my_weightings.push_back(weighting1);
  //my_weightings.push_back(weighting2);
  my_weightings.push_back(weighting3);
  //my_weightings.push_back(weighting4);
  //my_weightings.push_back(weighting5);
}


int out_nTags_nbins = 5;
double out_nTags_min = 0, out_nTags_max = 5;
int out_nJets_nbins = 10;
double out_nJets_min = 0, out_nJets_max = 10;
int out_JetPt_nbins = 15;
double out_JetPt_min = 0, out_JetPt_max = 150;
int out_HT_nbins = 30;
double out_HT_min = 40, out_HT_max = 500;
int out_PT_nbins = 10;
double out_PT_min = 10, out_PT_max = 250;
int out_VE_nbins = 30;
double out_VE_min = 0, out_VE_max = 400;


double get_xs(TString gjets_name){
  double xs = 0;
  if(gjets_name == "GJets40" || gjets_name.Contains("GJets_HT-40To")) xs = 20790;
  else if(gjets_name == "GJets100" || gjets_name.Contains("GJets_HT-100")) xs = 9238;
  else if(gjets_name == "GJets200" || gjets_name.Contains("GJets_HT-200")) xs = 2305;
  else if(gjets_name == "GJets400" || gjets_name.Contains("GJets_HT-400")) xs = 274.4;
  else if(gjets_name == "GJets600" || gjets_name.Contains("GJets_HT-600")) xs = 93.46;
  else if(gjets_name.Contains("QCD15_")) xs = 1837410000;//bentodo
  else if(gjets_name.Contains("QCD30_")) xs = 140932000;
  else if(gjets_name.Contains("QCD50_")) xs = 19204300;
  else if(gjets_name.Contains("QCD80_")) xs = 2762530;
  else if(gjets_name.Contains("QCD120_")) xs = 471100;
  else if(gjets_name.Contains("QCD170_")) xs = 117276;
  else if(gjets_name.Contains("QCD300_")) xs = 7823;
  else if(gjets_name.Contains("QCD470_")) xs = 648.2;
  else if(gjets_name.Contains("QCD600_")) xs = 186.9;
  else if(gjets_name.Contains("QCD800_")) xs = 32.293;
  else if(gjets_name.Contains("QCD1000_")) xs = 9.4183;
  else if(gjets_name.Contains("QCD1400_")) xs = 0.84265;
  else if(gjets_name.Contains("QCD1800_")) xs = 0.114943;
  else if(gjets_name.Contains("QCD2400_")) xs = 0.00682981;
  else if(gjets_name.Contains("QCD3200_")) xs = 0.000165445;
  else cout << "WARNING: Didn't find XS for " << gjets_name << endl;
  return xs;
}

TString get_file(TString gjets_name){
  TString filename = "ben.root";
  if(gjets_name == "GJets40") filename = "input_files/GJets_HT-40To100_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt";
  else if(gjets_name == "GJets100") filename = "input_files/GJets_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt";
  else if(gjets_name == "GJets200") filename = "input_files/GJets_HT-200To400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt";
  else if(gjets_name == "GJets400") filename = "input_files/GJets_HT-400To600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt";
  else if(gjets_name == "GJets600") filename = "input_files/GJets_HT-600ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt";
  else if(gjets_name == "QCD15") filename = "input_files/QCD_Pt_15to30_TuneCUETP8M1_13TeV_pythia8.txt";
  else if(gjets_name == "QCD30") filename = "input_files/QCD_Pt_30to50_TuneCUETP8M1_13TeV_pythia8.txt";
  else if(gjets_name == "QCD50") filename = "input_files/QCD_Pt_50to80_TuneCUETP8M1_13TeV_pythia8.txt";
  else if(gjets_name == "QCD80") filename = "input_files/QCD_Pt_80to120_TuneCUETP8M1_13TeV_pythia8.txt";
  else if(gjets_name == "QCD120") filename = "input_files/QCD_Pt_120to170_TuneCUETP8M1_13TeV_pythia8.txt";
  else if(gjets_name == "QCD170") filename = "input_files/QCD_Pt_170to300_TuneCUETP8M1_13TeV_pythia8.txt";
  else if(gjets_name == "QCD300") filename = "input_files/QCD_Pt_300to470_TuneCUETP8M1_13TeV_pythia8.txt";
  else if(gjets_name == "QCD470") filename = "input_files/QCD_Pt_470to600_TuneCUETP8M1_13TeV_pythia8.txt";
  else if(gjets_name == "QCD600") filename = "input_files/QCD_Pt_600to800_TuneCUETP8M1_13TeV_pythia8.txt";
  else if(gjets_name == "QCD800") filename = "input_files/QCD_Pt_800to1000_TuneCUETP8M1_13TeV_pythia8.txt";
  else if(gjets_name == "QCD1000") filename = "input_files/QCD_Pt_1000to1400_TuneCUETP8M1_13TeV_pythia8.txt";
  else if(gjets_name == "QCD1400") filename = "input_files/QCD_Pt_1400to1800_TuneCUETP8M1_13TeV_pythia8.txt";
  else if(gjets_name == "QCD1800") filename = "input_files/QCD_Pt_1800to2400_TuneCUETP8M1_13TeV_pythia8.txt";
  else if(gjets_name == "QCD2400") filename = "input_files/QCD_Pt_2400to3200_TuneCUETP8M1_13TeV_pythia8.txt";
  else if(gjets_name == "QCD3200") filename = "input_files/QCD_Pt_3200toInf_TuneCUETP8M1_13TeV_pythia8.txt";
  else cout << "WARNING: Didn't find filename for " << gjets_name << endl;
  return filename;
}

double get_count(TString gjets_name){
  TString filename = "ben.root";
  if(gjets_name == "GJets40" || gjets_name.Contains("GJets_HT-40To")) filename = "input_files/histOnly_GJets_HT-40To100_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.root";
  else if(gjets_name == "GJets100" || gjets_name.Contains("GJets_HT-100")) filename = "input_files/histOnly_GJets_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.root";
  else if(gjets_name == "GJets200" || gjets_name.Contains("GJets_HT-200")) filename = "input_files/histOnly_GJets_HT-200To400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.root";
  else if(gjets_name == "GJets400" || gjets_name.Contains("GJets_HT-400")) filename = "input_files/histOnly_GJets_HT-400To600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.root";
  else if(gjets_name == "GJets600" || gjets_name.Contains("GJets_HT-600")) filename = "input_files/histOnly_GJets_HT-600ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.root";
  else if(gjets_name.Contains("QCD15")) filename = "input_files/histOnly_QCD_Pt_15to30_TuneCUETP8M1_13TeV_pythia8.root";
  else if(gjets_name.Contains("QCD30")) filename = "input_files/histOnly_QCD_Pt_30to50_TuneCUETP8M1_13TeV_pythia8.root";
  else if(gjets_name.Contains("QCD50")) filename = "input_files/histOnly_QCD_Pt_50to80_TuneCUETP8M1_13TeV_pythia8.root";
  else if(gjets_name.Contains("QCD80")) filename = "input_files/histOnly_QCD_Pt_80to120_TuneCUETP8M1_13TeV_pythia8.root";
  else if(gjets_name.Contains("QCD120")) filename = "input_files/histOnly_QCD_Pt_120to170_TuneCUETP8M1_13TeV_pythia8.root";
  else if(gjets_name.Contains("QCD170")) filename = "input_files/histOnly_QCD_Pt_170to300_TuneCUETP8M1_13TeV_pythia8.root";
  else if(gjets_name.Contains("QCD300")) filename = "input_files/histOnly_QCD_Pt_300to470_TuneCUETP8M1_13TeV_pythia8.root";
  else if(gjets_name.Contains("QCD470")) filename = "input_files/histOnly_QCD_Pt_470to600_TuneCUETP8M1_13TeV_pythia8.root";
  else if(gjets_name.Contains("QCD600")) filename = "input_files/histOnly_QCD_Pt_600to800_TuneCUETP8M1_13TeV_pythia8.root";
  else if(gjets_name.Contains("QCD800")) filename = "input_files/histOnly_QCD_Pt_800to1000_TuneCUETP8M1_13TeV_pythia8.root";
  else if(gjets_name.Contains("QCD1000")) filename = "input_files/histOnly_QCD_Pt_1000to1400_TuneCUETP8M1_13TeV_pythia8.root";
  else if(gjets_name.Contains("QCD1400")) filename = "input_files/histOnly_QCD_Pt_1400to1800_TuneCUETP8M1_13TeV_pythia8.root";
  else if(gjets_name.Contains("QCD1800")) filename = "input_files/histOnly_QCD_Pt_1800to2400_TuneCUETP8M1_13TeV_pythia8.root";
  else if(gjets_name.Contains("QCD2400")) filename = "input_files/histOnly_QCD_Pt_2400to3200_TuneCUETP8M1_13TeV_pythia8.root";
  else if(gjets_name.Contains("QCD3200")) filename = "input_files/histOnly_QCD_Pt_3200toInf_TuneCUETP8M1_13TeV_pythia8.root";
  else cout << "WARNING: Didn't find filename for " << gjets_name << endl;

  TFile* file_gj = TFile::Open(filename);
  TH1D* h_count_gj = (TH1D*)file_gj->Get("noCutSignature_COUNT");
  return h_count_gj->GetBinContent(1);
}

 


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void check_stitch_QCD(){
  
  TFile* f15 = TFile::Open( "fout_num_den_QCD15.root" );
  TFile* f30 = TFile::Open( "fout_num_den_QCD30.root" );
  TFile* f50 = TFile::Open( "fout_num_den_QCD50.root" );
  TFile* f80 = TFile::Open( "fout_num_den_QCD80.root" );
  TFile* f120 = TFile::Open( "fout_num_den_QCD120.root" );
  TFile* f170 = TFile::Open( "fout_num_den_QCD170.root" );
  TFile* f300 = TFile::Open( "fout_num_den_QCD300.root" );
  TFile* f470 = TFile::Open( "fout_num_den_QCD470.root" );
  TFile* f600 = TFile::Open( "fout_num_den_QCD600.root" );
  TFile* f800 = TFile::Open( "fout_num_den_QCD800.root" );
  TFile* f1000 = TFile::Open( "fout_num_den_QCD1000.root" );
  TFile* f1400 = TFile::Open( "fout_num_den_QCD1400.root" );
  TFile* f1800 = TFile::Open( "fout_num_den_QCD1800.root" );
  TFile* f2400 = TFile::Open( "fout_num_den_QCD2400.root" ); 
  TFile* f3200 = TFile::Open( "fout_num_den_QCD3200.root" );
  
  std::vector<TH1D*> h_vec;
  h_vec.push_back( (TH1D*)f15->Get("h_gj_HT") );
  h_vec.push_back( (TH1D*)f30->Get("h_gj_HT") );
  h_vec.push_back( (TH1D*)f50->Get("h_gj_HT") );
  h_vec.push_back( (TH1D*)f80->Get("h_gj_HT") );
  h_vec.push_back( (TH1D*)f120->Get("h_gj_HT") );
  h_vec.push_back( (TH1D*)f170->Get("h_gj_HT") );
  h_vec.push_back( (TH1D*)f300->Get("h_gj_HT") );
  h_vec.push_back( (TH1D*)f470->Get("h_gj_HT") );
  h_vec.push_back( (TH1D*)f600->Get("h_gj_HT") );
  h_vec.push_back( (TH1D*)f800->Get("h_gj_HT") );
  h_vec.push_back( (TH1D*)f1000->Get("h_gj_HT") );
  h_vec.push_back( (TH1D*)f1400->Get("h_gj_HT") );
  h_vec.push_back( (TH1D*)f1800->Get("h_gj_HT") );
  h_vec.push_back( (TH1D*)f2400->Get("h_gj_HT") ); 
  h_vec.push_back( (TH1D*)f3200->Get("h_gj_HT") );

  THStack *hs = new THStack("hs", "");

  for(unsigned int i=0; i<h_vec.size(); i++){
    h_vec.at(i)->SetLineColor(i);
    h_vec.at(i)->SetFillStyle(1001);
    h_vec.at(i)->SetFillColor(i);
    hs->Add(h_vec.at(i));
  }

  //hs->GetXaxis()->SetTitle("HT [GeV]");

  TCanvas* c = new TCanvas("c", "c", 640, 480);
  hs->Draw("hist");
  gPad->SetLogy();
  gPad->Modified();
  c->SaveAs("plots/stitch.pdf");

}


void check_stitch_GJets(){
  
  TFile* f40 = TFile::Open( "fout_num_den_GJets40.root" );
  TFile* f100 = TFile::Open( "fout_num_den_GJets100.root" );
  TFile* f200 = TFile::Open( "fout_num_den_GJets200.root" );
  TFile* f400 = TFile::Open( "fout_num_den_GJets400.root" );
  TFile* f600 = TFile::Open( "fout_num_den_GJets600.root" );
  
  std::vector<TH1D*> h_vec;
  h_vec.push_back( (TH1D*)f40->Get("h_gj_NJetsNoPhotons") );
  h_vec.push_back( (TH1D*)f100->Get("h_gj_NJetsNoPhotons") );
  h_vec.push_back( (TH1D*)f200->Get("h_gj_NJetsNoPhotons") );
  h_vec.push_back( (TH1D*)f400->Get("h_gj_NJetsNoPhotons") );
  h_vec.push_back( (TH1D*)f600->Get("h_gj_NJetsNoPhotons") );

  THStack *hs = new THStack("hs", "");

  for(unsigned int i=0; i<h_vec.size(); i++){
    h_vec.at(i)->SetLineColor(i);
    h_vec.at(i)->SetFillStyle(1001);
    h_vec.at(i)->SetFillColor(i);
    hs->Add(h_vec.at(i));
  }

  //hs->GetXaxis()->SetTitle("HT [GeV]");

  TCanvas* c = new TCanvas("c", "c", 640, 480);
  hs->Draw("hist");
  gPad->SetLogy();
  gPad->Modified();
  c->SaveAs("plots/gjets_stitch.pdf");

}



void get_num_den(bool add_dy = true, TString gjets_name="", double g_pt_cut=10){
  setup_weights();

  TString cut_dy = "PTOSSF>10 && ONZ"; 
  //TString cut_dy = "NGOODLEPTONS==1 && MET>45"; 
  TString cut_gj = "NMEDIUMPHOTONS>0 && PT_MEDIUMPHOTONS[0]>";
  cut_gj+=g_pt_cut; //note cuts are written out in loop, too

  
  ///////////////////////////
  // Input and Output Files 
  ///////////////////////////

  cout << "Open DY" << endl;
  TString filename_dy = "input_files/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt";
  TFileCollection fc_dy("dum_dy", "", filename_dy);
  TChain* tree_dy = new TChain("treeR");
  tree_dy->AddFileInfoList((TCollection*)fc_dy.GetList());
  //TH1D* h_count_dy = (TH1D*)file_dy->Get("noCutSignature_COUNT");
  //tree_dy->SetWeight(1/h_count_dy->GetBinContent(1), "global");
  tree_dy->SetWeight(1.0, "global");

  cout << "Open GJets" << endl;
  TString filename_gj = get_file(gjets_name);
  TFileCollection fc_gj("dum_gj", "", filename_gj);
  TChain* tree_gj   = new TChain("treeR");
  tree_gj->AddFileInfoList((TCollection*)fc_gj.GetList());
  
  double xs_gj = get_xs(gjets_name);
  double count_gj = get_count(gjets_name);
  tree_gj->SetWeight(xs_gj/count_gj, "global");


  //numerator, denominator, and ratio
  std::vector<TH1D> vec_h_dy;
  std::vector<TH1D> vec_h_gj;

  ///////////////////////
  // Make Weight Histos
  ///////////////////////
  for(unsigned int i=0; i<my_weightings.size(); i++){

    plot_struct my_weighting = my_weightings.at(i);
    
    cout << "Making weight for " << my_weighting.name << endl;
    TH1D h_dy("h_dy_"+my_weighting.name, "h_dy_"+my_weighting.name, my_weighting.nbins, my_weighting.min, my_weighting.max);
    TH1D h_gj("h_gj_"+my_weighting.name, "h_gj_"+my_weighting.name, my_weighting.nbins, my_weighting.min, my_weighting.max);
    h_dy.Sumw2();
    h_gj.Sumw2();

    if(add_dy){
      tree_dy->Project("h_dy_"+my_weighting.name, my_weighting.var_dy, cut_dy);
    }
    tree_gj->Project("h_gj_"+my_weighting.name, my_weighting.var_gj, cut_gj);

    if(add_dy) vec_h_dy.push_back(h_dy);
    vec_h_gj.push_back(h_gj);
  }


  //////////////////////////////////////////////////////
  // DY Output Histos -- do these while we have DY open
  //////////////////////////////////////////////////////
  /*
  TH1D h_nTagsNoPhotons_dy("h_nTagsNoPhotons_dy", "h_nTagsNoPhotons_dy", out_nTags_nbins, out_nTags_min, out_nTags_max);
  h_nTagsNoPhotons_dy.Sumw2();
  if(add_dy) tree_dy->Project("h_nTagsNoPhotons_dy", "NINCLUSIVETAGGEDCALOJETSENOPHOTONS", cut_dy);//--no photons in these ntuples
  */
  
  TH1D h_nTags_dy("h_nTags_dy", "h_nTags_dy", out_nTags_nbins, out_nTags_min, out_nTags_max);
  h_nTags_dy.Sumw2();
  if(add_dy) tree_dy->Project("h_nTags_dy", "NINCLUSIVETAGGEDCALOJETSENOPHOTONS", cut_dy);
  
  /*
  TH1D h_nJetsNoPhotons_dy("h_nJetsNoPhotons_dy", "h_nJetsNoPhotons_dy", out_nJets_nbins, out_nJets_min, out_nJets_max);
  h_nJetsNoPhotons_dy.Sumw2();
  if(add_dy) tree_dy->Project("h_nJetsNoPhotons_dy", "NBASICCALOJETS1PT20NOPHOTONS", cut_dy);//--no photons in these ntuples
  */

  TH1D h_nJets_dy("h_nJets_dy", "h_nJets_dy", out_nJets_nbins, out_nJets_min, out_nJets_max);
  h_nJets_dy.Sumw2();
  if(add_dy) tree_dy->Project("h_nJets_dy", "NBASICCALOJETS1PT20NOPHOTONS", cut_dy);

  /*
  TH1D h_Jet1PTNoPhotons_dy("h_Jet1PTNoPhotons_dy", "h_Jet1PTNoPhotons_dy", out_JetPt_nbins, out_JetPt_min, out_JetPt_max);
  h_Jet1PTNoPhotons_dy.Sumw2();
  if(add_dy) tree_dy->Project("h_Jet1PTNoPhotons_dy", "PT_BASICCALOJETS1PT20NOPHOTONS[0]", cut_dy);//--no photons in these ntuples
  
  TH1D h_Jet1PT_dy("h_Jet1PT_dy", "h_Jet1PT_dy", out_JetPt_nbins, out_JetPt_min, out_JetPt_max);
  h_Jet1PT_dy.Sumw2();
  if(add_dy) tree_dy->Project("h_Jet1PT_dy", "PT_BASICCALOJETS1PT20[0]", cut_dy);
  
  TH1D h_HT_dy("h_HT_dy", "h_HT_dy", out_HT_nbins, out_HT_min, out_HT_max);
  h_HT_dy.Sumw2();
  if(add_dy) tree_dy->Project("h_HT_dy", "HT", cut_dy);

  TH1D h_PT_dy("h_PT_dy", "h_PT_dy", out_PT_nbins, out_PT_min, out_PT_max);
  h_PT_dy.Sumw2();
  if(add_dy) tree_dy->Project("h_PT_dy", "PTOSSF", cut_dy);

  TH1D h_VE_dy("h_VE_dy", "h_VE_dy", out_VE_nbins, out_VE_min, out_VE_max);
  h_VE_dy.Sumw2();
  if(add_dy) tree_dy->Project("h_VE_dy", "TMath::Sqrt((1+TMath::SinH(ETAOSSF)*TMath::SinH(ETAOSSF))*PTOSSF*PTOSSF+MOSSF*MOSSF)", cut_dy);
  */

  ////////////////////
  // Write
  ////////////////////
  cout << "Start writing output" << endl;

  TFile fout("fout_num_den_"+gjets_name+".root", "RECREATE"); 

  //input (i.e. numerator, denominator, and weight)
  for(unsigned int i=0; i<my_weightings.size(); i++){
    if(add_dy) vec_h_dy.at(i).Write();
    vec_h_gj.at(i).Write();
  }
  
  //output 
  if(add_dy){
    //h_nTagsNoPhotons_dy.Write();
    h_nTags_dy.Write();
    //h_nJetsNoPhotons_dy.Write();
    h_nJets_dy.Write();
    /*
      h_Jet1PTNoPhotons_dy.Write();
      h_Jet1PT_dy.Write();
      h_HT_dy.Write();
      h_PT_dy.Write();
      h_VE_dy.Write();
    */
  }

  fout.Close();

}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



void apply_weights(TString path = "", TString gjets_name = "", double g_pt_cut = 10){
  setup_weights();

  //numerator, denominator, and ratio
  std::vector<TH1D> vec_h_dy;
  std::vector<TH1D> vec_h_gj;
  std::vector<TH1D> vec_h_ratio;

  ///////////////////////
  // Get Weight Histos
  ///////////////////////
  TFile* file_hadded = 0;
  if(gjets_name.Contains("QCD")){
    file_hadded = TFile::Open("fout_num_den_hadded_QCD.root");  
  }
  else if(gjets_name.Contains("GJets")){
    file_hadded = TFile::Open("fout_num_den_hadded_GJets.root");  
  }

  for(unsigned int i=0; i<my_weightings.size(); i++){

    plot_struct my_weighting = my_weightings.at(i);

    TH1D h_dy = *(TH1D*)file_hadded->Get("h_dy_"+my_weighting.name);
    TH1D h_gj = *(TH1D*)file_hadded->Get("h_gj_"+my_weighting.name);

    TH1D h_r = *(TH1D*)h_dy.Clone("h_r_"+my_weighting.name);
    h_r.Divide(&h_gj);

    vec_h_dy.push_back(h_dy);
    vec_h_gj.push_back(h_gj);
    vec_h_ratio.push_back(h_r);
  }

  ////////////////////////
  // Weighted Histos
  ////////////////////////
  std::vector<TH1D> v_nTagsNoPhotons  = make_output_histos("nTagsNoPhotons", out_nTags_nbins, out_nTags_min, out_nTags_max, my_weightings);
  std::vector<TH1D> v_nTags           = make_output_histos("nTags", out_nTags_nbins, out_nTags_min, out_nTags_max, my_weightings);
  std::vector<TH1D> v_nJetsNoPhotons  = make_output_histos("nJetsNoPhotons", out_nJets_nbins, out_nJets_min, out_nJets_max, my_weightings);
  std::vector<TH1D> v_nJets           = make_output_histos("nJets", out_nJets_nbins, out_nJets_min, out_nJets_max, my_weightings);
  /*
    std::vector<TH1D> v_Jet1PTNoPhotons = make_output_histos("Jet1PTNoPhotons", out_JetPt_nbins, out_JetPt_min, out_JetPt_max, my_weightings);
    std::vector<TH1D> v_Jet1PT          = make_output_histos("Jet1PT", out_JetPt_nbins, out_JetPt_min, out_JetPt_max, my_weightings);
    std::vector<TH1D> v_HT              = make_output_histos("HT", out_HT_nbins, out_HT_min, out_HT_max, my_weightings);
    std::vector<TH1D> v_PT              = make_output_histos("PT", out_PT_nbins, out_PT_min, out_PT_max, my_weightings);
    std::vector<TH1D> v_VE              = make_output_histos("VE", out_VE_nbins, out_VE_min, out_VE_max, my_weightings);
  */

  ///////////////////////
  // Setup TTree
  ///////////////////////
  TFile* fin = TFile::Open(path+gjets_name);
  TTree* tree_gj = (TTree*)fin->Get("treeR");
  tree_gj->SetWeight(1.0);

  double xs_gj = get_xs(gjets_name);
  double count_gj = get_count(gjets_name);
  double weight_gs = xs_gj/count_gj;


  std::vector<int>    *b_NBASICCALOJETS1PT20                   = new std::vector<int>();
  std::vector<int>    *b_NBASICCALOJETS1PT20NOPHOTONS          = new std::vector<int>();
  std::vector<int>    *b_NINCLUSIVETAGGEDCALOJETSE             = new std::vector<int>();
  std::vector<int>    *b_NINCLUSIVETAGGEDCALOJETSENOPHOTONS    = new std::vector<int>();
  std::vector<int>    *b_NMEDIUMPHOTONS                        = new std::vector<int>();
  std::vector<double> *b_HT                                    = new std::vector<double>();
  std::vector<double> *b_PTOSSF                                = new std::vector<double>();
  std::vector<double> *b_ETAOSSF                               = new std::vector<double>();
  std::vector<double> *b_MOSSF                                 = new std::vector<double>();
  std::vector<double> *b_PT_MEDIUMPHOTONS                      = new std::vector<double>();
  std::vector<double> *b_PT_BASICCALOJETS1PT20                 = new std::vector<double>();
  std::vector<double> *b_PT_BASICCALOJETS1PT20NOPHOTONS        = new std::vector<double>();
  std::vector<double> *b_PT_INCLUSIVETAGGEDCALOJETSE           = new std::vector<double>();
  std::vector<double> *b_PT_INCLUSIVETAGGEDCALOJETSENOPHOTONS  = new std::vector<double>();
  std::vector<double> *b_ETA_MEDIUMPHOTONS                     = new std::vector<double>();
  std::vector<double> *b_ETA_BASICCALOJETS1PT20                = new std::vector<double>();
  std::vector<double> *b_ETA_BASICCALOJETS1PT20NOPHOTONS       = new std::vector<double>();
  std::vector<double> *b_ETA_INCLUSIVETAGGEDCALOJETSE          = new std::vector<double>();
  std::vector<double> *b_ETA_INCLUSIVETAGGEDCALOJETSENOPHOTONS = new std::vector<double>();
  tree_gj->SetBranchAddress("NBASICCALOJETS1PT20",                   &b_NBASICCALOJETS1PT20);  
  tree_gj->SetBranchAddress("NBASICCALOJETS1PT20NOPHOTONS",          &b_NBASICCALOJETS1PT20NOPHOTONS);  
  tree_gj->SetBranchAddress("NINCLUSIVETAGGEDCALOJETSE",             &b_NINCLUSIVETAGGEDCALOJETSE);  
  tree_gj->SetBranchAddress("NINCLUSIVETAGGEDCALOJETSENOPHOTONS",    &b_NINCLUSIVETAGGEDCALOJETSENOPHOTONS);  
  tree_gj->SetBranchAddress("NMEDIUMPHOTONS",                        &b_NMEDIUMPHOTONS);  
  tree_gj->SetBranchAddress("HT",                                    &b_HT);
  tree_gj->SetBranchAddress("PTOSSF",                                &b_PTOSSF);  
  tree_gj->SetBranchAddress("ETAOSSF",                               &b_ETAOSSF);  
  tree_gj->SetBranchAddress("MOSSF",                                 &b_MOSSF);  
  tree_gj->SetBranchAddress("PT_MEDIUMPHOTONS",                      &b_PT_MEDIUMPHOTONS);  
  tree_gj->SetBranchAddress("PT_BASICCALOJETS1PT20",                 &b_PT_BASICCALOJETS1PT20);  
  tree_gj->SetBranchAddress("PT_BASICCALOJETS1PT20NOPHOTONS",        &b_PT_BASICCALOJETS1PT20NOPHOTONS);  
  tree_gj->SetBranchAddress("PT_INCLUSIVETAGGEDCALOJETSE",           &b_PT_INCLUSIVETAGGEDCALOJETSE);  
  tree_gj->SetBranchAddress("PT_INCLUSIVETAGGEDCALOJETSENOPHOTONS",  &b_PT_INCLUSIVETAGGEDCALOJETSENOPHOTONS);  
  tree_gj->SetBranchAddress("ETA_MEDIUMPHOTONS",                     &b_ETA_MEDIUMPHOTONS);  
  tree_gj->SetBranchAddress("ETA_BASICCALOJETS1PT20",                &b_ETA_BASICCALOJETS1PT20);  
  tree_gj->SetBranchAddress("ETA_BASICCALOJETS1PT20NOPHOTONS",       &b_ETA_BASICCALOJETS1PT20NOPHOTONS);  
  tree_gj->SetBranchAddress("ETA_INCLUSIVETAGGEDCALOJETSE",          &b_ETA_INCLUSIVETAGGEDCALOJETSE);  
  tree_gj->SetBranchAddress("ETA_INCLUSIVETAGGEDCALOJETSENOPHOTONS", &b_ETA_INCLUSIVETAGGEDCALOJETSENOPHOTONS);  


  ///////////////////////
  // Event Loop
  ///////////////////////

  cout << "Starting event loop" << endl;
  for(unsigned int i = 0; i<tree_gj->GetEntries(); i++) {

    if(i%10000 == 0) cout << "Event " << i << "/" << tree_gj->GetEntries() << endl;
    tree_gj->GetEntry(i);

    //if(b_NMEDIUMPHOTONS->at(0)<1) continue;
    if(b_NMEDIUMPHOTONS->at(0)<1 || b_PT_MEDIUMPHOTONS->at(0)<=g_pt_cut) continue;
    
    double energy = TMath::Sqrt((1+TMath::SinH(b_ETA_MEDIUMPHOTONS->at(0))*TMath::SinH(b_ETA_MEDIUMPHOTONS->at(0)))*b_PT_MEDIUMPHOTONS->at(0)*b_PT_MEDIUMPHOTONS->at(0)); 

    std::vector<double> v_weights;
    //v_weights.push_back(vec_h_ratio.at(0).GetBinContent( vec_h_ratio.at(0).FindBin(b_HT->at(0)) ) );
    //v_weights.push_back(vec_h_ratio.at(1).GetBinContent( vec_h_ratio.at(1).FindBin(b_NBASICCALOJETS1PT20->at(0)) ) );
    v_weights.push_back(vec_h_ratio.at(0).GetBinContent( vec_h_ratio.at(0).FindBin(b_NBASICCALOJETS1PT20NOPHOTONS->at(0)) ) );
    //v_weights.push_back(vec_h_ratio.at(3).GetBinContent( vec_h_ratio.at(3).FindBin(energy) ) );
    //v_weights.push_back(vec_h_ratio.at(4).GetBinContent( vec_h_ratio.at(4).FindBin(b_PT_MEDIUMPHOTONS->at(0)) ) ); 
    
    //Fill
    fill_histo_vector( v_nTagsNoPhotons,  v_weights, b_NINCLUSIVETAGGEDCALOJETSENOPHOTONS->at(0), weight_gs );
    fill_histo_vector( v_nTags,           v_weights, b_NINCLUSIVETAGGEDCALOJETSE->at(0), weight_gs );
    fill_histo_vector( v_nJetsNoPhotons,  v_weights, b_NBASICCALOJETS1PT20NOPHOTONS->at(0), weight_gs );
    fill_histo_vector( v_nJets,           v_weights, b_NBASICCALOJETS1PT20->at(0), weight_gs );
    /*
    fill_histo_vector( v_HT,              v_weights, b_HT->at(0), weight_gs );
    fill_histo_vector( v_PT,              v_weights, b_PT_MEDIUMPHOTONS->at(0), weight_gs );
    fill_histo_vector( v_VE,              v_weights, xxx, weight_gs );

    if(b_NBASICCALOJETS1PT20NOPHOTONS->at(0)>0) fill_histo_vector( v_Jet1PTNoPhotons, v_weights, b_PT_BASICCALOJETS1PT20NOPHOTONS->at(0), weight_gs );
    if(b_NBASICCALOJETS1PT20->at(0)>0)          fill_histo_vector( v_Jet1PT,          v_weights, b_PT_BASICCALOJETS1PT20->at(0), weight_gs );
    */

  }
  
  fin->Close();

  ////////////////////
  // Write
  ////////////////////
  cout << "Start writing output" << endl;

  TFile fout("fout_weighted_"+gjets_name+".root", "RECREATE"); 

  for(unsigned int i=0; i<vec_h_ratio.size(); i++){
    vec_h_ratio.at(i).Write();
  }

  //output 
  write_vector(v_nTagsNoPhotons);
  write_vector(v_nTags);
  write_vector(v_nJetsNoPhotons);
  write_vector(v_nJets);
  /*
    write_vector(v_Jet1PTNoPhotons);
    write_vector(v_Jet1PT);
    write_vector(v_HT);
    write_vector(v_PT);
    write_vector(v_VE);
  */

  fout.Close();

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void make_plots(TString path = "", TString process = "GJets"){
  setup_weights();

  ////////////////////////
  // Weighted Histos
  ////////////////////////
  TFile* file_gj = 0;
  TFile* file_dy = 0;
  if(process == "GJets"){
    file_gj = TFile::Open(path+"/fout_weighted_hadded_GJets.root", "READ");  
    file_dy = TFile::Open(path+"/fout_num_den_hadded_GJets.root", "READ");
  }
  else if(process == "QCD"){
    file_gj = TFile::Open("fout_weighted_hadded_QCD.root", "READ");  
    file_dy = TFile::Open("fout_num_den_hadded_QCD.root", "READ");
  }


  std::vector<TH1D> v_nTagsNoPhotons  = get_histos(file_gj, "nTagsNoPhotons", my_weightings);
  std::vector<TH1D> v_nTags           = get_histos(file_gj, "nTags", my_weightings);
  std::vector<TH1D> v_nJetsNoPhotons  = get_histos(file_gj, "nJetsNoPhotons", my_weightings);
  std::vector<TH1D> v_nJets           = get_histos(file_gj, "nJets", my_weightings);
  /*
    std::vector<TH1D> v_Jet1PTNoPhotons = get_histos(file_gj, "Jet1PTNoPhotons", my_weightings);
    std::vector<TH1D> v_Jet1PT          = get_histos(file_gj, "Jet1PT", my_weightings);
    std::vector<TH1D> v_HT              = get_histos(file_gj, "HT", my_weightings);
    std::vector<TH1D> v_PT              = get_histos(file_gj, "PT", my_weightings);
    std::vector<TH1D> v_VE              = get_histos(file_gj, "VE", my_weightings);
  */

  //TH1D* h_nTagsNoPhotons_dy  = (TH1D*)file_dy->Get("h_nTagsNoPhotons_dy");
  TH1D* h_nTags_dy           = (TH1D*)file_dy->Get("h_nTags_dy");
  //TH1D* h_nJetsNoPhotons_dy  = (TH1D*)file_dy->Get("h_nJetsNoPhotons_dy");
  TH1D* h_nJets_dy           = (TH1D*)file_dy->Get("h_nJets_dy");
  /*
    TH1D* h_Jet1PTNoPhotons_dy = (TH1D*)file_dy->Get("h_Jet1PTNoPhotons_dy");
    TH1D* h_Jet1PT_dy          = (TH1D*)file_dy->Get("h_Jet1PT_dy");
    TH1D* h_HT_dy              = (TH1D*)file_dy->Get("h_HT_dy");
    TH1D* h_PT_dy              = (TH1D*)file_dy->Get("h_PT_dy");
    TH1D* h_VE_dy              = (TH1D*)file_dy->Get("h_VE_dy");
  */

  /////////////////
  // Normalize
  /////////////////
  //unit_normalize(h_nTagsNoPhotons_dy);
  unit_normalize(h_nTags_dy);
  //unit_normalize(h_nJetsNoPhotons_dy);
  unit_normalize(h_nJets_dy);
  /*
    unit_normalize(h_Jet1PTNoPhotons_dy);
    unit_normalize(h_Jet1PT_dy);
    unit_normalize(h_HT_dy);
    unit_normalize(h_PT_dy);
    unit_normalize(h_VE_dy);
  */
  unit_normalize_vector(v_nTagsNoPhotons);
  unit_normalize_vector(v_nTags);
  unit_normalize_vector(v_nJetsNoPhotons);
  unit_normalize_vector(v_nJets);
  /*
    unit_normalize_vector(v_Jet1PTNoPhotons);
    unit_normalize_vector(v_Jet1PT);
    unit_normalize_vector(v_HT);
    unit_normalize_vector(v_PT);
    unit_normalize_vector(v_VE);
  */

  /////////////////////
  // P L  O    T      S
  /////////////////////
  for(unsigned int i=0; i<v_nTagsNoPhotons.size(); i++){

    TString w = "";
    if(i==0) w = "unweighted";
    if(i==1) w = my_weightings.at(0).name;
    if(i==2) w = my_weightings.at(1).name;
    if(i==3) w = my_weightings.at(2).name;
    if(i==4) w = my_weightings.at(3).name;
    if(i==5) w = my_weightings.at(4).name;

    draw_histos(&v_nTagsNoPhotons.at(i), h_nTags_dy, "nTags_GJets-NoPhotons", w);
    draw_histos(&v_nTags.at(i), h_nTags_dy, "nTags", w);
    draw_histos(&v_nJetsNoPhotons.at(i), h_nJets_dy, "nJets_GJets-NoPhotons", w);
    draw_histos(&v_nJets.at(i), h_nJets_dy, "nJets", w);
    /*
      draw_histos(&v_Jet1PTNoPhotons.at(i), h_Jet1PTNoPhotons_dy, "Jet1PTNoPhotons", w);
      draw_histos(&v_Jet1PT.at(i), h_Jet1PT_dy, "Jet1PT", w);
      draw_histos(&v_HT.at(i), h_HT_dy, "HT", w);
      draw_histos(&v_PT.at(i), h_PT_dy, "PT", w);
      draw_histos(&v_VE.at(i), h_VE_dy, "VE", w);
    */
  }

  ////////////////////
  // Write
  ////////////////////
  //nothing atm

}


void test_function(TString arg){
  cout << "test function: " << arg << endl;
}

void ReweightGJets(TString path, TString filename, TString arg, TString step, int addDY, double g_pt_cut){
  
  //TFile *ftest = TFile::Open(arg+".root","RECREATE");
  //ftest->Close();
  //test_function(arg);
  
  if(step == "num_den"){
    get_num_den(addDY, arg, g_pt_cut);
  }
  else if(step == "apply_weights"){
    apply_weights(path, filename, g_pt_cut); //needs weight file from previous step
  }
  

}
