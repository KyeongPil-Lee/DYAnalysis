#include <ElecChannel/Include/DYElectronTool.h>

using namespace RooFit;

class UncEstimationTool
{
public:
  TString uncType_;

  TH1D* h_dXSec_cv_;

  Int_t nSmeared_;
  vector<TH1D*> vec_dXSec_smeared_;

  TH1D* h_mean_;
  TH1D* h_variance_;


  UncEstimationTool(TString uncType)
  {
    uncType_ = uncType;
  }

  void Set_DXSec_CV(TH1D* h_dXSec_cv) { h_dXSec_cv_ = h_dXSec_cv; }
  void Set_DXSec_Smeared(vector<TH1D*> vec_dXSec_smeared) { vec_dXSec_smeared_ = vec_dXSec_smeared; }

  void Estimate()
  {
    Init();
    GaussianFit();
    Save();
  }

private:
  void GaussianFit()
  {
    Int_t nBin = h_dXSec_cv_->GetNbinsX();
    for(Int_t i=0; i<nBin; i++)
    {
      Int_t i_bin = i+1;

      TTree *tree = makeTTree(i_bin);

      RooRealVar relDiff("relDiff","(#sigma_{Smeared} - #sigma_{Central}) / #sigma_{Central}", -0.1, 0.1);
      RooDataSet data("data","data", tree, relDiff); // -- Name of the variable should be same with the branch name in the tree -- //

      Double_t lowerEdge = h_dXSec_cv_->GetBinLowEdge(i_bin);
      Double_t upperEdge = h_dXSec_cv_->GetBinLowEdge(i_bin+1);

      // Make plot of binned dataset showing Poisson error bars (RooFit default)
      RooPlot* frame = relDiff.frame(Title(TString::Format("Rel. diff. (#sigma_{central}, #sigma_{smeared}) in %dth mass bin (%.0lf < M < %.0lf GeV)", i_bin, lowerEdge, upperEdge)) );
      
      // Fit a Gaussian p.d.f to the data
      RooRealVar mean("mean", "mean", 0, -0.5, 0.5) ;
      RooRealVar sigma("sigma", "sigma", 0.01, 0.0001, 1);
      RooGaussian gauss("gauss", "gauss", relDiff, mean, sigma);
      gauss.fitTo(data);

      data.plotOn(frame, Binning(50));
      gauss.plotOn(frame);

      TString canvasName = "c_relDiff_" + uncType_ + TString::Format("_%d", i_bin) + "bin";

      TCanvas *c = new TCanvas(canvasName, "", 800, 800); c->cd();
      frame->Draw();
      c->SaveAs(canvasName+".pdf");

      Double_t value_mean = mean.getVal();
      h_mean_->SetBinContent(i_bin, value_mean);
      h_mean_->SetBinError(i_bin, 0);

      Double_t value_var = sigma.getVal();
      h_variance_->SetBinContent(i_bin, value_var);
      h_variance_->SetBinError(i_bin, 0); // -- how to get the error of the sigma? -- //
    }
  }

  void Save()
  {
    TString fileName_output = "ROOTFile_UncFromGaussianFit_"+uncType_+".root";
    TFile *f_output = TFile::Open(fileName_output, "RECREATE");
    f_output->cd();

    h_mean_->Write();
    h_variance_->Write();

    f_output->Close();

    cout << "[UncEstimationTool::Save] All results are saved in " << fileName_output << endl;
  }

  void Init()
  {
    nSmeared_ = (Int_t)vec_dXSec_smeared_.size();

    h_mean_   = DYTool::MakeHist_DXSecBin("h_mean_"+uncType_);
    h_variance_ = DYTool::MakeHist_DXSecBin("h_variance_"+uncType_);
  }

  // -- tree for unbinned likelihood fit
  TTree* makeTTree(Int_t i_bin) 
  {
    TTree* tree = new TTree("tree","tree");

    Double_t* relDiff = new Double_t;

    tree->Branch("relDiff", relDiff, "relDiff/D");

    Double_t value_cv = h_dXSec_cv_->GetBinContent(i_bin);

    for(Int_t i=0; i<nSmeared_; i++)
    {
      Double_t value_smeared = vec_dXSec_smeared_[i]->GetBinContent(i_bin);
      *relDiff = ( value_smeared - value_cv ) / value_cv;
      // printf("[\t%d th relDiff = %lf]\n", i, *relDiff);

      tree->Fill();
    }

    return tree;
  }

};

void EstimateUnc_GivenUncType(TString uncType)
{
  TH1D* h_dXSec_cv = nullptr;
  vector<TH1D*> vec_dXSec_smeared;

  TString analyzerPath   = gSystem->Getenv("KP_ANALYZER_PATH");
  TString fileName_dXSec = analyzerPath+"/ElecChannel/CovMatrix/EfficiencySF/ROOTFile_SmearedDXSecProducer_"+uncType+".root";
  TString histName_dXSec_cv = "h_dXSec_cv";

  h_dXSec_cv = PlotTool::Get_Hist(fileName_dXSec, histName_dXSec_cv);
  const Int_t nEffMap = 500;
  for(Int_t i=0; i<nEffMap; i++)
  {
    TString histName_dXSec_smeared = TString::Format("h_dXSec_smeared_%d", i);
    TH1D* h_dXSec_smeared = PlotTool::Get_Hist(fileName_dXSec, histName_dXSec_smeared);
    vec_dXSec_smeared.push_back( h_dXSec_smeared );
  }

  UncEstimationTool* tool = new UncEstimationTool(uncType);
  tool->Set_DXSec_CV(h_dXSec_cv);
  tool->Set_DXSec_Smeared(vec_dXSec_smeared);
  tool->Estimate();
}

void EstimateUnc_GaussianFit()
{
  EstimateUnc_GivenUncType("bkgChange");
  EstimateUnc_GivenUncType("sgnChange");
  EstimateUnc_GivenUncType("tagChange");
  EstimateUnc_GivenUncType("nlo");
  EstimateUnc_GivenUncType("stat");
}