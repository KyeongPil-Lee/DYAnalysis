#include "DrawingTools.h"

void Draw_ControlPlots()
{
	if( gSystem->mkdir( "./Local" ) == 0 ) printf("Directory [Local] is created\n");
	
	DrawingTool *tool = new DrawingTool();
	tool->Draw_All();
}