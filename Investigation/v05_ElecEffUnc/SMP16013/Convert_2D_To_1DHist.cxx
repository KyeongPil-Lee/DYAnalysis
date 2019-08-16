#include <Include/SimplePlotTools.h>

class Converter
{
public:
  TString type_;

  TH2D* h_2D_;

  Int_t nPtBin_;
  Int_t nEtaBin_;
  vector<Double_t> vec_ptBinEdges_;
  vector<Double_t> vec_etaBinEdges_;

  Converter(TString type)
  {
    type_ = type;
    Init();
  }

  void ConvertAndSave( TFile *f_output )
  {
    for(Int_t i_pt=0; i_pt<nPtBin_; i_pt++)
    {
      Int_t i_ptBin = i_pt+1;

      TString histName = "";
      if( type_ == "signal" ) histName = TString::Format("h_relUnc_sgnChange_pt%.0lfto%.0lf", vec_ptBinEdges_[i_pt], vec_ptBinEdges_[i_pt+1]);
      if( type_ == "bkg" )    histName = TString::Format("h_relUnc_bkgChange_pt%.0lfto%.0lf", vec_ptBinEdges_[i_pt], vec_ptBinEdges_[i_pt+1]);

      TH1D* h_temp = MakeTH1D_FromVector(histName, vec_etaBinEdges_);

      for(Int_t i_eta=0; i_eta<nEtaBin_; i_eta++)
      {
        Int_t i_etaBin = i_eta+1;

        Double_t unc = h_2D_->GetBinContent(i_etaBin, i_ptBin) - 1.0;
        if( vec_etaBinEdges_[i_eta] == -1.566 || vec_etaBinEdges_[i_eta] == 1.4442 ) unc = 0; // -- ECAL gap: meaningless values (not used in the analysis)

        h_temp->SetBinContent(i_etaBin, unc);
        h_temp->SetBinError(i_etaBin, 0);
      }

      f_output->cd();
      h_temp->Write();

      delete h_temp;
    }
  }

private:
  TH1D* MakeTH1D_FromVector(TString histName, vector<Double_t> vec_binEdge)
  {
    const Int_t nBin = (Int_t)vec_binEdge.size() - 1;
    Double_t *arr_binEdge = new Double_t[nBin+1];

    for(Int_t i=0; i<nBin+1; i++)
      arr_binEdge[i] = vec_binEdge[i];

    TH1D* h_temp = new TH1D(histName, "", nBin, arr_binEdge);

    delete[] arr_binEdge;

    return h_temp;
  }

  void Init()
  {
    TString fileName = "";
    if( type_ == "signal" )      fileName = "EleGsfSelSigSys.root";
    else if( type_ == "bkg" )    fileName = "EleGsfSelBkgSys.root";
    else
    {
      cout << "type: " << type_ << " is not recognizable" << endl;
      return;
    }

    h_2D_ = PlotTool::Get_Hist2D(fileName, "h");

    vector<Double_t> vec_ptBinEdges_temp = {25., 40., 55, 8000.};
    vec_ptBinEdges_ = vec_ptBinEdges_temp;

    vector<Double_t> vec_etaBinEdges_temp = {-2.5, -2.0, -1.566, -1.4442, -1., -0.5, 0, 0.5, 1., 1.4442, 1.566, 2.0, 2.5};
    vec_etaBinEdges_ = vec_etaBinEdges_temp;

    nPtBin_  = (Int_t)vec_ptBinEdges_.size() - 1;
    nEtaBin_ = (Int_t)vec_etaBinEdges_.size() - 1;
  }

};

void Convert_2D_To_1DHist()
{
  TFile *f_output = TFile::Open("ROOTFile_SMP16013_EffUnc_eChannel.root", "RECREATE");
  Converter *converter_signal = new Converter("signal");
  converter_signal->ConvertAndSave(f_output);

  Converter *converter_bkg = new Converter("bkg");
  converter_bkg->ConvertAndSave(f_output);

  f_output->Close();

}