#include <ElecChannel/Include/DYElectron.h>
#include <Include/SimplePlotTools.h>
#include <TH1D.h>
#include <TString.h>
#include <TSystem.h>
#include <iostream>

namespace DYTool
{
  Double_t LUMI_ELE = 2258.066; // -- /pb

  vector<DYGenEle_preFSR> GetAllGenEle_preFSR(NtupleHandle_Ele* ntuple, Double_t minPt = -1)
  {
    vector<DYGenEle_preFSR> vec_ele;

    Int_t nEle = (Int_t)ntuple->genEle_preFSR_pt->size();
    for(Int_t i_ele=0; i_ele<nEle; i_ele++)
    {
      DYGenEle_preFSR ele(ntuple, i_ele);
      if( ele.pt > minPt )
        vec_ele.push_back( ele );
    }

    return vec_ele;
  }

  vector<DYGenEle_postFSR> GetAllGenEle_postFSR(NtupleHandle_Ele* ntuple, Double_t minPt = -1)
  {
    vector<DYGenEle_postFSR> vec_ele;

    Int_t nEle = (Int_t)ntuple->genEle_postFSR_pt->size();
    for(Int_t i_ele=0; i_ele<nEle; i_ele++)
    {
      DYGenEle_postFSR ele(ntuple, i_ele);
      if( ele.pt > minPt )
        vec_ele.push_back( ele );
    }

    return vec_ele;
  }

  vector<DYElectron> GetAllElectron(NtupleHandle_Ele* ntuple, Double_t minPt = -1 )
  {
    vector<DYElectron> vec_ele;
    Int_t nEle = ntuple->nEle;
    for(Int_t i_ele=0; i_ele<nEle; i_ele++)
    {
      DYElectron ele(ntuple, i_ele);
      if( ele.pt > minPt )
        vec_ele.push_back( ele );
    }
    return vec_ele;
  }

  Bool_t DoPassAcc_ee(Double_t pt1, Double_t eta1, Double_t pt2, Double_t eta2)
  {
    Bool_t flag = kFALSE;

    Double_t leadPt, leadEta, subPt, subEta;
    if( pt1 > pt2 )
    {
      leadPt  = pt1;
      leadEta = eta1;
      subPt   = pt2;
      subEta  = eta2;
    }
    else
    {
      leadPt  = pt2;
      leadEta = eta2;
      subPt   = pt1;
      subEta  = eta1;
    }

    if( leadPt > 30. && fabs(leadEta) < 2.5 && !( fabs(leadEta) > 1.4442 && fabs(leadEta) < 1.566 ) &&
        subPt  > 10. && fabs(subEta) < 2.5  && !( fabs(subEta) > 1.4442  && fabs(subEta) < 1.566 ) )
      flag = kTRUE;

    return flag;
  }

  TH1D* MakeHist_DXSecBin(TString histName)
  {
    Double_t arr_massBinEdge[44] = {15,20,25,30,35,40,45,50,55,60,64,68,72,76,81,86,91,96,101,106,110,115,120,126,133,141,150,160,171,185,200,220,243,273,320,380,440,510,600,700,830,1000,1500,3000};
    return new TH1D(histName, "", 43, arr_massBinEdge);
  }

  // -- used for cov. M
  TH2D* MakeHist2D_DXSecBin(TString histName)
  {
    Double_t arr_massBinEdge[44] = {15,20,25,30,35,40,45,50,55,60,64,68,72,76,81,86,91,96,101,106,110,115,120,126,133,141,150,160,171,185,200,220,243,273,320,380,440,510,600,700,830,1000,1500,3000};
    return new TH2D(histName, "", 43, arr_massBinEdge, 43, arr_massBinEdge);
  }

  // -- select DY->ee event @ pre-FSR level (to separate from DY->mm and DY->tautau events)
  Bool_t IsDYEEEvent(NtupleHandle_Ele* ntuple, Bool_t doTruncate_M100 = kFALSE)
  {
    Bool_t flag = kFALSE;

    vector<DYGenEle_preFSR> vec_ele = DYTool::GetAllGenEle_preFSR(ntuple);

    if( vec_ele.size() == 2 )
    {
      if( doTruncate_M100 )
      {
        Double_t mass = (vec_ele[0].vec_P + vec_ele[1].vec_P).M();
        if( mass < 100 )
          flag = kTRUE;
      }
      else
        flag = kTRUE;
    }

    return flag;
  }

  TH2D* Convert_CovMToCorrM( TH2D* h_cov, TString histName )
  {
    TH2D* h_corr = (TH2D*)h_cov->Clone();
    h_corr->SetName(histName);
    Int_t nBin = h_cov->GetNbinsX();

    vector<Double_t> vec_sigma; // -- uncertainties in each bin
    for(Int_t i=0; i<nBin; i++)
    {
      Int_t i_bin = i+1;
      Double_t variance = h_cov->GetBinContent(i_bin, i_bin);
      Double_t sigma = sqrt(variance);
      vec_sigma.push_back(sigma);
    }

    for(Int_t i=0; i<nBin; i++)
    {
      Int_t i_bin = i+1;
      Double_t sigma_i = vec_sigma[i];

      for(Int_t j=0; j<nBin; j++)
      {
        Int_t j_bin = j+1;
        Double_t sigma_j = vec_sigma[j];

        Double_t cov = h_cov->GetBinContent(i_bin, j_bin);
        Double_t corr = cov / (sigma_i * sigma_j);
        h_corr->SetBinContent(i_bin, j_bin, corr);
      }
    }

    return h_corr;
  }

  class PUWeightTool
  {
  public:
    TH1D* h_PU_data_;
    TH1D* h_PU_MC_;
    TH1D* h_PUWeight_;

    PUWeightTool()
    {
      Init();
    }

    Double_t Weight(Int_t truePU)
    {
      Int_t binNum = h_PUWeight_->GetXaxis()->FindBin(truePU);
      return h_PUWeight_->GetBinContent(binNum);
    }

  private:
    void Init()
    {
      TString analyzerPath    = gSystem->Getenv("KP_ANALYZER_PATH");
      TString includePath_ele = analyzerPath + "/ElecChannel/Include";
      TString fileName_data   = includePath_ele+"/dataPUDist.root";
      TString fileName_MC     = includePath_ele+"/PileUp_MC.root";

      h_PU_data_ = PlotTool::Get_Hist(fileName_data, "pileup");
      h_PU_data_->Scale( 1.0 / h_PU_data_->Integral() );

      h_PU_MC_   = PlotTool::Get_Hist(fileName_MC, "pileup_MC");

      h_PUWeight_ = (TH1D*)h_PU_data_->Clone();
      h_PUWeight_->Divide(h_PU_MC_);
    }
  };

  class SampleInfo
  {
  public: 
    TString ntuplePath_;
    Double_t xSec_;
    Double_t sumWeight_;

    // -- only for signal mass-binned samples
    Double_t minM_;
    Double_t maxM_;

    SampleInfo()
    {
      minM_ = -1;
      maxM_ = -1;
    }

    SampleInfo( TString ntuplePath, Double_t xSec, Double_t sumWeight ) : SampleInfo()
    {
      ntuplePath_ = ntuplePath;
      xSec_       = xSec;
      sumWeight_  = sumWeight;
    }

    SampleInfo( TString ntuplePath, Double_t xSec, Double_t sumWeight, Double_t minM, Double_t maxM ) : SampleInfo(ntuplePath, xSec, sumWeight)
    {
      minM_ = minM;
      maxM_ = maxM;
    }

    Double_t normFactor()
    {
      return (LUMI_ELE * xSec_) / sumWeight_;
    }

  private:
  };

  vector<SampleInfo> GetSampleInfo_Signal_Ridhi()
  {
    // double xsec[11] = {18610./3,5870./3, 226./3, 7.67/3, 0.423/3, 0.24/3, 0.035/3, 0.03/3, 0.016/3, 0.002/3, 0.00054/3};
    // double sumofWts[11] = {771413889185.162476, 144505031098.323120, 219889705.060318, 7008766.904321, 122987.746342, 69998.861766, 11080.956088, 9415.627747, 4893.463037, 664.033593, 159.615701};

    TString dataPath = gSystem->Getenv("KP_DATA_PATH");

    vector<SampleInfo> vec_sample = 
    {
      SampleInfo(dataPath+"/DY_10to50.root",     18610./3,  771413889185.162476, 10,  50),
      SampleInfo(dataPath+"/DY_50toInf.root",    5870./3,   144505031098.323120, 50,  100),
      SampleInfo(dataPath+"/DY_100to200.root",   226./3,    219889705.060318,    100, 200),
      SampleInfo(dataPath+"/DY_200to400.root",   7.67/3,    7008766.904321,      200, 400),
      SampleInfo(dataPath+"/DY_400to500.root",   0.423/3,   122987.746342,       400, 500),
      SampleInfo(dataPath+"/DY_500to700.root",   0.24/3,    69998.861766,        500, 700),
      SampleInfo(dataPath+"/DY_700to800.root",   0.035/3,   11080.956088,        700, 800),
      SampleInfo(dataPath+"/DY_800to1000.root",  0.03/3,    9415.627747,         800, 1000),
      SampleInfo(dataPath+"/DY_1000to1500.root", 0.016/3,   4893.463037,         1000, 1500),
      SampleInfo(dataPath+"/DY_1500to2000.root", 0.002/3,   664.033593,          1500, 2000),
      SampleInfo(dataPath+"/DY_2000to3000.root", 0.00054/3, 159.615701,          2000, 3000),
    };

    return vec_sample;
  }

  // -- sum of weights: calculated myself
  vector<SampleInfo> GetSampleInfo_Signal()
  {
    TString dataPath = gSystem->Getenv("KP_DATA_PATH");

    vector<SampleInfo> vec_sample = 
    {
      SampleInfo(dataPath+"/DY_10to50.root",     18610./3,  7.58772E+11, 10,  50),
      SampleInfo(dataPath+"/DY_50toInf.root",    5870./3,   1.44504E+11, 50,  100),
      SampleInfo(dataPath+"/DY_100to200.root",   226./3,    2.19885E+08, 100, 200),
      SampleInfo(dataPath+"/DY_200to400.root",   7.67/3,    7.00873E+06, 200, 400),
      SampleInfo(dataPath+"/DY_400to500.root",   0.423/3,   122990,      400, 500),
      SampleInfo(dataPath+"/DY_500to700.root",   0.24/3,    69998.9,     500, 700),
      SampleInfo(dataPath+"/DY_700to800.root",   0.035/3,   11081,       700, 800),
      SampleInfo(dataPath+"/DY_800to1000.root",  0.03/3,    9415.63,     800, 1000),
      SampleInfo(dataPath+"/DY_1000to1500.root", 0.016/3,   4893.46,     1000, 1500),
      SampleInfo(dataPath+"/DY_1500to2000.root", 0.002/3,   664.034,     1500, 2000),
      SampleInfo(dataPath+"/DY_2000to3000.root", 0.00054/3, 159.616,     2000, 3000),
    };

    return vec_sample;
  }

  Bool_t CompareEle_LargerPt( DYElectron ele1, DYElectron ele2 )
  {
    return ele1.pt > ele2.pt; 
  }

  Bool_t TriggerMatched_Ele23WPLoose(NtupleHandle_Ele* ntuple, DYElectron recoEle )
  {
    Bool_t flag = kFALSE;

    Int_t nTrigObj = (Int_t)ntuple->ele23Obj_pt->size();

    vector<TLorentzVector> vec_vecP_trigObj;
    for(Int_t i=0; i<nTrigObj; i++)
    {
      Double_t pt  = ntuple->ele23Obj_pt->at(i);
      Double_t eta = ntuple->ele23Obj_eta->at(i);
      Double_t phi = ntuple->ele23Obj_phi->at(i);

      TLorentzVector vec_P;
      vec_P.SetPtEtaPhiM( pt, eta, phi, EleMass );
      vec_vecP_trigObj.push_back( vec_P );
    }

    for(const auto& vecP_trigObj : vec_vecP_trigObj )
    {
      Double_t dR = recoEle.vec_P.DeltaR( vecP_trigObj );
      if( dR < 0.1 )
      {
        flag = kTRUE;
        break;
      }
    }

    return flag;
  }

  Bool_t IsDYEECandidate(NtupleHandle_Ele* ntuple, vector<DYElectron>& vec_selectedEle)
  {
    Bool_t flag = kFALSE;
    vector<DYElectron> vec_recoEle = DYTool::GetAllElectron(ntuple);
    // -- sort to have electron with largest pT in the first element
    std::sort(vec_recoEle.begin(), vec_recoEle.end(), DYTool::CompareEle_LargerPt);

    Int_t nEle = (Int_t)vec_recoEle.size();
    if( nEle >= 2 )
    {
      vector<DYElectron> vec_goodRecoEle;
      for(Int_t i_ele=0; i_ele<nEle; i_ele++)
      {
        Double_t etaSC = vec_recoEle[i_ele].etaSC;
        if (vec_recoEle[i_ele].mediumID && fabs(etaSC) < 2.5 && !(fabs(etaSC) > 1.4442 && fabs(etaSC) < 1.566) )
          vec_goodRecoEle.push_back( vec_recoEle[i_ele] );
      }

      Int_t nGoodEle = (Int_t)vec_goodRecoEle.size();
      if( nGoodEle == 2 )
      {
        Bool_t flag_trigMatched = kFALSE;
        if( DYTool::TriggerMatched_Ele23WPLoose(ntuple, vec_goodRecoEle[0]) ||
            DYTool::TriggerMatched_Ele23WPLoose(ntuple, vec_goodRecoEle[1]) )
          flag_trigMatched = kTRUE;

        Bool_t flag_passAcc = DYTool::DoPassAcc_ee( vec_goodRecoEle[0].pt, vec_goodRecoEle[0].eta, vec_goodRecoEle[1].pt, vec_goodRecoEle[1].eta );

        Double_t M_reco = (vec_goodRecoEle[0].vec_P + vec_goodRecoEle[1].vec_P).M();

        if(flag_passAcc && M_reco > 10. && flag_trigMatched && ntuple->ele23_WPLoose)
        {
          flag = kTRUE;
          vec_selectedEle.push_back( vec_goodRecoEle[0] ); // -- leading electron
          vec_selectedEle.push_back( vec_goodRecoEle[1] ); // -- sub-leading electron
        }
      } // -- end of if( nGoodEle == 2 )

    } // -- end of if( nEle >= 2 )

    return flag;
  }

  static inline void loadBar(int x, int n, int r, int w)
  {
      // Only update r times.
      if( x == n )
        cout << endl;

      if ( x % (n/r +1) != 0 ) return;

   
      // Calculuate the ratio of complete-to-incomplete.
      float ratio = x/(float)n;
      int   c     = ratio * w;
   
      // Show the percentage complete.
      printf("%3d%% [", (int)(ratio*100) );
   
      // Show the load bar.
      for (int x=0; x<c; x++) cout << "=";
   
      for (int x=c; x<w; x++) cout << " ";
   
      // ANSI Control codes to go back to the
      // previous line and clear it.
    cout << "]\r" << flush;
  }
};
// -- end of namespace