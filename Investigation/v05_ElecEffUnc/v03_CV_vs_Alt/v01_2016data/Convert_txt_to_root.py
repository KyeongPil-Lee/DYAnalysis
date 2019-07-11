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

        self.addUnc_diffWithOld = False # -- true if the difference of the central value with the old reesult should be taken into account
        self.rootFileName_old = ""

        self.nColumn = 0

    def Convert(self):
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


    def MakeHistogramAndSave(self, list_dic):
        h_eff_data = TH2D("h_eff_data", "", len(self.list_etaBinEdge)-1, array("d", self.list_etaBinEdge), len(self.list_ptBinEdge)-1, array("d", self.list_ptBinEdge) )
        h_eff_mc   = TH2D("h_eff_mc",   "", len(self.list_etaBinEdge)-1, array("d", self.list_etaBinEdge), len(self.list_ptBinEdge)-1, array("d", self.list_ptBinEdge) )

        h_statUnc_data = TH2D("h_statUnc_data", "", len(self.list_etaBinEdge)-1, array("d", self.list_etaBinEdge), len(self.list_ptBinEdge)-1, array("d", self.list_ptBinEdge) )
        h_statUnc_mc   = TH2D("h_statUnc_mc",   "", len(self.list_etaBinEdge)-1, array("d", self.list_etaBinEdge), len(self.list_ptBinEdge)-1, array("d", self.list_ptBinEdge) )

        h_eff_bkgChange_data = TH2D("h_eff_bkgChange_data", "", len(self.list_etaBinEdge)-1, array("d", self.list_etaBinEdge), len(self.list_ptBinEdge)-1, array("d", self.list_ptBinEdge) )
        h_eff_sgnChange_data = TH2D("h_eff_sgnChange_data", "", len(self.list_etaBinEdge)-1, array("d", self.list_etaBinEdge), len(self.list_ptBinEdge)-1, array("d", self.list_ptBinEdge) )
        h_eff_tagChange_mc = TH2D("h_eff_tagChange_mc", "", len(self.list_etaBinEdge)-1, array("d", self.list_etaBinEdge), len(self.list_ptBinEdge)-1, array("d", self.list_ptBinEdge) )

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

            h_eff_bkgChange_data.SetBinContent(i_etaBin, i_ptBin, dic_value["eff_bkgChange_data"])
            h_eff_bkgChange_data.SetBinError(i_etaBin, i_ptBin, 0)

            h_eff_sgnChange_data.SetBinContent(i_etaBin, i_ptBin, dic_value["eff_sgnChange_data"])
            h_eff_sgnChange_data.SetBinError(i_etaBin, i_ptBin, 0)

            h_eff_tagChange_mc.SetBinContent(i_etaBin, i_ptBin, dic_value["eff_tagChange_mc"])
            h_eff_tagChange_mc.SetBinError(i_etaBin, i_ptBin, 0)

            h_eff_nlo_mc.SetBinContent(i_etaBin, i_ptBin, dic_value["eff_nlo_mc"])
            h_eff_nlo_mc.SetBinError(i_etaBin, i_ptBin, 0)

        # -- set the efficiencies & uncertainties as 0 for the ECALGAP bin
        if self.noECALGap:
            for i_pt in range(0, len(self.list_ptBinEdge)-1):
                i_ptBin = i_pt+1

                i_etaBin = self.FindBin( self.ECALGapRange[0], self.ECALGapRange[1], self.list_etaBinEdge )

                h_eff_data.SetBinContent(i_etaBin, i_ptBin, 0)
                h_eff_data.SetBinError(i_etaBin, i_ptBin, 0)

                h_eff_mc.SetBinContent(i_etaBin, i_ptBin, 0)
                h_eff_mc.SetBinError(i_etaBin, i_ptBin, 0)

                h_statUnc_data.SetBinContent(i_etaBin, i_ptBin, 0)
                h_statUnc_data.SetBinError(i_etaBin, i_ptBin, 0)

                h_statUnc_mc.SetBinContent(i_etaBin, i_ptBin, 0)
                h_statUnc_mc.SetBinError(i_etaBin, i_ptBin, 0)

                h_eff_bkgChange_data.SetBinContent(i_etaBin, i_ptBin, 0)
                h_eff_bkgChange_data.SetBinError(i_etaBin, i_ptBin, 0)

                h_eff_sgnChange_data.SetBinContent(i_etaBin, i_ptBin, 0)
                h_eff_sgnChange_data.SetBinError(i_etaBin, i_ptBin, 0)

                h_eff_tagChange_mc.SetBinContent(i_etaBin, i_ptBin, 0)
                h_eff_tagChange_mc.SetBinError(i_etaBin, i_ptBin, 0)

                h_eff_nlo_mc.SetBinContent(i_etaBin, i_ptBin, 0)
                h_eff_nlo_mc.SetBinError(i_etaBin, i_ptBin, 0)


        h_relDiff_bkgChange = self.Make2DHist_RelDiff(h_eff_data, h_eff_bkgChange_data)
        h_relDiff_sgnChange = self.Make2DHist_RelDiff(h_eff_data, h_eff_sgnChange_data)
        h_relDiff_tagChange = self.Make2DHist_RelDiff(h_eff_mc, h_eff_tagChange_mc)
        h_relDiff_nlo       = self.Make2DHist_RelDiff(h_eff_mc, h_eff_nlo_mc)

        h_absRelDiff_bkgChange = self.Make2DHist_AbsValue(h_relDiff_bkgChange)
        h_absRelDiff_sgnChange = self.Make2DHist_AbsValue(h_relDiff_sgnChange)
        h_absRelDiff_tagChange = self.Make2DHist_AbsValue(h_relDiff_tagChange)
        h_absRelDiff_nlo = self.Make2DHist_AbsValue(h_relDiff_nlo)

        # -- if the additional uncertainty should be added: bring the efficiency histogram from the old root file
        h_eff_oldEffData = ""
        h_eff_oldEffMC = ""
        h_relDiff_oldEffData = ""
        h_relDiff_oldEffMC = ""
        h_absRelDiff_oldEffData = ""
        h_absRelDiff_oldEffMC = ""
        if self.addUnc_diffWithOld:
            f_oldEff = TFile(self.rootFileName_old)
            h_eff_oldEffData = f_oldEff.Get("h_eff_data").Clone()
            h_eff_oldEffMC   = f_oldEff.Get("h_eff_mc").Clone()

            # -- old efficiency result for ID: its eta axis should be converted to absolute eta
            if "IDSF" in self.rootFileName_old:
                h_eff_oldEffData = self.Convert_AbsEtaHist(h_eff_oldEffData)
                h_eff_oldEffMC   = self.Convert_AbsEtaHist(h_eff_oldEffMC)

            h_eff_oldEffData.SetName("h_eff_oldEffData")
            h_eff_oldEffMC.SetName("h_eff_oldEffMC")

            h_relDiff_oldEffData = self.Make2DHist_RelDiff(h_eff_data, h_eff_oldEffData)
            h_relDiff_oldEffMC   = self.Make2DHist_RelDiff(h_eff_mc, h_eff_oldEffMC)
            h_absRelDiff_oldEffData = self.Make2DHist_AbsValue(h_relDiff_oldEffData)
            h_absRelDiff_oldEffMC   = self.Make2DHist_AbsValue(h_relDiff_oldEffMC)



        list_absRelDiffHist = [h_absRelDiff_bkgChange, h_absRelDiff_sgnChange, h_absRelDiff_tagChange, h_absRelDiff_nlo]
        if self.addUnc_diffWithOld:
            list_absRelDiffHist = [h_absRelDiff_bkgChange, h_absRelDiff_sgnChange, h_absRelDiff_tagChange, h_absRelDiff_nlo, h_absRelDiff_oldEffData, h_absRelDiff_oldEffMC]

        h_absRelDiff_tot = self.Make2DHist_QuadSum(list_absRelDiffHist)
        h_absRelDiff_tot.SetName("h_absRelDiff_tot")

        f_output = TFile(self.outputFileName, "RECREATE")
        h_eff_data.Write()
        h_eff_mc.Write()
        h_statUnc_data.Write()
        h_statUnc_mc.Write()
        h_eff_bkgChange_data.Write()
        h_eff_sgnChange_data.Write()
        h_eff_tagChange_mc.Write()
        h_eff_nlo_mc.Write()

        h_relDiff_bkgChange.Write()
        h_relDiff_sgnChange.Write()
        h_relDiff_tagChange.Write()
        h_relDiff_nlo.Write()

        h_absRelDiff_bkgChange.Write()
        h_absRelDiff_sgnChange.Write()
        h_absRelDiff_tagChange.Write()
        h_absRelDiff_nlo.Write()
        h_absRelDiff_tot.Write()

        self.Make1DHist_from2DHist( h_eff_data, f_output )
        self.Make1DHist_from2DHist( h_eff_mc, f_output )
        self.Make1DHist_from2DHist( h_statUnc_data, f_output )
        self.Make1DHist_from2DHist( h_statUnc_mc, f_output )
        self.Make1DHist_from2DHist( h_eff_bkgChange_data, f_output )
        self.Make1DHist_from2DHist( h_eff_sgnChange_data, f_output )
        self.Make1DHist_from2DHist( h_eff_tagChange_mc, f_output )
        self.Make1DHist_from2DHist( h_eff_nlo_mc, f_output )

        self.Make1DHist_from2DHist( h_relDiff_bkgChange, f_output )
        self.Make1DHist_from2DHist( h_relDiff_sgnChange, f_output )
        self.Make1DHist_from2DHist( h_relDiff_tagChange, f_output )
        self.Make1DHist_from2DHist( h_relDiff_nlo, f_output )

        self.Make1DHist_from2DHist( h_absRelDiff_bkgChange, f_output )
        self.Make1DHist_from2DHist( h_absRelDiff_sgnChange, f_output )
        self.Make1DHist_from2DHist( h_absRelDiff_tagChange, f_output )
        self.Make1DHist_from2DHist( h_absRelDiff_nlo, f_output )
        self.Make1DHist_from2DHist( h_absRelDiff_tot, f_output )

        if self.addUnc_diffWithOld:
            h_eff_oldEffData.Write()
            h_eff_oldEffMC.Write()
            h_relDiff_oldEffData.Write()
            h_relDiff_oldEffMC.Write()
            h_absRelDiff_oldEffData.Write()
            h_absRelDiff_oldEffMC.Write()

            self.Make1DHist_from2DHist( h_eff_oldEffData, f_output )
            self.Make1DHist_from2DHist( h_eff_oldEffMC, f_output )
            self.Make1DHist_from2DHist( h_relDiff_oldEffData, f_output )
            self.Make1DHist_from2DHist( h_relDiff_oldEffMC, f_output )
            self.Make1DHist_from2DHist( h_absRelDiff_oldEffData, f_output )
            self.Make1DHist_from2DHist( h_absRelDiff_oldEffMC, f_output )

            f_oldEff.Close()

            print "Additional uncertainty from the difference of the central value with the old result is also taken into account in the total uncertainty"


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

        nPtBin  = len(self.list_ptBinEdge) - 1
        nEtaBin = len(self.list_etaBinEdge) - 1
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


    def Make2DHist_AbsValue( self, h_2D ):
        h_2D_absValue = h_2D.Clone()
        histName = h_2D.GetName()
        histName = histName.replace("relDiff", "absRelDiff")
        h_2D_absValue.SetName(histName)

        nPtBin  = len(self.list_ptBinEdge) - 1
        nEtaBin = len(self.list_etaBinEdge) - 1
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

        nPtBin  = len(self.list_ptBinEdge) - 1
        nEtaBin = len(self.list_etaBinEdge) - 1
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

        nPtBin  = len(self.list_ptBinEdge) - 1
        nEtaBin = len(self.list_etaBinEdge) - 1
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
# -- official (SMP-17-001), Reco
converter_2016_reco = Converter()
converter_2016_reco.inputFile = "elecEff_2016_Moriond17_reco.txt"
converter_2016_reco.outputFileName = "ROOTFile_RecoSF_2016_Moriond17.root"
converter_2016_reco.list_etaBinEdge = [-2.5,-2.45,-2.4,-2.3,-2.2,-2.0,-1.8,-1.63,-1.566,-1.444, -1.2,-1.0,-0.6,-0.4,-0.2,0.0, 0.2,0.4,0.6, 1.0,1.2,1.444,1.566, 1.63,1.8,2.0, 2.2,2.3,2.4,2.45,2.5]
converter_2016_reco.list_ptBinEdge = [25, 2000]
converter_2016_reco.list_columnTitle = ["etaMin", "etaMax", "ptMin", "ptMax", "eff_data", "statUnc_data", "eff_mc", "statUnc_mc", "eff_bkgChange_data", "eff_sgnChange_data", "eff_nlo_mc", "eff_tagChange_mc"]
converter_2016_reco.noECALGap = False
converter_2016_reco.Convert()

# -- official (SMP-17-001), ID
converter_2016_ID = Converter()
converter_2016_ID.inputFile = "elecEff_2016_Moriond17_mediumID.txt"
converter_2016_ID.outputFileName = "ROOTFile_IDSF_2016_Moriond17.root"
converter_2016_ID.list_etaBinEdge = [-2.5, -2.0, -1.566, -1.444, -0.800, 0, 0.800, 1.444, 1.566, 2.0, 2.5]
converter_2016_ID.list_ptBinEdge = [10, 20, 35, 50, 90, 150, 500]
converter_2016_ID.list_columnTitle = ["etaMin", "etaMax", "ptMin", "ptMax", "eff_data", "statUnc_data", "eff_mc", "statUnc_mc", "eff_bkgChange_data", "eff_sgnChange_data", "eff_nlo_mc", "eff_tagChange_mc"]
converter_2016_ID.noECALGap = False

converter_2016_ID.Convert()

