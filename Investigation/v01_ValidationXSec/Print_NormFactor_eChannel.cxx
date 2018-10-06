#include <Include/DYAnalyzer.h>
#include <Include/SimplePlotTools.h>
#include <TStopwatch.h>
#include <TTimeStamp.h>

void Print_NormFactor_eChannel()
{
  DYAnalyzer *analyzer = new DYAnalyzer( "None" );

  vector< TString > ntupleDirectory; vector< TString > Tag; vector< Double_t > Xsec; vector< Double_t > nEvents;
  analyzer->SetupMCsamples_v20160309_76X_MiniAODv2("aMCNLO_ee_AdditionalSF_LargeN_M10to50", &ntupleDirectory, &Tag, &Xsec, &nEvents);

  // -- from Ridhi
  double xsec[11] = {18610./3,5870./3,226./3,7.67/3,0.423/3,0.24/3,0.035/3,0.03/3,0.016/3,0.002/3,0.00054/3};
  double sumofWts[11] = {771413889185.162476,144505031098.323120,219889705.060318,7008766.904321,122987.746342,69998.861766,11080.956088,9415.627747,4893.463037,664.033593,159.615701};

  // -- absolute weight of a event
  double absW[11] = 
  {
    41444.199000,
    23443.424000,
    963.769110,
    40.880079,
    2.439265,
    1.468282,
    0.241347,
    0.212754,
    0.121011,
    0.017809,
    0.004678
  };

  Int_t nSample = (Int_t)(ntupleDirectory.size());
  for(Int_t i=0; i<nSample; i++)
  {
    printf("[%s]\n", Tag[i].Data());

    Double_t xSec_KP = Xsec[i];
    Double_t xSec_Ridhi = xsec[i];
    Double_t relDiff_xSec = (xSec_Ridhi - xSec_KP) / xSec_KP;
    printf("   [x-section] (KP, Ridhi) = (%16.10lf, %16.10lf) -> Rel.diff = %lf\n", xSec_KP, xSec_Ridhi, relDiff_xSec);

    Double_t sumW_KP = nEvents[i];
    Double_t sumW_Ridhi = sumofWts[i] / absW[i];
    Double_t relDiff_sumW = (sumW_Ridhi - sumW_KP) / sumW_KP;
    printf("   [x-section] (KP, Ridhi) = (%15.0lf, %15.0lf) -> Rel.diff = %lf\n", sumW_KP, sumW_Ridhi, relDiff_sumW);

    printf("\n");
  }

}