from ROOT import TH1D, TFile
from array import array
from math import sqrt

MassBinEdge = [
15, 20, 25, 30, 35, 40, 45, 50, 55, 60,
64, 68, 72, 76, 81, 86, 91, 96, 101, 106,
110, 115, 120, 126, 133, 141, 150, 160, 171, 185,
200, 220, 243, 273, 320, 380, 440, 510, 600, 700,
830, 1000, 1500, 3000]


h_Acc = TH1D("h_Acc", "", len(MassBinEdge)-1, array("d", MassBinEdge) )

f = open("PDF4LHC15_nnlo_100_acc_AcceptanceNumbers2.txt", "r")

lines = f.readlines()
i_bin = 1;
for line in lines:
	numbers = line.split(" ")
	# print numbers
	Acc = float(numbers[1])
	Integ_Err = float(numbers[2])
	PDFErr_plus = float(numbers[3])
	PDFErr_minus = float(numbers[4])

	print "%.5lf\t%.5lf\t%.5lf\t%.5lf" % (Acc, Integ_Err, PDFErr_plus, PDFErr_minus)

	if PDFErr_minus > PDFErr_plus:
		Error = sqrt( PDFErr_minus*PDFErr_minus + Integ_Err*Integ_Err )
	else:
		Error = sqrt( PDFErr_plus*PDFErr_plus + Integ_Err*Integ_Err )

	h_Acc.SetBinContent(i_bin, Acc)
	h_Acc.SetBinError(i_bin, Error)
	i_bin = i_bin + 1

File = TFile("ROOTFile_Acc_Electron_FEWZ_PDF4LHC15.root", "RECREATE")
File.cd()
h_Acc.Write()
File.Close()
