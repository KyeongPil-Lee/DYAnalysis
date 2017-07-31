#include "XCheck_Kinematics_EachSample.h"

void XCheck_Kinematics_EachSample()
{
	TString FileName_KP = "ROOTFile_Acc_Dressed_EE.root";
	TString FileName_Ridhi = "ROOTFile_DYEE_AccTest_Input_Electron_Ridhi.root";

	XCheckTool *tool = new XCheckTool();
	tool->Set_FileName( FileName_KP, FileName_Ridhi );
	tool->DrawPlot_All();
}