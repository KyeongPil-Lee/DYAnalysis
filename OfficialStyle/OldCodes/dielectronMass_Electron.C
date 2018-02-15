void dielectronMass_Electron()
{
   TCanvas *c1 = new TCanvas("c1", "",0,0, 800, 700);
   gStyle->SetOptFit(1);
   gStyle->SetOptStat(0);
   gStyle->SetOptTitle(0);
   c1->Range(0,0,1,1);
   c1->SetFillColor(0);
   c1->SetBorderMode(0);
   c1->SetBorderSize(2);
   //c1->SetGridx();
   //c1->SetGridy();
   c1->SetTickx(1);
   c1->SetTicky(1);
   c1->SetLeftMargin(0.16);
   c1->SetRightMargin(0.02);
   c1->SetTopMargin(0.08);
   c1->SetBottomMargin(0.14);
   c1->SetFrameFillStyle(0);
   c1->SetFrameBorderMode(0);
  
// ------------>Primitives in pad: TopPad
   TPad *TopPad = new TPad("TopPad", "TopPad",0.01,0.01,0.99,0.99);
   TopPad->Draw();
   TopPad->cd();
   TopPad->Range(0.7100599,-11.71078,3.622756,3.443786);
   TopPad->SetFillColor(0);
   TopPad->SetBorderMode(0);
   TopPad->SetBorderSize(2);
   TopPad->SetLogx();
   TopPad->SetLogy();
   //TopPad->SetGridx();
   //TopPad->SetGridy();
   TopPad->SetTickx(1);
   TopPad->SetTicky(1);
   TopPad->SetLeftMargin(0.13);
   TopPad->SetRightMargin(0.05);
   TopPad->SetTopMargin(0.05);
   TopPad->SetBottomMargin(0.32);
   TopPad->SetFrameFillStyle(0);
   TopPad->SetFrameBorderMode(0);
   TopPad->SetFrameFillStyle(0);
   TopPad->SetFrameBorderMode(0);

   TFile* f1 = new TFile("ROOTFile_Histograms_DielectronMassSpectrum.root");
   f1->cd();
   Double_t xAxis[44] = {15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 64, 68, 72, 76, 81, 86, 91, 96, 101, 106, 110, 115, 120, 126, 133, 141, 150, 160, 171, 185, 200, 220, 243, 273, 320, 380, 440, 510, 600, 700, 830, 1000, 1500, 3000}; 

   TH1D* hdata = new TH1D("hdata", "hdata", 43, xAxis);
   TH1D* hDY = new TH1D("hDY", "hDY", 43, xAxis);
   TH1D* httbar = new TH1D("httbar", "httbar", 43, xAxis);
   TH1D* hDYtautau = new TH1D("hDYtautau", "hDYtautau", 43, xAxis);
   TH1D* hWW = new TH1D("hWW", "hWW", 43, xAxis);
   TH1D* hWZ = new TH1D("hWZ", "hWZ", 43, xAxis);
   TH1D* hZZ = new TH1D("hZZ", "hZZ", 43, xAxis);
   TH1D* htW = new TH1D("htW", "htW", 43, xAxis);
   TH1D* hWJets = new TH1D("hWJets", "hWJets", 43, xAxis);
   TH1D* hdijet = new TH1D("hdijet", "hdijet", 43, xAxis);
   TH1D* htotal = new TH1D("htotal", "htotal", 43, xAxis);
   TH1D* hratio = new TH1D("hratio", "hratio", 43, xAxis);

   // call
   hdata = (TH1D*) h_data->Clone();
   hDY = (TH1D*) h_SignalMC->Clone();
   httbar = (TH1D*) h_ttbar_emu->Clone();
   hDYtautau = (TH1D*) h_DYTauTau_emu->Clone();
   hWW = (TH1D*) h_WW_emu->Clone();
   hWZ = (TH1D*) h_WZ->Clone();
   hZZ = (TH1D*) h_ZZ->Clone();
   hdiboson = (TH1D*) hWW->Clone();
   hdiboson->Add( hWZ );
   hdiboson->Add( hZZ );

   htW = (TH1D*) h_tW_emu->Clone();
   hWJets = (TH1D*) h_WJets_FR->Clone();
   hdijet = (TH1D*) h_diJet_FR->Clone();

   TH1D* hTop = (TH1D*) httbar->Clone();
   hTop->Add(htW);
   TH1D* hEW = (TH1D*) hdiboson->Clone();
   hEW->Add(hDYtautau);
   //hEW->Add(hWJets);

   TH1D* hFake = (TH1D*) hdijet->Clone();
   hFake->Add(hWJets);

   hDY->SetFillColor(kOrange-2);
   hDY->SetLineColor(kOrange+3);
   hTop->SetFillColor(kRed+2);
   hTop->SetLineColor(kRed+4);
   hEW->SetFillColor(kOrange+10);
   hEW->SetLineColor(kOrange+3);
   hFake->SetFillColor(kViolet-5);
   hFake->SetLineColor(kOrange+3);

   THStack* hstack = new THStack("hstack", "hstack");
   hstack->Add(hFake, "hist");
   hstack->Add(hEW, "hist");
   hstack->Add(hTop, "hist");
   hstack->Add(hDY, "hist");

   htotal->Add(hFake);
   htotal->Add(hEW);
   htotal->Add(hTop);
   htotal->Add(hDY);
   hratio->Divide(hdata, htotal);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = 932;
   color = new TColor(ci, 1, 1, 1, " ", 0);
   h_data->SetFillColor(ci);
   h_data->SetMarkerStyle(20);
   h_data->SetMarkerSize(1.0);
   h_data->GetXaxis()->SetTitle("m [GeV]");
   h_data->GetXaxis()->SetRange(1,43);
   h_data->GetXaxis()->SetMoreLogLabels();
   h_data->GetXaxis()->SetNoExponent();
   h_data->GetXaxis()->SetLabelFont(42);
   h_data->GetXaxis()->SetLabelSize(0);
   h_data->GetXaxis()->SetTitleSize(0);
   h_data->GetXaxis()->SetTitleFont(42);
   h_data->GetYaxis()->SetTitle("Entries per bin");
   h_data->GetYaxis()->SetLabelFont(42);
   h_data->GetYaxis()->SetLabelSize(0.035);
   h_data->GetYaxis()->SetTitleSize(0.06);
   h_data->GetYaxis()->SetTitleOffset(0.9);
   h_data->GetYaxis()->SetTitleFont(42);
   h_data->GetZaxis()->SetLabelFont(42);
   h_data->GetZaxis()->SetLabelSize(0.035);
   h_data->GetZaxis()->SetTitleSize(0.035);
   h_data->GetZaxis()->SetTitleFont(42);
   h_data->SetMinimum(0.1);
   h_data->SetMaximum(3100000);
   h_data->Draw("E1P");
   hstack->Draw("histsame");
   h_data->Draw("E1Psame");
   //
   TLine grid_;
   grid_.SetLineColor(kGray+2);
   grid_.SetLineStyle(kSolid);
   for( size_t ii=0; ii<44; ii++ ) {
     grid_.DrawLine(xAxis[ii],0.1,xAxis[ii],htotal->GetBinContent(ii+1));
   }

   TLegend *leg = new TLegend(0.70,0.70,0.90,0.90,NULL,"brNDC");
   leg->SetBorderSize(0);
   leg->SetTextFont(62);
   leg->SetLineColor(1);
   leg->SetLineStyle(1);
   leg->SetLineWidth(1);
   leg->SetFillColor(0);
   leg->SetFillStyle(0);
   hdata->SetMarkerSize(1);
   TLegendEntry *entry=leg->AddEntry(hdata,"data","pl");
   entry=leg->AddEntry(hDY,"#gamma* /#font[122]{Z} #rightarrow ee","f");
   entry=leg->AddEntry(hTop,"t#bar{t}+tW+#bar{t}W","f");
   entry=leg->AddEntry(hEW,"EW","f");
   entry=leg->AddEntry(hFake,"Fakes","f");
   leg->Draw();

   // TPaveText *pave = new TPaveText(1390,4500000,2000,6500000); // CHANGE
   // pave->SetFillColor(0);
   // TText* text = pave->AddText("2.3 fb^{#font[122]{\55}1} (13 TeV)");
   // text->SetTextSize(0.03);
   // pave->Draw("0");
   // TPaveText *ptitle = new TPaveText(19,900000,60,1020000); // CHANGE
   // ptitle->SetFillColor(0);
   // TText* ttitle = ptitle->AddText("CMS Preliminary");
   // ttitle->SetTextSize(0.035);
   // ptitle->Draw("0");
   TLatex latex;
   latex.DrawLatexNDC(0.14, 0.96, "#font[62]{#scale[0.8]{CMS}}");
   latex.DrawLatexNDC(0.74, 0.96, "#font[62]{#scale[0.7]{2.3 fb^{-1} (13 TeV)}}");

// ------------>Primitives in pad: bottomPad
   TPad *bottomPad = new TPad("bottomPad", "bottomPad",0.01,0.01,0.99,0.3);
   bottomPad->Draw();
   bottomPad->cd();
   bottomPad->Range(0.7499746,-0.2272727,3.590752,1.590909);
   bottomPad->SetFillColor(0);
   bottomPad->SetBorderMode(0);
   bottomPad->SetBorderSize(2);
   bottomPad->SetLogx();
   //bottomPad->SetGridx();
   //bottomPad->SetGridy();
   bottomPad->SetTickx(1);
   bottomPad->SetTicky(1);
   bottomPad->SetLeftMargin(0.122);
   bottomPad->SetRightMargin(0.04);
   bottomPad->SetTopMargin(0.05);
   bottomPad->SetBottomMargin(0.4);
   bottomPad->SetFrameFillStyle(0);
   bottomPad->SetFrameBorderMode(0);
   bottomPad->SetFrameFillStyle(0);
   bottomPad->SetFrameBorderMode(0);

   hratio->SetMinimum(0.45);
   hratio->SetMaximum(1.55);
   hratio->SetEntries(129);
   hratio->SetDirectory(0);
   hratio->SetStats(0);

   hratio->SetFillColor(kBlack);
   hratio->SetFillStyle(1001);
   hratio->SetMarkerStyle(20);
   hratio->GetXaxis()->SetTitle("m [GeV]");
   hratio->GetXaxis()->SetRange(1,43);
   hratio->GetXaxis()->SetMoreLogLabels();
   hratio->GetXaxis()->SetNoExponent();
   hratio->GetXaxis()->SetLabelFont(42);
   hratio->GetXaxis()->SetLabelOffset(0.007);
   hratio->GetXaxis()->SetLabelSize(0.15);
   hratio->GetXaxis()->SetTitleSize(0.2);
   hratio->GetXaxis()->SetTitleOffset(0.9);
   hratio->GetXaxis()->SetTitleFont(42);
   hratio->GetYaxis()->SetTitle("Data/(DY+Bkg)");
   hratio->GetYaxis()->SetLabelFont(42);
   hratio->GetYaxis()->SetLabelSize(0.07);
   hratio->GetYaxis()->SetTitleSize(0.1);
   hratio->GetYaxis()->SetTitleOffset(0.4);
   hratio->GetYaxis()->SetTitleFont(42);
   // hratio->SetMinimum(0.4);
   // hratio->SetMaximum(1.6);
   // hratio->GetYaxis()->SetRangeUser(42);
   hratio->GetZaxis()->SetLabelFont(42);
   hratio->GetZaxis()->SetLabelSize(0.035);
   hratio->GetZaxis()->SetTitleSize(0.035);
   hratio->GetZaxis()->SetTitleFont(42);
   hratio->Draw("E1PL");
   
   TF1 *f_line1 = new TF1("f_line","1",-10000,10000);
   f_line1->SetFillColor(19);
   f_line1->SetFillStyle(0);
   f_line1->SetMarkerStyle(20);

   f_line1->SetLineColor(kSpring-1);
   f_line1->SetLineStyle(2);
   f_line1->SetLineWidth(1);
   f_line1->GetXaxis()->SetLabelFont(42);
   f_line1->GetXaxis()->SetLabelOffset(0.007);
   f_line1->GetXaxis()->SetLabelSize(0.05);
   f_line1->GetXaxis()->SetTitleSize(0.06);
   f_line1->GetXaxis()->SetTitleOffset(0.9);
   f_line1->GetXaxis()->SetTitleFont(42);
   f_line1->GetYaxis()->SetLabelFont(42);
   f_line1->GetYaxis()->SetLabelOffset(0.007);
   f_line1->GetYaxis()->SetLabelSize(0.05);
   f_line1->GetYaxis()->SetTitleSize(0.06);
   f_line1->GetYaxis()->SetTitleOffset(1.25);
   f_line1->GetYaxis()->SetTitleFont(42);
   f_line1->Draw("SAME");
   hratio->Draw("E1PLsame");

   TLine gridRatio;
   gridRatio.SetLineColor(kGray+2);
   gridRatio.SetLineStyle(2);
   for( size_t ii=0; ii<44; ii++ ) {
     gridRatio.DrawLine(xAxis[ii],0.45,xAxis[ii],1.55);
   }

   c1->cd();
   c1->Modified();
   c1->cd();
   c1->SetSelected(c1);

   c1->SaveAs("Electron_Mass_DYBin.pdf");
}

