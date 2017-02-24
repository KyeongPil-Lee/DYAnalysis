import os
from ROOT import TFile, TH1D
if not os.path.exists("./Local"):
	os.mkdir( "./Local" )

if "ROOTFile_DYPowheg.root" not in os.listdir("."):
	cmd_hadd = "hadd ROOTFile_DYPowheg.root ROOTFile_DYPowheg_M*.root"
	print cmd_hadd
	os.system( cmd_hadd )
	cmd_mv = "mv ROOTFile_DYPowheg_M*.root ./Local"
	print cmd_mv
	os.system( cmd_mv )

List_ROOTFile = []
print "List of ROOT files that will be merged: "
for file in os.listdir("."):
	ext = os.path.splitext(file)[-1]
	if ext == ".root":
		print "\t"+file
		List_ROOTFile.append ( file )

f_output = TFile("ROOTFile_Systematic_Eff.root", "RECREATE")

print "Loop over list of ROOT files ..."
for ROOTFile in List_ROOTFile:

	# -- make a directory -- #
	DirName = ROOTFile.split(".")[0].split("ROOTFile_")[-1]
	print "\tDirName = %s\n" % (DirName)
	f_output.cd() # -- return to top directory -- #
	f_output.mkdir( DirName )

	f_input = TFile( ROOTFile )
	f_input.cd()
	# -- loop over object in TFile -- #
	for key in f_input.GetListOfKeys():
		obj = key.ReadObj()

		f_output.cd( DirName ) # -- move to sub directory -- #
		obj.Write()
		
		f_input.cd()

	f_input.Close()

print "Merging root files are finished: Output file = ", f_output.GetName()
f_output.Close()