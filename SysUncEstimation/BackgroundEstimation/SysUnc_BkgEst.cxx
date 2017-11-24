#include "SysUncTool_BkgEst.h"

void SysUnc_BkgEst(TString sampleName)
{
	SysUncTool* tool = new SysUncTool(sampleName);
	tool->Estimate();
}