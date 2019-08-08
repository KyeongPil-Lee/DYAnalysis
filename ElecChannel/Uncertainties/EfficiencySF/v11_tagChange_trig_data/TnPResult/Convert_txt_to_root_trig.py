from ROOT import TFile, TH1D, TH2D
from array import array
import sys
from math import sqrt

class Converter:
    def __init__(self):
        self.inputFile = ""
        self.outputFileName = ""
        self.list_etaBinEdge = []
        self.list_ptBinEdge = []
        self.list_columnTitle = []
        self.noECALGap = False # -- true if the text file doesn't include the efficiencies in ECAL gap
        self.ECALGapRange = [] # -- only used when self.noECALGap = True

        self.unfoldAbsEtaToEta = False # -- in case when the input is vs. |eta| -> the output histogram will be vs. eta to be coherent with the other efficiencies
        self.list_unfoldedEtaBinEdge = []

        self.addUnc_diffWithOld = False # -- true if the difference of the central value with the old reesult should be taken into account
        self.rootFileName_old = ""

        self.nColumn = 0

    def Convert(self):
        print "[Converter::Convert] input file: %s" % self.inputFile
        self.nColumn = len(self.list_columnTitle)

        f_input = open(self.inputFile, "r")
        lines = f_input.readlines()

        list_dic = [] # -- dictionary with all values
        for line in lines:
            list_float = self.Convert_Text_To_ListWithFloat(line)
            if len(list_float) < self.nColumn:
                print "line = %s -> does not include enough # nColumn (%d): ignored\n" % (line, self.nColumn)
                continue

            dic_value = {}
            for i in range(0, self.nColumn):
                dic_value[self.list_columnTitle[i]] = list_float[i]

            print dic_value
            list_dic.append( dic_value )

        self.MakeHistogramAndSave( list_dic )
        print "[Converter::Convert] Done\n"


    def MakeHistogramAndSave(self, list_dic):
        h_eff_data = TH2D("h_eff_data", "", len(self.list_etaBinEdge)-1, array("d", self.list_etaBinEdge), len(self.list_ptBinEdge)-1, array("d", self.list_ptBinEdge) )
        h_eff_mc   = TH2D("h_eff_mc",   "", len(self.list_etaBinEdge)-1, array("d", self.list_etaBinEdge), len(self.list_ptBinEdge)-1, array("d", self.list_ptBinEdge) )

        h_statUnc_data = TH2D("h_statUnc_data", "", len(self.list_etaBinEdge)-1, array("d", self.list_etaBinEdge), len(self.list_ptBinEdge)-1, array("d", self.list_ptBinEdge) )
        h_statUnc_mc   = TH2D("h_statUnc_mc",   "", len(self.list_etaBinEdge)-1, array("d", self.list_etaBinEdge), len(self.list_ptBinEdge)-1, array("d", self.list_ptBinEdge) )

        h_eff_tagChange_data = TH2D("h_eff_tagChange_data", "", len(self.list_etaBinEdge)-1, array("d", self.list_etaBinEdge), len(self.list_ptBinEdge)-1, array("d", self.list_ptBinEdge) )
        h_eff_nlo_mc   = TH2D("h_eff_nlo_mc",   "", len(self.list_etaBinEdge)-1, array("d", self.list_etaBinEdge), len(self.list_ptBinEdge)-1, array("d", self.list_ptBinEdge) )

        for dic_value in list_dic:
            i_etaBin = self.FindBin( dic_value["etaMin"], dic_value["etaMax"], self.list_etaBinEdge )
            i_ptBin  = self.FindBin( dic_value["ptMin"], dic_value["ptMax"], self.list_ptBinEdge )

            h_eff_data.SetBinContent(i_etaBin, i_ptBin, dic_value["eff_data"])
            h_eff_data.SetBinError(i_etaBin, i_ptBin, dic_value["statUnc_data"])

            h_eff_mc.SetBinContent(i_etaBin, i_ptBin, dic_value["eff_mc"])
            h_eff_mc.SetBinError(i_etaBin, i_ptBin, dic_value["statUnc_mc"])

            h_statUnc_data.SetBinContent(i_etaBin, i_ptBin, dic_value["statUnc_data"])
            h_statUnc_data.SetBinError(i_etaBin, i_ptBin, 0)

            h_statUnc_mc.SetBinContent(i_etaBin, i_ptBin, dic_value["statUnc_mc"])
            h_statUnc_mc.SetBinError(i_etaBin, i_ptBin, 0)

            h_eff_tagChange_data.SetBinContent(i_etaBin, i_ptBin, dic_value["eff_tagChange_data"])
            h_eff_tagChange_data.SetBinError(i_etaBin, i_ptBin, 0)

            h_eff_nlo_mc.SetBinContent(i_etaBin, i_ptBin, dic_value["eff_nlo_mc"])
            h_eff_nlo_mc.SetBinError(i_etaBin, i_ptBin, 0)


        h_diff_tagChange = self.Make2DHist_Diff(h_eff_data, h_eff_tagChange_data)
        h_diff_nlo       = self.Make2DHist_Diff(h_eff_mc,   h_eff_nlo_mc)

        h_relDiff_tagChange = self.Make2DHist_RelDiff(h_eff_data, h_eff_tagChange_data)
        h_relDiff_nlo       = self.Make2DHist_RelDiff(h_eff_mc, h_eff_nlo_mc)

        h_absRelDiff_tagChange = self.Make2DHist_AbsValue(h_relDiff_tagChange)
        h_absRelDiff_nlo = self.Make2DHist_AbsValue(h_relDiff_nlo)

        list_absRelDiffHist = [h_absRelDiff_tagChange, h_absRelDiff_nlo]
        h_absRelDiff_tot = self.Make2DHist_QuadSum(list_absRelDiffHist)
        h_absRelDiff_tot.SetName("h_absRelDiff_tot")

        f_output = TFile(self.outputFileName, "RECREATE")
        h_eff_data.Write()
        h_eff_mc.Write()
        h_statUnc_data.Write()
        h_statUnc_mc.Write()
        h_eff_tagChange_data.Write()
        h_eff_nlo_mc.Write()

        h_diff_tagChange.Write()
        h_diff_nlo.Write()

        h_relDiff_tagChange.Write()
        h_relDiff_nlo.Write()

        h_absRelDiff_tagChange.Write()
        h_absRelDiff_nlo.Write()
        h_absRelDiff_tot.Write()

        self.Make1DHist_from2DHist( h_eff_data, f_output )
        self.Make1DHist_from2DHist( h_eff_mc, f_output )
        self.Make1DHist_from2DHist( h_statUnc_data, f_output )
        self.Make1DHist_from2DHist( h_statUnc_mc, f_output )
        self.Make1DHist_from2DHist( h_eff_tagChange_data, f_output )
        self.Make1DHist_from2DHist( h_eff_nlo_mc, f_output )

        self.Make1DHist_from2DHist( h_diff_tagChange, f_output )
        self.Make1DHist_from2DHist( h_diff_nlo,       f_output )

        self.Make1DHist_from2DHist( h_relDiff_tagChange, f_output )
        self.Make1DHist_from2DHist( h_relDiff_nlo, f_output )

        self.Make1DHist_from2DHist( h_absRelDiff_tagChange, f_output )
        self.Make1DHist_from2DHist( h_absRelDiff_nlo, f_output )
        self.Make1DHist_from2DHist( h_absRelDiff_tot, f_output )

        f_output.Close()

    def FindBin(self, lowEdge, highEdge, list_binEdge):
        i_bin = -1

        for i in range(0, len(list_binEdge)):
            if list_binEdge[i] == lowEdge and list_binEdge[i+1] == highEdge:
                i_bin = i+1 # -- bin number = i+1 (start with 1, not 0)
                break

        if i_bin == -1:
            print "[FindBin] (lowEdge, highEdge) = (%f, %f): corresponding bin is not found" % (lowEdge, highEdge)
            print "   -> * input list for bin edges: ", list_binEdge
            sys.exit()

        return i_bin

    def Make2DHist_RelDiff( self, h_cv, h_alt ):
        histName_alt = h_alt.GetName()
        uncType = histName_alt.split("h_eff_")[-1]

        histName = "h_relDiff_"+uncType

        h_relDiff = TH2D(histName, "", len(self.list_etaBinEdge)-1, array("d", self.list_etaBinEdge), len(self.list_ptBinEdge)-1, array("d", self.list_ptBinEdge) )

        nPtBin  = h_cv.GetNbinsY()
        nEtaBin = h_cv.GetNbinsX()
        for i_pt in range(0, nPtBin):
            i_ptBin = i_pt+1

            for i_eta in range(0, nEtaBin):
                i_etaBin = i_eta+1

                cenValue = h_cv.GetBinContent(i_etaBin, i_ptBin)
                altValue = h_alt.GetBinContent(i_etaBin, i_ptBin)

                relDiff = 0
                if cenValue != 0: # -- if it is not ECAL gap
                    relDiff = (altValue - cenValue) / cenValue

                h_relDiff.SetBinContent(i_etaBin, i_ptBin, relDiff)
                h_relDiff.SetBinError(i_etaBin, i_ptBin, 0)

        return h_relDiff

    def Make2DHist_Diff( self, h_cv, h_alt ):
        histName_alt = h_alt.GetName()
        uncType = histName_alt.split("h_eff_")[-1]

        histName = "h_diff_"+uncType

        h_diff = TH2D(histName, "", len(self.list_etaBinEdge)-1, array("d", self.list_etaBinEdge), len(self.list_ptBinEdge)-1, array("d", self.list_ptBinEdge) )

        nPtBin  = h_cv.GetNbinsY()
        nEtaBin = h_cv.GetNbinsX()
        for i_pt in range(0, nPtBin):
            i_ptBin = i_pt+1

            for i_eta in range(0, nEtaBin):
                i_etaBin = i_eta+1

                cenValue = h_cv.GetBinContent(i_etaBin, i_ptBin)
                altValue = h_alt.GetBinContent(i_etaBin, i_ptBin)

                diff = altValue - cenValue

                h_diff.SetBinContent(i_etaBin, i_ptBin, diff)
                h_diff.SetBinError(i_etaBin, i_ptBin, 0)

        return h_diff

    def Make2DHist_AbsValue( self, h_2D ):
        h_2D_absValue = h_2D.Clone()
        histName = h_2D.GetName()
        histName = histName.replace("relDiff", "absRelDiff")
        h_2D_absValue.SetName(histName)

        nPtBin  = h_2D.GetNbinsY()
        nEtaBin = h_2D.GetNbinsX()
        for i_pt in range(0, nPtBin):
            i_ptBin = i_pt+1

            for i_eta in range(0, nEtaBin):
                i_etaBin = i_eta+1

                value = h_2D.GetBinContent(i_etaBin, i_ptBin)
                if value < 0:
                    h_2D_absValue.SetBinContent(i_etaBin, i_ptBin, abs(value) )

        return h_2D_absValue


    def Make2DHist_QuadSum( self, list_2DHist ):
        h_2D_tot = list_2DHist[0].Clone()

        nPtBin  = h_2D_tot.GetNbinsY()
        nEtaBin = h_2D_tot.GetNbinsX()
        for i_pt in range(0, nPtBin):
            i_ptBin = i_pt+1

            for i_eta in range(0, nEtaBin):
                i_etaBin = i_eta+1

                quadSum = 0;
                for hist in list_2DHist:
                    value = hist.GetBinContent(i_etaBin, i_ptBin);
                    quadSum += value*value;

                quadSum = sqrt(quadSum)
                h_2D_tot.SetBinContent(i_etaBin, i_ptBin, quadSum)

        return h_2D_tot

    def Make1DHist_from2DHist( self, h_2D, f_output ):
        histName_2D = h_2D.GetName()

        nPtBin  = h_2D.GetNbinsY()
        nEtaBin = h_2D.GetNbinsX()
        for i_pt in range(0, nPtBin):
            histName_1D = histName_2D + "_pt%.0lfto%.0lf" % (self.list_ptBinEdge[i_pt], self.list_ptBinEdge[i_pt+1])
            h_1D = TH1D(histName_1D, "", nEtaBin, array("d", self.list_etaBinEdge))

            i_ptBin = i_pt+1
            for i_eta in range(0, nEtaBin):
                i_etaBin = i_eta+1

                value = h_2D.GetBinContent(i_etaBin, i_ptBin)
                error = h_2D.GetBinError(i_etaBin, i_ptBin)

                h_1D.SetBinContent(i_etaBin, value)
                h_1D.SetBinError(i_etaBin, error)

                # print "(ptBin, etaBin) = (%d, %d) -> (value, error) = (%lf, %lf)" % (i_ptBin, i_etaBin, value, error)

            f_output.cd()
            h_1D.Write()


    def Convert_AbsEtaHist(self, h2D):
        # -- self.list_etaBinEdge: already absolute eta bin edge
        histName = h2D.GetName()
        h2D_absEta = TH2D(histName+"_absEta", "", len(self.list_etaBinEdge)-1, array("d", self.list_etaBinEdge), len(self.list_ptBinEdge)-1, array("d", self.list_ptBinEdge) )

        nPtBin     = len(self.list_ptBinEdge) - 1
        nAbsEtaBin = len(self.list_etaBinEdge) - 1

        for i_pt in range(0, nPtBin):
            i_ptBin = i_pt+1

            for i_abseta in range(0, nAbsEtaBin):
                i_absEtaBin = i_abseta + 1
                i_etaBin = i_absEtaBin + 5

                value = h2D.GetBinContent(i_etaBin, i_ptBin)
                error = h2D.GetBinError(i_etaBin, i_ptBin)

                h2D_absEta.SetBinContent(i_absEtaBin, i_ptBin, value)
                h2D_absEta.SetBinError(i_absEtaBin, i_ptBin, error)

        return h2D_absEta

    def ConvertAbsEta_to_Eta(self, h2D_absEta):
        histName = h2D_absEta.GetName()
        h2D_absEta.SetName(histName+"_absEta")

        h2D_eta = TH2D(histName, "", len(self.list_unfoldedEtaBinEdge)-1, array("d", self.list_unfoldedEtaBinEdge), len(self.list_ptBinEdge)-1, array("d", self.list_ptBinEdge) )

        nPtBin     = len(self.list_ptBinEdge) - 1
        nAbsEtaBin = len(self.list_etaBinEdge) - 1

        for i_pt in range(0, nPtBin):
            i_ptBin = i_pt+1

            for i_absEta in range(0, nAbsEtaBin):
                i_absEtaBin = i_absEta+1

                value = h2D_absEta.GetBinContent(i_absEtaBin, i_ptBin)
                error = h2D_absEta.GetBinError(i_absEtaBin, i_ptBin)

                i_etaBin1 = i_absEta+(nAbsEtaBin+1);
                i_etaBin2 = nAbsEtaBin-i_absEta;

                h2D_eta.SetBinContent(i_etaBin1, i_ptBin, value)
                h2D_eta.SetBinContent(i_etaBin2, i_ptBin, value)
                h2D_eta.SetBinError(i_etaBin1, i_ptBin, error)
                h2D_eta.SetBinError(i_etaBin2, i_ptBin, error)

        return h2D_eta

    def Convert_Text_To_ListWithFloat(self, line):
        list_float = []

        list_split = line.split()
        for value in list_split:
            if self.IsNumber(value): list_float.append( float(value) )

        return list_float


    def IsNumber(self, s):
        try:
            float(s)
            return True
        except ValueError:
            return False


# -- main
baseDir = "/Users/KyeongPil_Lee/Physics/DYAnalysis_76X_LumiUpdate/Investigation/v05_ElecEffUnc"

# -- official (SMP-17-001), trigger
converter_SMP17001_trig = Converter()
converter_SMP17001_trig.inputFile = baseDir+"/Trigger.txt"
converter_SMP17001_trig.outputFileName = "ROOTFile_trigSF_SMP17001.root"
converter_SMP17001_trig.list_etaBinEdge = [-2.5, -2.0, -1.566, -1.4442, -0.800, 0, 0.800, 1.4442, 1.566, 2.0, 2.5]
converter_SMP17001_trig.list_ptBinEdge = [10, 20, 30, 40, 50, 2000]
converter_SMP17001_trig.list_columnTitle = ["etaMin", "etaMax", "ptMin", "ptMax", "eff_data", "statUnc_data", "eff_mc", "statUnc_mc", "eff_nlo_mc", "eff_tagChange_data"]

converter_SMP17001_trig.Convert()