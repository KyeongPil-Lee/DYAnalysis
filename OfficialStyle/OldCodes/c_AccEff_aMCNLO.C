void c_AccEff_aMCNLO()
{
// change: coloar, title margin etc => flag "CHANGE"
//=========Macro generated from canvas: c_AccEff_aMCNLO/
//=========  (Thu Jul 14 11:21:11 2016) by ROOT version6.02/05
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
   0.004890906,
   0.009082363,
   0.0176073,
   0.0455531,
   0.09860085,
   0.1805071,
   0.2637167,
   0.3150208,
   0.352526,
   0.3806888,
   0.4021783,
   0.4139533,
   0.4245321,
   0.4372233,
   0.4472878,
   0.4550676,
   0.4587785,
   0.4681591,
   0.4705091,
   0.477365,
   0.4799197,
   0.4888532,
   0.4913901,
   0.4947683,
   0.4962391,
   0.5058652,
   0.5149497,
   0.5193702,
   0.5307285,
   0.5393456,
   0.5533921,
   0.5676345,
   0.583872,
   0.6084787,
   0.627156,
   0.6602667,
   0.6973616,
   0.7284311,
   0.7477453,
   0.7829229,
   0.8125739,
   0.8501621,
   0.8980768};
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
   0.774309,
   0.809608,
   0.7853961,
   0.7526853,
   0.7661737,
   0.7803707,
   0.8255139,
   0.8428951,
   0.8523172,
   0.8534721,
   0.8575455,
   0.8685455,
   0.8694956,
   0.880646,
   0.899264,
   0.9178234,
   0.9203901,
   0.9240839,
   0.9267951,
   0.9265246,
   0.9262486,
   0.922794,
   0.9293299,
   0.9490347,
   0.9371148,
   0.9361131,
   0.944204,
   0.924605,
   0.9433539,
   0.9363647,
   0.9467309,
   0.9464329,
   0.9481299,
   0.9497306,
   0.9547509,
   0.9512028,
   0.9418325,
   0.9481128,
   0.9465001,
   0.9434357,
   0.9419723,
   0.9419381,
   0.9296171};
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
   0.003787073,
   0.007353154,
   0.0138287,
   0.03428715,
   0.07554538,
   0.1408625,
   0.2177018,
   0.2655295,
   0.300464,
   0.3249072,
   0.3448862,
   0.3595373,
   0.3691288,
   0.385039,
   0.4022298,
   0.4176717,
   0.4222552,
   0.4326183,
   0.4360655,
   0.4422904,
   0.4445249,
   0.4511108,
   0.4566635,
   0.4695523,
   0.465033,
   0.473547,
   0.4862176,
   0.4802123,
   0.5006648,
   0.5050242,
   0.5239134,
   0.5372279,
   0.5535865,
   0.5778908,
   0.5987778,
   0.6280475,
   0.6567978,
   0.6906349,
   0.707741,
   0.7386375,
   0.7654221,
   0.8008001,
   0.8348676};
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
   // TPaveText *pave = new TPaveText(1700,1.1,2000,1.2); // CHANGE
   // pave->SetFillColor(0);
   // TText* text = pave->AddText("13 TeV");
   // //TText* text = pave->AddText("2.8 fb^{#font[122]{\55}1} (13 TeV)");
   // text->SetTextSize(0.04);
   // pave->Draw("0");
   // TPaveText *ptitle = new TPaveText(19,0.95,70,1.05); // CHANGE
   // ptitle->SetFillColor(0);
   // TText* ttitle = ptitle->AddText("CMS Simulation");
   // ttitle->SetTextSize(0.045);
   // ptitle->Draw("0");
   TLatex latex;
   latex.DrawLatexNDC(0.14, 0.96, "#font[62]{#scale[0.8]{CMS}}");
   latex.DrawLatexNDC(0.74, 0.96, "#font[62]{#scale[0.7]{2.8 fb^{-1} (13 TeV)}}");
   c_AccEff_aMCNLO->Update();

   c_AccEff_aMCNLO->Modified();
   c_AccEff_aMCNLO->cd();
   c_AccEff_aMCNLO->SetSelected(c_AccEff_aMCNLO);
   // c_AccEff_aMCNLO->SaveAs(".pdf");
}
