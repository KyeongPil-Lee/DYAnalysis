#include "sumWeight_ele.cxx"

void sumWeight_ele_M100(TString ntuplePath)
{
  SumWeightTool* tool = new SumWeightTool(ntuplePath);
  tool->DoTruncate_M100_ = kTRUE;
  tool->Calculate();
}
