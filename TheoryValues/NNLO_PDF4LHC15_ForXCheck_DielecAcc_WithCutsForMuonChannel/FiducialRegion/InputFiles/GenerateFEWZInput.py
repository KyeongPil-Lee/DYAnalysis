import sys
import os
import time

class GenerateFEWZInput:
	def __init__(self):
		self.TIME = time.strftime('%Y%m%d', time.localtime(time.time()))
		self.LeptonMass = "1.05d-1" # -- muon -- #
		self.Alpha0 = 0;
		self.AlphaEff = 0.007756146746;
		self.RelUnc = 0.1; # -- % -- #
		self.Order_pQCD = 2; # -- 0=LO, 1=NLO, 2=NNLO -- #
		self.AccFlag = 0; # -- full phase space -- #
		self.nCore = 20;
		self.PDFset = "NNPDF30_nnlo_as_0118"

		self.List_cmd_local_run = []
		self.List_cmd_finish = []

	def Set_Lepton(self, name):
		if name == "Muon":
			self.LeptonMass = "1.05d-1"
		elif name == "Electron":
			self.LeptonMass = "5.109989461d-4"
		else:
			print "Wrong lepton flavor! ... only Muon and Electron are provided. Lepton mass is set as -1"
			self.LeptonMass = "-1"

	def Set_MassBinEdge(self, _List_BinEdge ):
		self.List_BinEdge = _List_BinEdge
		print "Total # jobs: %d\n" %(len(self.List_BinEdge)-1)
		print "MassBinEdges: \n", self.List_BinEdge

	def Generate_AllBins(self):

		BatchFileNames = []
		for i in range( 0, len(self.List_BinEdge)-1 ):
			LowerEdge = self.List_BinEdge[i]
			UpperEdge = self.List_BinEdge[i+1]

			# print "%.0f < M < %.0f" % (LowerEdge, UpperEdge)

			FileName_Hist = self.Generate_HistFileName( LowerEdge, UpperEdge )

			InputFileName = self.Generate_InputFile( LowerEdge, UpperEdge )
			BatchFileName = self.Generate_BatchJobScript( InputFileName, FileName_Hist )
			BatchFileNames.append( BatchFileName )

		self.Generate_ScriptForAllSubmission( BatchFileNames )
		self.Generate_BatchJobScript_AllJobs()

		self.Print_AllParameters()

	def Print_AllParameters( self ):
		print "#" * 100
		print "All relavant parameters"
		print "\t[version]", self.TIME
		print "\t[Lepton mass]", self.LeptonMass
		print "\t[PDF set]", self.PDFset
		print "\t[QCD order (LO=0, NLO=1, NNLO=2)]", self.Order_pQCD
		print "\t[Flag_Acc]", self.AccFlag
		print "\t[alpha_0]", self.Alpha0
		print "\t[Alpha_Eff]", self.AlphaEff
		print "\t[Required Relative uncertainty (%)]", self.RelUnc
		print "\t[# cores]", self.nCore
		print "\t[Mass bin edges]", self.List_BinEdge
		print "#" * 100


	def Generate_HistFileName( self, LowerEdge, UpperEdge ):
		FileName = "v%s_histograms_M%.0fto%.0f.txt" % (self.TIME, LowerEdge, UpperEdge)

		# -- # mass bin -- #
		nBin = 10;	
		# BinCenter = (LowerEdge + UpperEdge) / 2.0;
		# if BinCenter < 320:
		# 	nBin = UpperEdge - LowerEdge
		# else:
		# 	nBin = (UpperEdge - LowerEdge) / 2.0

		f = open(FileName, "w")
		f.write(
"""HISTOGRAMS------(Order of Histograms Can Not Be Changed)----------------------
Name (DO NOT CHANGE)    Num Bins (<30)	Lower Bound	Upper Bound	Write Out (1=hist, 2=cuml, 3=both1&2, 4=rev-cuml, 5=both1&4, 0=none)
'1.  Z/W pT           ' 25		0d0		250d0		1
'2.  Z/W eta          ' 24		-9.6d0		9.6d0		1
'3.  Q_ll inv mass    ' {_nBin:.0f}		{_LowerEdge:.0f}d0		{_UpperEdge:.0f}d0		1
'4.  l-/lep. pT       ' 25		0d0		100d0		1
'5.  l-/lep. eta      ' 24		-9.6d0		9.6d0		1
'6.  l+/neut. pT      ' 25		0d0		100d0		1
'7.  l+/neut. eta     ' 24		-9.6d0		9.6d0		1
'8.  jet 1 pT         ' 20		0d0		100d0		1
'9.  jet 1 eta        ' 20		-5d0		5d0		1
'10. jet 2 pT         ' 20		0d0		100d0		1
'11. jet 2 eta        ' 20		-5d0		5d0		1
'12. photon pT        ' 20              0d0             100d0           1
'13. photon eta       ' 20              -5d0            5d0             1
'14. beam thrust      ' 20              0d0             100d0           1
'15. dR_l-l+          ' 20		0d0		5d0		1
'16. dR_j1,l-	      '	20		0d0		5d0		1
'17. dR_j1,l+	      '	20		0d0		5d0		1
'18. dR_j2,l-	      '	20		0d0		5d0		1
'19. dR_j2,l+	      '	20		0d0		5d0		1
'20. dR_j1j2          ' 20		0d0		5d0		1
'21. dR_phot,l+-      ' 20              0d0             5d0             1
'22. H_T	      ' 20		0d0		200d0		1
'23. Mass_T           ' 20		0d0		1000d0		1
'24. A_FB vs Q_ll     ' {_nBin:.0f}              {_LowerEdge:.0f}d0            {_UpperEdge:.0f}d0           1
====================================================================================
Moments (A_0, A_1, A_2) related to Collins-Soper Angles
'25. A_i vs Z pT      ' 10		0d0		100d0		1
'26. phi (CS Frame)   '	10		-3.14159265d0	3.14159265d0	1
'27. cos(theta) (CS)  '	10		-1d0		1d0		1
'28. dPhi_l-l+        ' 10		0d0		3.1415927d0	1
====================================================================================
Smoothing parameters
'Level (0 = none)     '  2
'Bin fraction (< 0.5) ' 1d-1
Method of combining iterations (0 = more reliable err. estimation; 1 = more consistent with tot. x-section)
'Method choice = ' 0
Histogram bin display (0 = bin central value, -1 = bin low edge, 1 = bin upper edge)
'Display option =' 0
	""".format( _LowerEdge=LowerEdge, _UpperEdge=UpperEdge, _nBin=nBin )
		)
		f.close()

		print "[%s is created]" % FileName

		return FileName


	def Generate_InputFile( self, LowerEdge, UpperEdge ):
		FileName = self.Make_FileName( LowerEdge, UpperEdge )
		leadPtCut = 0;
		subPtCut = 0;
		etaCut = 0;
		if self.AccFlag:
			leadPtCut = 22;
			subPtCut = 10;
			etaCut = 2.4;
		else:
			leadPtCut = 0;
			subPtCut = 0;
			etaCut = 10000;

		scale = int( (LowerEdge + UpperEdge) / 2.0 )
		print "[%.1d < M < %.1d] scale = %.0f" % (LowerEdge, UpperEdge, scale)

		f = open(FileName, "w")
		f.write(
"""=============================================
'CMS collision energy (GeV)    = ' 13000d0
=============================================
'Factorization scale  (GeV)    = ' {_scale:0.0f}.0d0
'Renormalization scale  (GeV)  = ' {_scale:0.0f}.0d0
=============================================
'Z production (pp=1,ppbar=2)   = ' 1
=============================================
Set to Alpha QED for incoming photon to 0 to turn off photon-induced (photon PDF dependent) channels
'Alpha QED for incoming photon = ' {_Alpha0}d0
'Alpha QED effective           = ' {_AlphaEff}d0
'Fermi constant (1/Gev^2)      = ' 1.16637d-5
=============================================
'Lepton mass (GeV)             = ' {_LeptonMass}
'W mass (GeV)                  = ' 80.403d0
'W width (GeV)                 = ' 2.141d0
'Z mass (GeV)                  = ' 91.1876d0
'Z width (GeV)                 = ' 2.4952d0
'Top mass (GeV)                = ' 172.9d0
'Higgs mass (GeV)              = ' 125d0
=============================================
Only QED corrections is on if the input scheme is manual
Input scheme: 0. Manual input; 1. Gmu scheme; 2. AlphaMz scheme; 3. Alpha0 scheme
'Which input scheme:           = ' 1
'sin^2(theta_w)                = ' 0.22255d0
'up quark charge               = ' 0.6666667d0
'down quark charge             = ' -0.3333333d0
'lepton charge                 = ' -1d0
'up quark vector coupling      = ' 0.4091d0
'down quark vector coupling    = ' -0.7045d0
'lepton vector coupling        = ' -0.11360d0
'up quark axial coupling       = ' -1d0
'down quark axial coupling     = ' 1d0
'lepton axial coupling         = ' 1d0
=============================================
Vegas Parameters
'Relative accuracy (in %)           = ' {_RelUnc:.1f}d0
'Absolute accuracy                  = ' 0d0
'Number of calls per iteration      = ' 1000000
'Number of increase calls per iter. = ' 500000
'Maximum number of evaluations      = ' 200000000
'Random number seed for Vegas       = ' 11
=============================================
'QCD Perturb. Order (0=LO, 1=NLO, 2=NNLO) = ' {_Order_pQCD:.0f}
'EW Perturb. Order (0=LO, 1=NLO)    = ' 1
'Z pole focus (1=Yes, 0=No)	= ' 0
'EW control (leave 0 to keep all on) = ' 0 
'Turn off photon (1=Yes, 0=No, disabled if weak corr. is on) = ' 0
=============================================
'Lepton-pair invariant mass minimum = ' {_LowerEdge:.0f}.0d0
'Lepton-pair invariant mass maximum = ' {_UpperEdge:.0f}.0d0
'Transverse mass minimum           = ' 0d0
'Transverse mass maximum           = ' 100000d0
'Z pT minimum                       = ' 0d0
'Z pT maximum                       = ' 100000d0
'Z rapidity minimum                 = ' -2000d0
'Z rapidity maximum                 = ' 2000d0
'Lepton pT minimum                  = ' 0d0
'Lepton pT maximum                  = ' 100000d0
'Anti-lepton pT minimum             = ' 0.0d0
'Anti-lepton pT maximum             = ' 100000d0
'pT min for softer lepton           = ' {_subPtCut:.0f}.0d0
'pT max for softer lepton           = ' 100000d0
'pT min for harder lepton           = ' {_leadPtCut:.0f}.0d0
'pT max for harder lepton           = ' 100000d0
Taking absolute value of lepton pseudorapidity?
'(yes = 1, no = 0) 		    = ' 1
'Lepton pseudorapidity minimum      = ' 0d0
'Lepton pseudorapidity maximum      = ' 100.0d0
Taking absolute value of anti-lepton pseudorapidity?
'(yes = 1, no = 0) 		    = ' 1
'Anti-lepton pseudorapidity minimum = ' 0d0
'Anti-lepton pseudorapidity maximum = ' 100.0d0
Taking absolute value of soft lepton pseudorapidity?
'(yes = 1, no = 0)                  = ' 1
'Softer lepton pseudorapidity min   = ' 0d0 
'Softer Lepton pseudorapidity max   = ' {_etaCut}d0
Taking absolute value of hard lepton pseudorapidity?
'(yes = 1, no = 0)                  = ' 1
'Harder lepton pseudorapidity min   = ' 0d0
'Harder Lepton pseudorapidity max   = ' {_etaCut}d0
PHOTON RECOMBINATION-----------------------------
'DeltaR sep. for photon recomb.     = ' 0.1d0
'Minimum pT for observable photon   = ' 10d0
'Maximum eta for observable photon  = ' 2.5d0
PHOTON CUTS--------------------------------------
'Minimum Number of Photon           = ' 0
'Maximum Number of Photon           = ' 1
JET DEFINITION-------------------------------
Jet Algorithm & Cone Size ('ktal'=kT algorithm, 'aktal'=anti-kT algorithm, 'cone'=cone)
'ktal, aktal or cone		    = ' ktal
'Jet algorithm cone size (deltaR)   = ' 0.4d0
'DeltaR separation for cone algo    = ' 1.3
'Minimum pT for observable jets     = ' 20d0
'Maximum eta for observable jets    = ' 4.5d0
JET CUTS--------------------------------------
'Minimum Number of Jets		    = ' 0
'Maximum Number of Jets		    = ' 2
'Min. leading jet pT                = ' 0d0
ISOLATION CUTS-------------------------------
'Lep-Anti-lep deltaR minimum        = ' 0.0d0
'Lep-Anti-lep deltaPhi min	    = ' 0.0d0
'Lep-Anti-lep deltaPhi max	    = ' 4.0d0
'Lep-Jet deltaR minimum             = ' 0.0d0
'Lep-Photon deltaR minimum          = ' 0.0d0
=============================================
Cut on Z rapidity for well-defined Collins-Soper Angles at pp Collider
'Z rapidity cutoff for CS frame     = ' 0.0d0
=============================================
(See manual for complete listing)
'PDF set =                        ' '{_PDFset}'
'Turn off PDF error (1=Yes, 0=No)    = ' 0
(Active for MSTW2008 only, if PDF error is on:)
(Compute PDF+as errors: 1; just PDF errors: 0)
'Which alphaS                       = ' 0
(Active for MSTW2008 only; 0: 90 CL for PDFs+alphas, 1: 68 CL)
'PDF+alphas confidence level        = ' 1
=============================================""".format( 
			_scale=scale, _Alpha0=self.Alpha0, _AlphaEff=self.AlphaEff, 
			_LowerEdge=LowerEdge, _UpperEdge=UpperEdge, _RelUnc=self.RelUnc, 
			_Order_pQCD=self.Order_pQCD, _leadPtCut=leadPtCut, _subPtCut=subPtCut, _etaCut=etaCut, _PDFset=self.PDFset, _LeptonMass=self.LeptonMass )
		)
		f.close()

		print "[%s is created]" % FileName

		return FileName

	def Make_FileName( self, LowerEdge, UpperEdge ):
		MassRange = "M%.0fto%.0f" % (LowerEdge, UpperEdge )
		
		Process = "DY"
		if self.Alpha0 == 0:
			Process = "DY"
		else:
			Process = "DYPI"

		# UncName = "%.1fPercent" % RelUnc;

		OrderName = self.Get_OrderName()
		AccName = ""
		if self.AccFlag == 0:
			AccName = "4pi"
		else:
			AccName = "WithinAcc"

		# return "input_"+MassRange+"_"+Process+"_"+OrderName+"_"+PDFset+".txt"
		return "input_v%s_" % (self.TIME) + MassRange+"_"+Process+"_"+OrderName+"_"+AccName+"_"+self.PDFset+".txt"

	def Get_OrderName(self):
		OrderName = ""
		if self.Order_pQCD == 0:
			OrderName = "LO"
		elif self.Order_pQCD == 1:
			OrderName = "NLO"
		elif self.Order_pQCD == 2:
			OrderName = "NNLO"

		return OrderName;

	def Generate_BatchJobScript( self, InputFileName, HistogramFileName ):
		BatchFileName = InputFileName.replace("input_", "b_").replace("txt", "sh")
		DirName = BatchFileName.split(".sh")[0].split("b_")[1]
		OrderName = self.Get_OrderName()

		cmd_local_run = "./local_run.sh z {_DirName} {_InputFileName} {_HistogramFileName} {_DirName}.dat ../ {_nCore} >&log_{_DirName}".format(
			_DirName=DirName, _InputFileName=InputFileName, _HistogramFileName=HistogramFileName, _nCore = self.nCore)
		cmd_finish = "./finish.sh {_DirName} {_OrderName}.{_DirName}.dat".format( _DirName=DirName, _OrderName=OrderName)

		self.List_cmd_local_run.append( cmd_local_run )
		self.List_cmd_finish.append( cmd_finish )

		f = open(BatchFileName, "w")
		f.write(
"""#!/bin/bash

#########################################################
# -- qsub commands: #$ + command (details: man qsub) -- #
#########################################################
# -- shell used for executing the script -- #
#$ -S /bin/sh

# -- combine standard output & error file -- #
#$ -j y

# -- send the mail when the job is aborted or ended -- #
#$ -m ae -M kplee@cern.ch

# -- stay in the directory where qsub command is executed -- #
#$ -cwd

cwd=$(pwd)

export SCRAM_ARCH=slc6_amd64_gcc530
export VO_CMS_SW_DIR=/cvmfs/cms.cern.ch
source $VO_CMS_SW_DIR/cmsset_default.sh

# -- CMSSW enviornment -- #
cd /cvmfs/cms.cern.ch/slc6_amd64_gcc530/cms/cmssw/CMSSW_8_0_13
cmsenv

cd ${{cwd}}

{_cmd_local_run}
{_cmd_finish}

echo "job is completed"

# -- &>log: "Invalid null command" Error occurs. please use >&log. -- #

# -- PLEASE ENTER AFTER THE LAST LINE! ... IF YOU DON'T, LAST LINE WILL NOT BE EXECUTED BY BATCH JOB -- # 
""".format(_cmd_local_run=cmd_local_run, _cmd_finish=cmd_finish)
		)
		print "[%s is created]" % BatchFileName
		f.close()

		return BatchFileName

	def Generate_ScriptForAllSubmission( self, BatchFileNames ):
		f = open("v%s_qsub_all.sh" % (self.TIME), "w")
		f.write("#!/bin/bash\n")

		for BatchFileName in BatchFileNames:
			QsubName = BatchFileName.split("_")[2] + "_" + BatchFileName.split("_")[3]
			f.write("qsub -N %s %s\n" % (QsubName, BatchFileName) )

		f.write('\necho "job submission is finished"\n\n')
		f.close()

		print "[v%s_qsub_all.sh is created]" % (self.TIME)

	def Generate_BatchJobScript_AllJobs(self):

		FileName = self.Make_FileName( self.List_BinEdge[0], self.List_BinEdge[-1] )
		FileName = FileName.replace("input_", "script_All_").replace("txt", "sh")

		List_InputFileName = []
		List_HistogramFileName = []
		List_DirName = []

		List_cmd_total = []

		f = open(FileName, "w")
		f.write(
"""#!/bin/bash

#########################################################
# -- qsub commands: #$ + command (details: man qsub) -- #
#########################################################
# -- shell used for executing the script -- #
#$ -S /bin/sh

# -- combine standard output & error file -- #
#$ -j y

# -- send the mail when the job is aborted or ended -- #
#$ -m ae -M kplee@cern.ch

# -- stay in the directory where qsub command is executed -- #
#$ -cwd

cwd=$(pwd)

export SCRAM_ARCH=slc6_amd64_gcc530
export VO_CMS_SW_DIR=/cvmfs/cms.cern.ch
source $VO_CMS_SW_DIR/cmsset_default.sh

# -- CMSSW enviornment -- #
cd /cvmfs/cms.cern.ch/slc6_amd64_gcc530/cms/cmssw/CMSSW_8_0_13
cmsenv

cd ${cwd}

"""
		)
		for i in range( 0, len(self.List_cmd_local_run) ):
			f.write( self.List_cmd_local_run[i] )
			f.write( "\n" )
			f.write( self.List_cmd_finish[i] )
			f.write( "\n\n" )

		f.write(
"""

echo "job is completed"

# -- &>log: "Invalid null command" Error occurs. please use >&log. -- #

# -- PLEASE ENTER AFTER THE LAST LINE! ... IF YOU DON'T, LAST LINE WILL NOT BE EXECUTED BY BATCH JOB -- # 
"""
		)
		print "[%s is created]" % FileName
		f.close()

		return FileName

if __name__ == "__main__":
	tool = GenerateFEWZInput()

	############################
	# -- individual setting -- #
	############################
	tool.Set_Lepton( "Electron" )
	tool.AccFlag = 1
	tool.PDFset = "PDF4LHC15_nnlo_100"
	# tool.PDFset = "NNPDF30_nnlo_as_0118"
	# tool.PDFset = "MMHT2014nnlo68cl"
	# tool.PDFset = "HERAPDF15NNLO_EIG"
	# tool.PDFset = "CT14nnlo"
	# tool.PDFset = "abm12lhc_5_nnlo"
	MassBinEdges = [
				 15, 20, 25, 30, 35, 40, 45, 50, 55, 60
				 # 64, 68, 72, 76, 81, 86, 91, 96, 101, 106,
				 # 110, 115, 120, 126, 133, 141, 150, 160, 171, 185,
				 # 200, 220, 243, 273, 320, 380, 440, 510, 600, 700,
				 # 830, 1000, 1200, 1500, 2000, 3000
				 ]
	tool.Set_MassBinEdge( MassBinEdges )

	##################
	# -- Generate -- #
	##################
	tool.Generate_AllBins()
