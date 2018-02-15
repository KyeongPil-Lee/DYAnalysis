void c_AccEff_aMCNLO_Electron()
{
// change: coloar, title margin etc => flag "CHANGE"
//=========Macro generated from canvas: c_AccEff_aMCNLO/
//=========  (Thu Jul 14 11:21:11 2016) by ROOT version6.02/05

   TFile *f_output = TFile::Open("ROOTFile_AccEff_Electron.root", "RECREATE");

   TCanvas *c_AccEff_aMCNLO = new TCanvas("c_AccEff_aMCNLO", "",0,0,800,600);
   gStyle->SetOptFit(1);
   gStyle->SetOptStat(0);
   gStyle->SetOptTitle(0);
   c_AccEff_aMCNLO->Range(0.664386,-0.1743902,3.57656,1.167073);
   c_AccEff_aMCNLO->SetFillColor(0);
   c_AccEff_aMCNLO->SetBorderMode(0);
   c_AccEff_aMCNLO->SetBorderSize(2);
   c_AccEff_aMCNLO->SetLogx();
   //c_AccEff_aMCNLO->SetGridx(); // CHANGE
   //c_AccEff_aMCNLO->SetGridy(); // CHANGE
   c_AccEff_aMCNLO->SetTickx(1);
   c_AccEff_aMCNLO->SetTicky(1);
   c_AccEff_aMCNLO->SetLeftMargin(0.13); // CHANGE
   c_AccEff_aMCNLO->SetRightMargin(0.02);
   c_AccEff_aMCNLO->SetTopMargin(0.08); // CHANGE
   c_AccEff_aMCNLO->SetBottomMargin(0.14); // CHANGE
   c_AccEff_aMCNLO->SetFrameFillStyle(0);
   c_AccEff_aMCNLO->SetFrameBorderMode(0);
   c_AccEff_aMCNLO->SetFrameFillStyle(0);
   c_AccEff_aMCNLO->SetFrameBorderMode(0);
   
   Double_t g_Acc_fx3001[43] = {
   17.5,
   22.5,
   27.5,
   32.5,
   37.5,
   42.5,
   47.5,
   52.5,
   57.5,
   62,
   66,
   70,
   74,
   78.5,
   83.5,
   88.5,
   93.5,
   98.5,
   103.5,
   108,
   112.5,
   117.5,
   123,
   129.5,
   137,
   145.5,
   155,
   165.5,
   178,
   192.5,
   210,
   231.5,
   258,
   296.5,
   350,
   410,
   475,
   555,
   650,
   765,
   915,
   1250,
   2250};
   Double_t g_Acc_fy3001[43] = {
   0.00241526,
   0.00442631,
   0.00770605,
   0.0131774,
   0.0274439,
   0.0566186,
   0.111737,
   0.172266,
   0.240917,
   0.29714,
   0.329609,
   0.349232,
   0.363963,
   0.375351,
   0.389052,
   0.402658,
   0.410595,
   0.422718,
   0.432043,
   0.433941,
   0.449775,
   0.452133,
   0.454192,
   0.458333,
   0.463889,
   0.481315,
   0.495399,
   0.492444,
   0.498463,
   0.529381,
   0.532634,
   0.537669,
   0.551896,
   0.578786,
   0.605395,
   0.637767,
   0.657545,
   0.685071,
   0.708959,
   0.741226,
   0.77101,
   0.803484,
   0.842834,};
   Double_t g_Acc_felx3001[43] = {
   2.5,
   2.5,
   2.5,
   2.5,
   2.5,
   2.5,
   2.5,
   2.5,
   2.5,
   2,
   2,
   2,
   2,
   2.5,
   2.5,
   2.5,
   2.5,
   2.5,
   2.5,
   2,
   2.5,
   2.5,
   3,
   3.5,
   4,
   4.5,
   5,
   5.5,
   7,
   7.5,
   10,
   11.5,
   15,
   23.5,
   30,
   30,
   35,
   45,
   50,
   65,
   85,
   250,
   750};
   Double_t g_Acc_fely3001[43] = {
   8.094653e-05,
   0.0001697936,
   0.000324105,
   0.0006415422,
   0.001130414,
   0.00172974,
   0.002241055,
   0.001744193,
   0.001927307,
   0.002224365,
   0.002188738,
   0.002071899,
   0.001857125,
   0.001370773,
   0.0009588698,
   0.0003966966,
   0.0003972615,
   0.001360638,
   0.002392883,
   0.003680421,
   0.004098762,
   0.005075813,
   0.005653833,
   0.006239719,
   0.006968332,
   0.008134276,
   0.008958675,
   0.01034768,
   0.01058506,
   0.01223439,
   0.002926514,
   0.003400791,
   0.003778255,
   0.004146745,
   0.005210306,
   0.005055831,
   0.003914761,
   0.003862915,
   0.005082798,
   0.002616619,
   0.003207504,
   0.002718439,
   0.002141064};
   Double_t g_Acc_fehx3001[43] = {
   2.5,
   2.5,
   2.5,
   2.5,
   2.5,
   2.5,
   2.5,
   2.5,
   2.5,
   2,
   2,
   2,
   2,
   2.5,
   2.5,
   2.5,
   2.5,
   2.5,
   2.5,
   2,
   2.5,
   2.5,
   3,
   3.5,
   4,
   4.5,
   5,
   5.5,
   7,
   7.5,
   10,
   11.5,
   15,
   23.5,
   30,
   30,
   35,
   45,
   50,
   65,
   85,
   250,
   750};
   Double_t g_Acc_fehy3001[43] = {
   8.094653e-05,
   0.0001697936,
   0.000324105,
   0.0006415422,
   0.001130414,
   0.00172974,
   0.002241055,
   0.001744193,
   0.001927307,
   0.002224365,
   0.002188738,
   0.002071899,
   0.001857125,
   0.001370773,
   0.0009588698,
   0.0003966966,
   0.0003972615,
   0.001360638,
   0.002392883,
   0.003680421,
   0.004098762,
   0.005075813,
   0.005653833,
   0.006239719,
   0.006968332,
   0.008134276,
   0.008958675,
   0.01034768,
   0.01058506,
   0.01223439,
   0.002926514,
   0.003400791,
   0.003778255,
   0.004146745,
   0.005210306,
   0.005055831,
   0.003914761,
   0.003862915,
   0.005082798,
   0.002616619,
   0.003207504,
   0.002718439,
   0.002141064};
   TGraphAsymmErrors *grae = new TGraphAsymmErrors(43,g_Acc_fx3001,g_Acc_fy3001,g_Acc_felx3001,g_Acc_fehx3001,g_Acc_fely3001,g_Acc_fehy3001);
   grae->SetName("g_Acc");
   grae->SetTitle("CMS Simulation"); // CHANGE
   f_output->cd();
   grae->Write("g_acc");

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = 924;
   color = new TColor(ci, 1, 1, 1, " ", 0);
   grae->SetFillColor(ci);

   ci = TColor::GetColor("#0000ff"); // CHANGE
   grae->SetLineColor(ci);

   ci = TColor::GetColor("#0000ff"); // CHANGE
   grae->SetMarkerColor(ci);
   grae->SetMarkerStyle(20);
   
   TH1F *Graph_g_Acc3001 = new TH1F("Graph_g_Acc3001","",100,13.5,3298.5);
   Graph_g_Acc3001->SetMinimum(0);
   Graph_g_Acc3001->SetMaximum(1.1);
   Graph_g_Acc3001->SetDirectory(0);
   Graph_g_Acc3001->SetStats(0);
   Graph_g_Acc3001->SetLineStyle(0);
   Graph_g_Acc3001->SetMarkerStyle(20);
   Graph_g_Acc3001->GetXaxis()->SetTitle("m (post-FSR) [GeV]");
   Graph_g_Acc3001->GetXaxis()->SetMoreLogLabels();
   Graph_g_Acc3001->GetXaxis()->SetNoExponent();
   Graph_g_Acc3001->GetXaxis()->SetLabelFont(42);
   Graph_g_Acc3001->GetXaxis()->SetLabelOffset(0.007);
   Graph_g_Acc3001->GetXaxis()->SetLabelSize(0.04);
   Graph_g_Acc3001->GetXaxis()->SetTitleSize(0.06);
   Graph_g_Acc3001->GetXaxis()->SetTitleOffset(1.00);
   Graph_g_Acc3001->GetXaxis()->SetTitleFont(42);
   Graph_g_Acc3001->GetYaxis()->SetTitle("Fraction of events");
   Graph_g_Acc3001->GetYaxis()->SetLabelFont(42);
   Graph_g_Acc3001->GetYaxis()->SetLabelOffset(0.007);
   Graph_g_Acc3001->GetYaxis()->SetLabelSize(0.04);
   Graph_g_Acc3001->GetYaxis()->SetTitleSize(0.06);
   Graph_g_Acc3001->GetYaxis()->SetTitleOffset(0.9);
   Graph_g_Acc3001->GetYaxis()->SetTitleFont(42);
   Graph_g_Acc3001->GetZaxis()->SetLabelFont(42);
   Graph_g_Acc3001->GetZaxis()->SetLabelOffset(0.007);
   Graph_g_Acc3001->GetZaxis()->SetLabelSize(0.05);
   Graph_g_Acc3001->GetZaxis()->SetTitleSize(0.06);
   Graph_g_Acc3001->GetZaxis()->SetTitleFont(42);
   grae->SetHistogram(Graph_g_Acc3001);
   
   grae->Draw("ap");
   
   Double_t g_Eff_fx3002[43] = {
   17.5,
   22.5,
   27.5,
   32.5,
   37.5,
   42.5,
   47.5,
   52.5,
   57.5,
   62,
   66,
   70,
   74,
   78.5,
   83.5,
   88.5,
   93.5,
   98.5,
   103.5,
   108,
   112.5,
   117.5,
   123,
   129.5,
   137,
   145.5,
   155,
   165.5,
   178,
   192.5,
   210,
   231.5,
   258,
   296.5,
   350,
   410,
   475,
   555,
   650,
   765,
   915,
   1250,
   2250};
   Double_t g_Eff_fy3002[43] = {
   0.371143,
   0.384214,
   0.390763,
   0.378502,
   0.309379,
   0.291768,
   0.267626,
   0.263468,
   0.285086,
   0.326627,
   0.369785,
   0.401107,
   0.440921,
   0.47162,
   0.510949,
   0.576081,
   0.595057,
   0.599345,
   0.610596,
   0.6159,
   0.618582,
   0.624026,
   0.627362,
   0.618864,
   0.645617,
   0.64012,
   0.650206,
   0.665262,
   0.66049,
   0.694485,
   0.691222,
   0.700838,
   0.706864,
   0.704666,
   0.710127,
   0.728986,
   0.719565,
   0.737109,
   0.730499,
   0.745373,
   0.753163,
   0.763362,
   0.776996,};
   Double_t g_Eff_felx3002[43] = {
   2.5,
   2.5,
   2.5,
   2.5,
   2.5,
   2.5,
   2.5,
   2.5,
   2.5,
   2,
   2,
   2,
   2,
   2.5,
   2.5,
   2.5,
   2.5,
   2.5,
   2.5,
   2,
   2.5,
   2.5,
   3,
   3.5,
   4,
   4.5,
   5,
   5.5,
   7,
   7.5,
   10,
   11.5,
   15,
   23.5,
   30,
   30,
   35,
   45,
   50,
   65,
   85,
   250,
   750};
   Double_t g_Eff_fely3002[43] = {
   0.007294507,
   0.007982581,
   0.007851808,
   0.006384529,
   0.005243014,
   0.004456144,
   0.003888443,
   0.002567033,
   0.002555722,
   0.002771931,
   0.002596652,
   0.002371215,
   0.00206872,
   0.001441172,
   0.0009303165,
   0.000348932,
   0.0003434271,
   0.001138346,
   0.001960079,
   0.002980723,
   0.003299383,
   0.004090716,
   0.004464148,
   0.00464273,
   0.005315284,
   0.006161627,
   0.006319776,
   0.007987223,
   0.007377645,
   0.008534041,
   0.001933359,
   0.0022379,
   0.00239763,
   0.002653949,
   0.003133323,
   0.003024053,
   0.002422101,
   0.002399088,
   0.003165954,
   0.001733202,
   0.002212569,
   0.00202791,
   0.001924405};
   Double_t g_Eff_fehx3002[43] = {
   2.5,
   2.5,
   2.5,
   2.5,
   2.5,
   2.5,
   2.5,
   2.5,
   2.5,
   2,
   2,
   2,
   2,
   2.5,
   2.5,
   2.5,
   2.5,
   2.5,
   2.5,
   2,
   2.5,
   2.5,
   3,
   3.5,
   4,
   4.5,
   5,
   5.5,
   7,
   7.5,
   10,
   11.5,
   15,
   23.5,
   30,
   30,
   35,
   45,
   50,
   65,
   85,
   250,
   750};
   Double_t g_Eff_fehy3002[43] = {
   0.007294507,
   0.007982581,
   0.007851808,
   0.006384529,
   0.005243014,
   0.004456144,
   0.003888443,
   0.002567033,
   0.002555722,
   0.002771931,
   0.002596652,
   0.002371215,
   0.00206872,
   0.001441172,
   0.0009303165,
   0.000348932,
   0.0003434271,
   0.001138346,
   0.001960079,
   0.002980723,
   0.003299383,
   0.004090716,
   0.004464148,
   0.00464273,
   0.005315284,
   0.006161627,
   0.006319776,
   0.007987223,
   0.007377645,
   0.008534041,
   0.001933359,
   0.0022379,
   0.00239763,
   0.002653949,
   0.003133323,
   0.003024053,
   0.002422101,
   0.002399088,
   0.003165954,
   0.001733202,
   0.002212569,
   0.00202791,
   0.001924405};
   grae = new TGraphAsymmErrors(43,g_Eff_fx3002,g_Eff_fy3002,g_Eff_felx3002,g_Eff_fehx3002,g_Eff_fely3002,g_Eff_fehy3002);
   grae->SetName("g_Eff");
   grae->SetTitle("");
   f_output->cd();
   grae->Write("g_eff");

   ci = 925;
   color = new TColor(ci, 1, 1, 1, " ", 0);
   grae->SetFillColor(ci);

   ci = TColor::GetColor("#ff0000"); // CHANGE
   grae->SetLineColor(ci);

   ci = TColor::GetColor("#ff0000"); // CHANGE
   grae->SetMarkerColor(ci);
   grae->SetMarkerStyle(20);
   
   TH1F *Graph_g_Eff3002 = new TH1F("Graph_g_Eff3002","",100,13.5,3298.5);
   Graph_g_Eff3002->SetMinimum(0.7251424);
   Graph_g_Eff3002->SetMaximum(0.9790425);
   Graph_g_Eff3002->SetDirectory(0);
   Graph_g_Eff3002->SetStats(0);
   Graph_g_Eff3002->SetLineStyle(0);
   Graph_g_Eff3002->SetMarkerStyle(20);
   Graph_g_Eff3002->GetXaxis()->SetLabelFont(42);
   Graph_g_Eff3002->GetXaxis()->SetLabelOffset(0.007);
   Graph_g_Eff3002->GetXaxis()->SetLabelSize(0.05);
   Graph_g_Eff3002->GetXaxis()->SetTitleSize(0.06);
   Graph_g_Eff3002->GetXaxis()->SetTitleOffset(0.9);
   Graph_g_Eff3002->GetXaxis()->SetTitleFont(42);
   Graph_g_Eff3002->GetYaxis()->SetLabelFont(42);
   Graph_g_Eff3002->GetYaxis()->SetLabelOffset(0.007);
   Graph_g_Eff3002->GetYaxis()->SetLabelSize(0.05);
   Graph_g_Eff3002->GetYaxis()->SetTitleSize(0.06);
   Graph_g_Eff3002->GetYaxis()->SetTitleOffset(1.25);
   Graph_g_Eff3002->GetYaxis()->SetTitleFont(42);
   Graph_g_Eff3002->GetZaxis()->SetLabelFont(42);
   Graph_g_Eff3002->GetZaxis()->SetLabelOffset(0.007);
   Graph_g_Eff3002->GetZaxis()->SetLabelSize(0.05);
   Graph_g_Eff3002->GetZaxis()->SetTitleSize(0.06);
   Graph_g_Eff3002->GetZaxis()->SetTitleFont(42);
   grae->SetHistogram(Graph_g_Eff3002);
   
   grae->Draw("p");
   
   Double_t g_AccEff_fx3003[43] = {
   17.5,
   22.5,
   27.5,
   32.5,
   37.5,
   42.5,
   47.5,
   52.5,
   57.5,
   62,
   66,
   70,
   74,
   78.5,
   83.5,
   88.5,
   93.5,
   98.5,
   103.5,
   108,
   112.5,
   117.5,
   123,
   129.5,
   137,
   145.5,
   155,
   165.5,
   178,
   192.5,
   210,
   231.5,
   258,
   296.5,
   350,
   410,
   475,
   555,
   650,
   765,
   915,
   1250,
   2250};
   Double_t g_AccEff_fy3003[43] = {
   0.000896408,
   0.00170065,
   0.00301124,
   0.00498766,
   0.00849055,
   0.0165195,
   0.0299037,
   0.0453867,
   0.068682,
   0.0970539,
   0.121884,
   0.140079,
   0.160479,
   0.177023,
   0.198786,
   0.231964,
   0.244327,
   0.253354,
   0.263804,
   0.267264,
   0.278223,
   0.282143,
   0.284943,
   0.283646,
   0.299495,
   0.308099,
   0.322111,
   0.327604,
   0.32923,
   0.367648,
   0.368168,
   0.376819,
   0.390115,
   0.407851,
   0.429908,
   0.464923,
   0.473146,
   0.504972,
   0.517894,
   0.55249,
   0.580696,
   0.613349,
   0.654879,};
   Double_t g_AccEff_felx3003[43] = {
   2.5,
   2.5,
   2.5,
   2.5,
   2.5,
   2.5,
   2.5,
   2.5,
   2.5,
   2,
   2,
   2,
   2,
   2.5,
   2.5,
   2.5,
   2.5,
   2.5,
   2.5,
   2,
   2.5,
   2.5,
   3,
   3.5,
   4,
   4.5,
   5,
   5.5,
   7,
   7.5,
   10,
   11.5,
   15,
   23.5,
   30,
   30,
   35,
   45,
   50,
   65,
   85,
   250,
   750};
   Double_t g_AccEff_fely3003[43] = {
   0.0001202896,
   0.000237104,
   0.0004695984,
   0.0009949955,
   0.001718246,
   0.002580267,
   0.003103883,
   0.00236167,
   0.00257904,
   0.002981819,
   0.002920798,
   0.002717276,
   0.002431329,
   0.001755846,
   0.001183951,
   0.0004715289,
   0.0004699907,
   0.001599219,
   0.002796237,
   0.004304234,
   0.004794587,
   0.00598082,
   0.006592697,
   0.007052141,
   0.008019628,
   0.009389227,
   0.01016605,
   0.01219816,
   0.01205248,
   0.01408102,
   0.003313651,
   0.003863008,
   0.00427843,
   0.004719039,
   0.005867606,
   0.005755109,
   0.004571937,
   0.00451437,
   0.005985049,
   0.003164897,
   0.003962344,
   0.003479203,
   0.003050256};
   Double_t g_AccEff_fehx3003[43] = {
   2.5,
   2.5,
   2.5,
   2.5,
   2.5,
   2.5,
   2.5,
   2.5,
   2.5,
   2,
   2,
   2,
   2,
   2.5,
   2.5,
   2.5,
   2.5,
   2.5,
   2.5,
   2,
   2.5,
   2.5,
   3,
   3.5,
   4,
   4.5,
   5,
   5.5,
   7,
   7.5,
   10,
   11.5,
   15,
   23.5,
   30,
   30,
   35,
   45,
   50,
   65,
   85,
   250,
   750};
   Double_t g_AccEff_fehy3003[43] = {
   0.0001202896,
   0.000237104,
   0.0004695984,
   0.0009949955,
   0.001718246,
   0.002580267,
   0.003103883,
   0.00236167,
   0.00257904,
   0.002981819,
   0.002920798,
   0.002717276,
   0.002431329,
   0.001755846,
   0.001183951,
   0.0004715289,
   0.0004699907,
   0.001599219,
   0.002796237,
   0.004304234,
   0.004794587,
   0.00598082,
   0.006592697,
   0.007052141,
   0.008019628,
   0.009389227,
   0.01016605,
   0.01219816,
   0.01205248,
   0.01408102,
   0.003313651,
   0.003863008,
   0.00427843,
   0.004719039,
   0.005867606,
   0.005755109,
   0.004571937,
   0.00451437,
   0.005985049,
   0.003164897,
   0.003962344,
   0.003479203,
   0.003050256};
   grae = new TGraphAsymmErrors(43,g_AccEff_fx3003,g_AccEff_fy3003,g_AccEff_felx3003,g_AccEff_fehx3003,g_AccEff_fely3003,g_AccEff_fehy3003);
   grae->SetName("g_AccEff");
   grae->SetTitle("");
   f_output->cd();
   grae->Write("g_accEff");
   f_output->Close();

   ci = 926;
   color = new TColor(ci, 1, 1, 1, " ", 0);
   grae->SetFillColor(ci);

   ci = TColor::GetColor(kBlack); // CHANGE
   grae->SetLineColor(ci);

   ci = TColor::GetColor(kBlack); // CHANGE
   grae->SetMarkerColor(ci);
   grae->SetMarkerStyle(20);
   
   TH1F *Graph_g_AccEff3003 = new TH1F("Graph_g_AccEff3003","",100,13.5,3298.5);
   Graph_g_AccEff3003->SetMinimum(0);
   Graph_g_AccEff3003->SetMaximum(0.9213429);
   Graph_g_AccEff3003->SetDirectory(0);
   Graph_g_AccEff3003->SetStats(0);
   Graph_g_AccEff3003->SetLineStyle(0);
   Graph_g_AccEff3003->SetMarkerStyle(20);
   Graph_g_AccEff3003->GetXaxis()->SetLabelFont(42);
   Graph_g_AccEff3003->GetXaxis()->SetLabelOffset(0.007);
   Graph_g_AccEff3003->GetXaxis()->SetLabelSize(0.05);
   Graph_g_AccEff3003->GetXaxis()->SetTitleSize(0.06);
   Graph_g_AccEff3003->GetXaxis()->SetTitleOffset(0.9);
   Graph_g_AccEff3003->GetXaxis()->SetTitleFont(42);
   Graph_g_AccEff3003->GetYaxis()->SetLabelFont(42);
   Graph_g_AccEff3003->GetYaxis()->SetLabelOffset(0.007);
   Graph_g_AccEff3003->GetYaxis()->SetLabelSize(0.05);
   Graph_g_AccEff3003->GetYaxis()->SetTitleSize(0.06);
   Graph_g_AccEff3003->GetYaxis()->SetTitleOffset(1.25);
   Graph_g_AccEff3003->GetYaxis()->SetTitleFont(42);
   Graph_g_AccEff3003->GetZaxis()->SetLabelFont(42);
   Graph_g_AccEff3003->GetZaxis()->SetLabelOffset(0.007);
   Graph_g_AccEff3003->GetZaxis()->SetLabelSize(0.05);
   Graph_g_AccEff3003->GetZaxis()->SetTitleSize(0.06);
   Graph_g_AccEff3003->GetZaxis()->SetTitleFont(42);
   grae->SetHistogram(Graph_g_AccEff3003);
   
   grae->Draw("p");
   
   TLegend *leg = new TLegend(0.75,0.2,0.97,0.4,NULL,"brNDC");
   leg->SetBorderSize(0);
   leg->SetTextFont(62);
   leg->SetLineColor(1);
   leg->SetLineStyle(1);
   leg->SetLineWidth(1);
   leg->SetFillColor(0);
   leg->SetFillStyle(0);
   TLegendEntry *entry=leg->AddEntry("g_Acc","#font[122]{A}","lpf");

   ci = 924;
   color = new TColor(ci, 1, 1, 1, " ", 0);
   entry->SetFillColor(ci);
   entry->SetFillStyle(1001);

   ci = TColor::GetColor("#00cc00");
   entry->SetLineColor(ci);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);

   ci = TColor::GetColor("#00cc00");
   entry->SetMarkerColor(ci);
   entry->SetMarkerStyle(20);
   entry->SetMarkerSize(1);
   entry->SetTextFont(62);
   entry=leg->AddEntry("g_Eff","#varepsilon","lpf");

   ci = 925;
   color = new TColor(ci, 1, 1, 1, " ", 0);
   entry->SetFillColor(ci);
   entry->SetFillStyle(1001);

   ci = TColor::GetColor("#0000ff");
   entry->SetLineColor(ci);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);

   ci = TColor::GetColor("#0000ff");
   entry->SetMarkerColor(ci);
   entry->SetMarkerStyle(20);
   entry->SetMarkerSize(1);
   entry->SetTextFont(62);
   entry=leg->AddEntry("g_AccEff","#font[122]{A} #times #varepsilon","lpf");

   ci = 926;
   color = new TColor(ci, 1, 1, 1, " ", 0);
   entry->SetFillColor(ci);
   entry->SetFillStyle(1001);

   ci = TColor::GetColor("#ff0000");
   entry->SetLineColor(ci);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);

   ci = TColor::GetColor("#ff0000");
   entry->SetMarkerColor(ci);
   entry->SetMarkerStyle(20);
   entry->SetMarkerSize(1);
   entry->SetTextFont(62);
   leg->Draw();
   /*
   tex->SetLineWidth(2); // CHANGE
   tex->Draw();
      tex = new TLatex(17,1.01,"#font[62]{CMS}");
   tex->SetLineWidth(2);
   tex->Draw();
   tex = new TLatex(17,0.95,"#font[42]{#it{#scale[0.8]{Simulation}}}");
   tex->SetLineWidth(2);
   tex->Draw();
   */
   TPaveText *pave = new TPaveText(1700,1.1,2000,1.2); // CHANGE
   pave->SetFillColor(0);
   TText* text = pave->AddText("13 TeV");
   //TText* text = pave->AddText("2.8 fb^{#font[122]{\55}1} (13 TeV)");
   text->SetTextSize(0.04);
   pave->Draw("0");
   TPaveText *ptitle = new TPaveText(19,0.95,70,1.05); // CHANGE
   ptitle->SetFillColor(0);
   TText* ttitle = ptitle->AddText("CMS Simulation");
   ttitle->SetTextSize(0.045);
   ptitle->Draw("0");
   c_AccEff_aMCNLO->Update();

   c_AccEff_aMCNLO->Modified();
   c_AccEff_aMCNLO->cd();
   c_AccEff_aMCNLO->SetSelected(c_AccEff_aMCNLO);
   c_AccEff_aMCNLO->SaveAs("Electron_AccEff_aMCNLO.pdf");;
}
