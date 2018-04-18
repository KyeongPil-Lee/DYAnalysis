#include <Include/PlotTools.h>

class StatUncTool
{
	TH1D* h_ttbar;
	TH1D* h_absUncStat_ttbar;
public:
	StatUncTool()
	{
		this->Init();
	}

	void CrossCheck()
	{
		Int_t nBin = this->h_ttbar->GetNbinsX();
		for(Int_t i=0; i<nBin; i++)
		{
			Int_t i_bin = i+1;
			Double_t nEvent = this->h_ttbar->GetBinContent(i_bin);
			Double_t absUncStat = sqrt(nEvent);
			Double_t absUncStat_fromHist = this->h_absUncStat_ttbar->GetBinContent(i_bin);

			printf("[%02d bin] (nEvent, sqrt(nEvent), absUncStat) = (%.1lf, %.3lf, %.3lf)\n",
				i_bin, nEvent, absUncStat, absUncStat_fromHist);
		}
	}
private:
	void Init()
	{
		TString rootFilePath = gSystem->Getenv("KP_ROOTFILE_PATH");
		TString fileName = rootFilePath + "/ROOTFile_Bkg_DataDrivenMethod.root";
		this->h_ttbar = Get_Hist(fileName, "ttbar" );
		this->h_absUncStat_ttbar = Get_Hist(fileName, "ttbar_stat" );

		// Print_Histogram( h_ttbar );
		// Print_Histogram( h_absUncStat_ttbar );
	}
};

void StatUnc_DataDrivenBkg()
{
	StatUncTool* tool = new StatUncTool();
	tool->CrossCheck();
}