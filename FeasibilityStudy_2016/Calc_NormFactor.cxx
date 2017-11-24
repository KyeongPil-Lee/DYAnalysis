#define Lumi 35867.060 // -- pb -- //

class SampleInfo
{	
public:
	TString Type;
	Double_t XSec;
	Int_t nEvent;
	Double_t NormFactor;

	SampleInfo( TString _Type, Double_t _XSec, Double_t _nEvent )
	{
		this->Type = _Type;
		this->XSec = _XSec;
		this->nEvent = _nEvent;
		this->NormFactor = (Lumi * XSec ) / (Double_t)nEvent;

		this->Print_Info();
	}

	void Print_Info()
	{
		printf("[%15s] Norm. factor = (%8.3lf * %12.5lf) / %15d = %.10lf\n", Type.Data(), Lumi, XSec, nEvent, NormFactor);
	}
};

void Calc_NormFactor()
{
	SampleInfo *DY_M10to50 = new SampleInfo( "DY_M10to50", 18610, 65888233 );
	SampleInfo *DY_M50toInf = new SampleInfo( "DY_M50toInf", 6104, 122055388 );
	SampleInfo *DY_M100to200 = new SampleInfo( "DY_M100to200", 226, 1083606 );
	SampleInfo *DY_M200to400 = new SampleInfo( "DY_M200to400", 7.67, 295242 );
	SampleInfo *DY_M400to500 = new SampleInfo( "DY_M400to500", 0.423, 287262 );
	SampleInfo *DY_M500to700 = new SampleInfo( "DY_M500to700", 0.24, 280940 );
	SampleInfo *DY_M700to800 = new SampleInfo( "DY_M700to800", 0.035, 276235 );
	SampleInfo *DY_M800to1000 = new SampleInfo( "DY_M800to1000", 0.03, 271768 );
	SampleInfo *DY_M1000to1500 = new SampleInfo( "DY_M1000to1500", 0.016, 258620 );
	SampleInfo *DY_M1500to2000 = new SampleInfo( "DY_M1500to2000", 0.002, 258625 );
	SampleInfo *DY_M2000to3000 = new SampleInfo( "DY_M2000to3000", 0.00054, 255342 );

	SampleInfo *WJetsTo2L2Nu = new SampleInfo( "WJetsTo2L2Nu", 6.15E+04, 235575850 );
	SampleInfo *ttbar = new SampleInfo( "ttbar", 831.76, 154948894 );
	SampleInfo *tW = new SampleInfo( "tW", 35.6, 6952830 );
	SampleInfo *tbarW = new SampleInfo( "tbarW", 35.6, 6933094 );
	SampleInfo *WW = new SampleInfo( "WW", 118.7, 6987124 );
	SampleInfo *WZ = new SampleInfo( "WZ", 47.13, 2995828 );
	SampleInfo *ZZ = new SampleInfo( "ZZ", 16.523, 998034 );

	SampleInfo *ttbarTo2L2Nu = new SampleInfo( "ttbarTo2L2Nu", 87.31, 79092400 );
	SampleInfo *WWTo2L2Nu = new SampleInfo( "WWTo2L2Nu", 10.481, 1999000 );
	SampleInfo *WZTo3L1Nu = new SampleInfo( "WZTo3L1Nu", 4.43, 17990100 );
	SampleInfo *ZZTo4L = new SampleInfo( "ZZTo4L", 1.256, 96581110 );
}