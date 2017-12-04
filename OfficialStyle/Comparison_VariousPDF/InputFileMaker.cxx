#include "InputFileMaker.h"

void InputFileMaker()
{
	TString FileName_Base = "./Local/ROOTFile_Input_Comparison_VariousPDF";

	TString FileName_MM = FileName_Base + "_MM.root";
	TFile *f_output_MM = TFile::Open(FileName_MM, "RECREATE");

	TString FileName_EE = FileName_Base + "_EE.root";
	TFile *f_output_EE = TFile::Open(FileName_EE, "RECREATE");

	TString FileName_LL = FileName_Base + "_LL.root";
	TFile *f_output_LL = TFile::Open(FileName_LL, "RECREATE");

	Tool_InputFileMaker *tool = new Tool_InputFileMaker();
	tool->MakeInputFile_MM( f_output_MM );
	tool->MakeInputFile_EE( f_output_EE );
	tool->MakeInputFile_LL( f_output_LL );

	f_output_EE->Close();
	f_output_MM->Close();
	f_output_LL->Close();
}