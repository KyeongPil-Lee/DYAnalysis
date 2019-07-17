{

  // ------------------------------------------------------- Total Stat+Sys Error -------------------------------------------------------- //
  TString analyzerPath = gSystem->Getenv("KP_ANALYZER_PATH");
  TFile *file1 = TFile::Open(analyzerPath+"/ElecChannel/CentralValue/Fiducial/FinalCorr_postFSR_v11.root");

  TFile *f1 = TFile::Open("RelUnc_BkgEst.root");
  //TFile *f2 = TFile::Open("/home/ridhi/Work/Analysis/76X/Systematics/Efficiency_TnP/v5_28Aug/RelUnc_EffSF.root");
  TFile *f2 = TFile::Open("ROOTFile_SystUnc_TagProbeMethod_ElecChannel_May2019.root");
  TFile *f3 = TFile::Open("RelUnc_detUnfold.root");
  TFile *f4 = TFile::Open("RelUnc_Smear.root");
  TFile *f5 = TFile::Open("RelUnc_StatError.root");
  TFile *f6 = TFile::Open("ROOTFile_TheoryUnc_aMCNLO.root");

  TFile *file2 = TFile::Open("FinalCorr_postFSR_v12_May2019.root","RECREATE");

  double lumi = 2.3;

  TH1D *h_Xsec1  = (TH1D*)file1->Get("h_xSec_dM_FSRCorr");

  TH1D *h_1 = (TH1D*)f1->Get("RelUnc_Total_BkgEst");
  //TH1D *h_2 = (TH1D*)f2->Get("RelUnc_Total_EffSF");
  TH1D *h_2 = (TH1D*)f2->Get("h_relUnc_tot");
  TH1D *h_3 = (TH1D*)f3->Get("RelUnc_detUnfold");
  TH1D *h_4 = (TH1D*)f4->Get("RelUnc_Total_Smear");
  TH1D *h_5 = (TH1D*)f5->Get("RelUnc_Stat");
  TH1D *h_6 = (TH1D*)file1->Get("h_xSec_dM_FSRCorr_aMC@NLO");
  TH1D *h_7 = (TH1D*)f6->Get("Electron_withinAcc/h_RelUnc_Tot");

  TH1D *h_Xsec2  = (TH1D*)h_Xsec1->Clone();

  TH1D *h_StatUnc  = (TH1D*)h_Xsec1->Clone();
  TH1D *h_SystUnc  = (TH1D*)h_Xsec1->Clone();
  TH1D *h_Total1Unc = (TH1D*)h_Xsec1->Clone();	// Stat+Syst
  TH1D *h_Total2Unc = (TH1D*)h_Xsec1->Clone();	// Theo
  TH1D *h_Total3Unc = (TH1D*)h_Xsec1->Clone();	// Stat+Syst+Theo

  TH1D *h_Bkg_bfrSmooth = (TH1D*)h_1->Clone();
  TH1D *h_Bkg_aftSmooth = (TH1D*)h_1->Clone();

  TH1D *h_Unf_bfrSmooth = (TH1D*)h_3->Clone();
  TH1D *h_Unf_aftSmooth = (TH1D*)h_3->Clone();

  TH1D *h_Smr_bfrSmooth = (TH1D*)h_4->Clone();
  TH1D *h_Smr_aftSmooth = (TH1D*)h_4->Clone();

  for(int i=1; i<44; i++){

    double Nbkg = h_1->GetBinContent(i);

    h_Bkg_bfrSmooth->SetBinContent(i,Nbkg);
    h_Bkg_bfrSmooth->SetBinError(i,0);

    h_Bkg_aftSmooth->SetBinContent(i,Nbkg);
    h_Bkg_aftSmooth->SetBinError(i,0);

    double Nunf = h_3->GetBinContent(i);

    h_Unf_bfrSmooth->SetBinContent(i,Nunf);
    h_Unf_bfrSmooth->SetBinError(i,0);

    h_Unf_aftSmooth->SetBinContent(i,Nunf);
    h_Unf_aftSmooth->SetBinError(i,0);

    double Nsmr = h_4->GetBinContent(i);

    h_Smr_bfrSmooth->SetBinContent(i,Nsmr);
    h_Smr_bfrSmooth->SetBinError(i,0);

    h_Smr_aftSmooth->SetBinContent(i,Nsmr);
    h_Smr_aftSmooth->SetBinError(i,0);

    // Theoretical Xsec
    double mcnlo = h_6->GetBinContent(i);
    double err   = h_7->GetBinContent(i)*mcnlo;
    double scale = (0.03*mcnlo);

    double total = sqrt(err*err + scale*scale);

    //cout<<"MCNLO = "<<mcnlo<<"   Before = "<<err<<"   scale = "<<scale<<"   After = "<<total<<endl;
    //cout<<""<<endl;

    h_6->SetBinError(i, total);

  }

  h_Bkg_aftSmooth->Smooth();
  h_Unf_aftSmooth->Smooth();
  h_Smr_aftSmooth->Smooth();

  for(int i=1; i<44; i++){
    double low  = h_1->GetBinLowEdge(i);
    double high = h_1->GetBinLowEdge(i+1);

    double Nxsec  = h_Xsec1->GetBinContent(i);
    double Exsec  = h_Xsec1->GetBinError(i);
    double Rxsec = (Exsec*100)/Nxsec;

    double Rstat = h_5->GetBinContent(i);
    double Estat = (Rstat*Nxsec)/100;

    double Nbkg1 = h_Bkg_bfrSmooth->GetBinContent(i);
    double Nbkg2 = h_Bkg_aftSmooth->GetBinContent(i);

    double Nunf1 = h_Unf_bfrSmooth->GetBinContent(i);
    double Nunf2 = h_Unf_aftSmooth->GetBinContent(i);

    double Nsmr1 = h_Smr_bfrSmooth->GetBinContent(i);
    double Nsmr2 = h_Smr_aftSmooth->GetBinContent(i);

    double Nsf = h_2->GetBinContent(i);

    double RelUnf1 = sqrt(Nunf1*Nunf1 + Nsmr1*Nsmr1);
    double RelUnf2 = sqrt(Nunf2*Nunf2 + Nsmr2*Nsmr2);

    double AbsBkg_xsec  = (Nbkg2*Nxsec)/100;
    double AbsSF_xsec   = (Nsf*Nxsec)/100;
    double AbsUnf_xsec  = (Nunf2*Nxsec)/100;
    double AbsSmr_xsec  = (Nsmr2*Nxsec)/100;
    double AbsLumi_xsec = (lumi*Nxsec)/100;

    double theo = (h_6->GetBinError(i)/h_6->GetBinContent(i)) * 100;

    //cout<<"theo error:    mcnlo = "<<theo<<endl;

    double AbsSyst =  sqrt(AbsBkg_xsec*AbsBkg_xsec + AbsSF_xsec*AbsSF_xsec + AbsUnf_xsec*AbsUnf_xsec + AbsSmr_xsec*AbsSmr_xsec + AbsLumi_xsec*AbsLumi_xsec);
    double AbsTotal = sqrt(Estat*Estat + AbsBkg_xsec*AbsBkg_xsec + AbsSF_xsec*AbsSF_xsec + AbsUnf_xsec*AbsUnf_xsec + AbsSmr_xsec*AbsSmr_xsec + AbsLumi_xsec*AbsLumi_xsec);

    double Stat = sqrt(Rstat*Rstat);
   // Total rel. systematic error (Smoothing applied) -> syst.+theo // Without lumi unc.
    double Syst = sqrt(Nbkg2*Nbkg2 + Nsf*Nsf + Nunf2*Nunf2 + Nsmr2*Nsmr2);
    double Mcnlo = sqrt(theo*theo);

    double Total1 = sqrt(Rstat*Rstat + Nbkg2*Nbkg2 + Nsf*Nsf + Nunf2*Nunf2 + Nsmr2*Nsmr2 + lumi*lumi);			// Stat+Syst
    double Total2 = sqrt(Mcnlo*Mcnlo);											// Theo
    double Total3 = sqrt(Rstat*Rstat + Nbkg2*Nbkg2 + Nsf*Nsf + Nunf2*Nunf2 + Nsmr2*Nsmr2 + lumi*lumi + Mcnlo*Mcnlo);	// Stat+Syst+Theo

    //cout<<low<<"   "<<high<<endl;
    //printf("%.1e\n", Nxsec);
    //printf("%.1e\n", Estat);
    //printf("%.1e\n", AbsSyst);
    //printf("%.1e\n", AbsTotal);

    //cout<<"Xsec: "<<Nxsec<<endl;
    //cout<<"Stat error on xsec:  "<<Estat<<endl;
    //cout<<"Syst error on xsec:  "<<AbsSyst<<endl;
    //cout<<"Total error on xsec: "<<AbsTotal<<endl;
    //cout<<""<<endl;

    h_Xsec1->SetBinError(i, Estat);
    h_Xsec2->SetBinError(i, AbsTotal);

    h_StatUnc->SetBinContent(i, Stat);
    h_StatUnc->SetBinError(i, 0);

    h_SystUnc->SetBinContent(i, Syst);
    h_SystUnc->SetBinError(i, 0);

    h_Total1Unc->SetBinContent(i, Total1);
    h_Total1Unc->SetBinError(i, 0);

    h_Total2Unc->SetBinContent(i, Total2);
    h_Total2Unc->SetBinError(i, 0);

    h_Total3Unc->SetBinContent(i, Total3);
    h_Total3Unc->SetBinError(i, 0);
  }
  
  h_Xsec1->Write("h_xSec_dM_FSRCorr_v1");
  h_Xsec2->Write("h_xSec_dM_FSRCorr_v2");

  h_StatUnc->Write("h_RelUnc_Stat");
  h_SystUnc->Write("h_RelUnc_Syst");
  h_Total1Unc->Write("h_RelUnc_Total1");
  h_Total2Unc->Write("h_RelUnc_Total2");
  h_Total3Unc->Write("h_RelUnc_Total3");

  h_6->Write("h_diffXsec_MCNLO");

  file2->Close();
}
