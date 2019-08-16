from ROOT import TFile, TH1D, TH2D
from array import array
import sys

class Converter:
    def __init__(self):
        self.type = ""
        self.inputFile = ""
        self.outputFileName = ""

        # -- internal values
        self.list_ptBinEdge = [10, 20, 30, 40, 50, 2000.0]
        self.list_etaBinEdge = [-2.5, -2.0, -1.566, -1.4442, -0.800, 0, 0.800, 1.4442, 1.566, 2.0, 2.5]

    def Convert(self):
        self.CheckOption()



        f_input = open(self.inputFile, "r")
        lines = f_input.readlines()

        list_dic = [] # -- dictionary with all values
        for line in lines:
            list_float = self.Convert_Text_To_ListWithFloat(line)
            if len(list_float) == 0: continue

            dic_value = self.Convert_ListFloat_To_Dic(list_float)
            print dic_value
            list_dic.append( dic_value )


        self.MakeHistogramAndSave( list_dic )

        print "[Converter::Convert] Done"

    def CheckOption(self):
        print "type:           ", self.type
        print "inputFile:      ", self.inputFile
        print "outputFileName: ", self.outputFileName


    def MakeHistogramAndSave(self, list_dic):
        h_eff_data = TH2D("h_eff_data", "", len(self.list_etaBinEdge)-1, array("d", self.list_etaBinEdge), len(self.list_ptBinEdge)-1, array("d", self.list_ptBinEdge) )
        h_eff_mc   = TH2D("h_eff_mc",   "", len(self.list_etaBinEdge)-1, array("d", self.list_etaBinEdge), len(self.list_ptBinEdge)-1, array("d", self.list_ptBinEdge) )

        h_statUnc_data = TH2D("h_statUnc_data", "", len(self.list_etaBinEdge)-1, array("d", self.list_etaBinEdge), len(self.list_ptBinEdge)-1, array("d", self.list_ptBinEdge) )
        h_statUnc_mc   = TH2D("h_statUnc_mc",   "", len(self.list_etaBinEdge)-1, array("d", self.list_etaBinEdge), len(self.list_ptBinEdge)-1, array("d", self.list_ptBinEdge) )

        h_eff_bkgChange_data = TH2D("h_eff_bkgChange_data", "", len(self.list_etaBinEdge)-1, array("d", self.list_etaBinEdge), len(self.list_ptBinEdge)-1, array("d", self.list_ptBinEdge) )
        h_eff_sgnChange_data = TH2D("h_eff_sgnChange_data", "", len(self.list_etaBinEdge)-1, array("d", self.list_etaBinEdge), len(self.list_ptBinEdge)-1, array("d", self.list_ptBinEdge) )
        h_eff_tagChange_data = TH2D("h_eff_tagChange_data", "", len(self.list_etaBinEdge)-1, array("d", self.list_etaBinEdge), len(self.list_ptBinEdge)-1, array("d", self.list_ptBinEdge) )

        h_eff_nlo_mc   = TH2D("h_eff_nlo_mc",   "", len(self.list_etaBinEdge)-1, array("d", self.list_etaBinEdge), len(self.list_ptBinEdge)-1, array("d", self.list_ptBinEdge) )

        for dic_value in list_dic:
            i_etaBin = self.FindBin( dic_value["lowEtaBinEdge"], dic_value["highEtaBinEdge"], self.list_etaBinEdge )
            i_ptBin  = self.FindBin( dic_value["lowPtBinEdge"], dic_value["highPtBinEdge"], self.list_ptBinEdge )

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

            h_eff_tagChange_data.SetBinContent(i_etaBin, i_ptBin, dic_value["eff_tagChange_data"])
            h_eff_tagChange_data.SetBinError(i_etaBin, i_ptBin, 0)

            h_eff_nlo_mc.SetBinContent(i_etaBin, i_ptBin, dic_value["eff_nlo_mc"])
            h_eff_nlo_mc.SetBinError(i_etaBin, i_ptBin, 0)


        h_relDiff_bkgChange = self.Make2DHist_RelDiff(h_eff_data, h_eff_bkgChange_data)
        h_relDiff_sgnChange = self.Make2DHist_RelDiff(h_eff_data, h_eff_sgnChange_data)
        h_relDiff_tagChange = self.Make2DHist_RelDiff(h_eff_data, h_eff_tagChange_data)
        h_relDiff_nlo = self.Make2DHist_RelDiff(h_eff_mc, h_eff_nlo_mc)

        f_output = TFile(self.outputFileName, "RECREATE")
        h_eff_data.Write()
        h_eff_mc.Write()
        h_statUnc_data.Write()
        h_statUnc_mc.Write()
        h_eff_bkgChange_data.Write()
        h_eff_sgnChange_data.Write()
        h_eff_tagChange_data.Write()
        h_eff_nlo_mc.Write()

        h_relDiff_bkgChange.Write()
        h_relDiff_sgnChange.Write()
        h_relDiff_tagChange.Write()
        h_relDiff_nlo.Write()

        self.Make1DHist_from2DHist( h_eff_data, f_output )
        self.Make1DHist_from2DHist( h_eff_mc, f_output )
        self.Make1DHist_from2DHist( h_statUnc_data, f_output )
        self.Make1DHist_from2DHist( h_statUnc_mc, f_output )
        self.Make1DHist_from2DHist( h_eff_bkgChange_data, f_output )
        self.Make1DHist_from2DHist( h_eff_sgnChange_data, f_output )
        self.Make1DHist_from2DHist( h_eff_tagChange_data, f_output )
        self.Make1DHist_from2DHist( h_eff_nlo_mc, f_output )

        self.Make1DHist_from2DHist( h_relDiff_bkgChange, f_output )
        self.Make1DHist_from2DHist( h_relDiff_sgnChange, f_output )
        self.Make1DHist_from2DHist( h_relDiff_tagChange, f_output )
        self.Make1DHist_from2DHist( h_relDiff_nlo, f_output )

        f_output.Close()


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

                relDiff = (altValue - cenValue) / cenValue

                h_relDiff.SetBinContent(i_etaBin, i_ptBin, relDiff)
                h_relDiff.SetBinError(i_etaBin, i_ptBin, 0)

        return h_relDiff


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


    def Convert_Text_To_ListWithFloat(self, line):
        list_float = []

        list_split = line.split()
        for value in list_split:
            if self.IsNumber(value): list_float.append( float(value) )

        return list_float

    def Convert_ListFloat_To_Dic(self, list_float):
        dic_binValue = {}
        if self.type == "reco":
            dic_temp = {
            "lowEtaBinEdge":      list_float[0],
            "highEtaBinEdge":     list_float[1],
            "lowPtBinEdge":       list_float[2],
            "highPtBinEdge":      list_float[3],
            "eff_data":           list_float[4],
            "statUnc_data":       list_float[5],
            "eff_mc":             list_float[6],
            "statUnc_mc":         list_float[7],
            "eff_bkgChange_data": list_float[8],
            "eff_sgnChange_data": list_float[9],
            "eff_nlo_mc":         list_float[10],
            "eff_tagChange_data": list_float[11],
            }

            dic_binValue = dic_temp

        if self.type == "ID": # -- same with "reco" case
            dic_temp = {
            "lowEtaBinEdge":      list_float[0],
            "highEtaBinEdge":     list_float[1],
            "lowPtBinEdge":       list_float[2],
            "highPtBinEdge":      list_float[3],
            "eff_data":           list_float[4],
            "statUnc_data":       list_float[5],
            "eff_mc":             list_float[6],
            "statUnc_mc":         list_float[7],
            "eff_bkgChange_data": list_float[8],
            "eff_sgnChange_data": list_float[9],
            "eff_nlo_mc":         list_float[10],
            "eff_tagChange_data": list_float[11],
            }

            dic_binValue = dic_temp

        return dic_binValue



    def IsNumber(self, s):
        try:
            float(s)
            return True
        except ValueError:
            return False


# -- main -- #
converter_reco = Converter()
converter_reco.type = "reco"
converter_reco.inputFile = "RECO.txt"
converter_reco.outputFileName = "ROOTFile_RecoSF_Electron.root"
converter_reco.Convert()

converter_reco.type = "ID"
converter_reco.inputFile = "MediumID.txt"
converter_reco.outputFileName = "ROOTFile_IDSF_Electron.root"
converter_reco.Convert()

