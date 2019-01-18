import os

from hepdata_lib import Submission, Table, RootFileReader, Variable, Uncertainty

class HEPDataTableProducer:
    def __init__(self):
        self.channel = ""

        self.tableTitle = ""
        self.tableDesc = ""
        self.tableLocation = ""

        self.rootFilePath = ""
        self.dXSecHistName = ""
        self.covMHistName = ""
        self.list_uncInfo = []

    # -- for diff. x-section results
    def CreateTable(self):
        table = Table(self.tableTitle)
        self.SetTableProperties(table)

        (var_M, var_dXSec) = self.CreateVar_DXSecWithUnc()

        table.add_variable(var_M)
        table.add_variable(var_dXSec)

        return table


    def CreateTableForCovM(self):
        table = Table(self.tableTitle)
        self.SetTableProperties(table)

        # -- matrix: two independent variables (x, y bins) + one dependent variable (covariance values)
        (var_Mx, var_My, var_cov) = self.CreateVar_CovM()

        table.add_variable(var_Mx)
        table.add_variable(var_My)
        table.add_variable(var_cov)

        return table


    def SetTableProperties(self, table):
        table.description = self.tableDesc
        table.location = self.tableLocation
        # table.keywords["observables"] = ["ACC", "EFF"]
        table.keywords["reactions"] = ["P P --> GAMMA* X", "P P --> Z0 X", "P P --> LEPTON+ LEPTON- X"]
        table.keywords["observables"] = ["DSIG/DM"]
        table.keywords["phrases"] = ["Inclusive", "Single Differential Cross Section", "Proton-Proton Scattering", "Drell Yan", "Z Production"]
        if self.channel == "muon":
            table.keywords["reactions"].append("P P --> MU+ MU- X")
            table.keywords["phrases"].append("Muon production")

        elif self.channel == "electron":
            table.keywords["reactions"].append("P P --> E+ E- X")
            table.keywords["phrases"].append("Electron production")

        elif self.channel == "combined":
            table.keywords["reactions"].append("P P --> MU+ MU- X")
            table.keywords["reactions"].append("P P --> E+ E- X")
            table.keywords["phrases"].append("Muon production")
            table.keywords["phrases"].append("Electron production")


    def CreateVar_CovM(self):
        rootFileReader = RootFileReader(self.rootFilePath)
        h_covM = rootFileReader.read_hist_2d(self.covMHistName)

        # -- mass bin edges
        varName_M = ""
        if self.channel == "muon":     varName_M = "$M_{\mu\mu}$"
        if self.channel == "electron": varName_M = "$M_{ee}$"
        if self.channel == "combined": varName_M = "$M_{\ell\ell}$"

        var_Mx = Variable(varName_M+"(x)", is_independent=True, is_binned=True, units="GEV")
        var_Mx.values = h_covM["x_edges"]
        print var_Mx.values

        var_My = Variable(varName_M+"(y)", is_independent=True, is_binned=True, units="GEV")
        var_My.values = h_covM["y_edges"]
        print var_My.values

        var_cov = Variable("Covariance", is_independent=False, is_binned=False, units="")
        if self.channel == "muon":      var_cov.add_qualifier("Reaction", "P P --> MU+ MU- X")
        if self.channel == "electron":  var_cov.add_qualifier("Reaction", "P P --> E+ E- X")
        if self.channel == "combined":  var_cov.add_qualifier("Reaction", "P P --> LEPTON+ LEPTON- X")
        var_cov.add_qualifier("SQRT(S)", 13, "TeV")
        var_cov.values = h_covM["z"]
        print var_cov.values

        return (var_Mx, var_My, var_cov)


    def CreateVar_DXSecWithUnc(self):
        rootFileReader = RootFileReader(self.rootFilePath)
        h_dXSec = rootFileReader.read_hist_1d(self.dXSecHistName)

        # -- mass bin edges
        varName_M = ""
        if self.channel == "muon":     varName_M = "$M_{\mu\mu}$"
        if self.channel == "electron": varName_M = "$M_{ee}$"
        if self.channel == "combined": varName_M = "$M_{\ell\ell}$"
        var_M = Variable(varName_M, is_independent=True, is_binned=True, units="GEV")
        var_M.values = h_dXSec["x_edges"]
        print var_M.values

        # -- central value of differential cross section
        var_dXSec = Variable("D(SIG)/DM", is_independent=False, is_binned=False, units="PB/GEV")
        if self.channel == "muon":      var_dXSec.add_qualifier("Reaction", "P P --> MU+ MU- X")
        if self.channel == "electron":  var_dXSec.add_qualifier("Reaction", "P P --> E+ E- X")
        if self.channel == "combined":  var_dXSec.add_qualifier("Reaction", "P P --> LEPTON+ LEPTON- X")
        var_dXSec.add_qualifier("SQRT(S)", 13, "TeV")
        var_dXSec.values = h_dXSec["y"]
        print var_dXSec.values

        self.AddUncertainties(var_dXSec)

        return (var_M, var_dXSec)


    def AddUncertainties(self, var_dXSec):
        for uncInfo in self.list_uncInfo:
            uncType     = uncInfo[0]
            uncHistName = uncInfo[1]

            # -- "Uncertainty" object initialization
            uncObj = Uncertainty(uncType, is_symmetric=True)

            # -- Get the uncertainty values from the histogram & put them to "Uncertaitny" object
            rootFileReader = RootFileReader(self.rootFilePath)
            h_absUnc = rootFileReader.read_hist_1d(uncHistName)
            uncObj.values = h_absUnc["y"]

            print "+"*100
            print "uncertainty type = %s -> values:" % uncType
            print uncObj.values
            print "+"*100
            print "\n"

            var_dXSec.add_uncertainty(uncObj)


##############################################################
# -- muon channel, full phase space, FSR corrected results
producer_muon = HEPDataTableProducer()
producer_muon.channel = "muon"

producer_muon.tableTitle = "Table 1"
producer_muon.tableDesc = "Drell-Yan differential cross section as a function of the dimuon invariant mass, measured in the full phase space, with FSR correction is applied."
producer_muon.tableLocation = "Data from figure 4 (top)"

producer_muon.rootFilePath = "%s/OfficialStyle/ROOTFile_PrintNumbersInTable_Muon.root" % os.getenv("KP_ANALYZER_PATH")
producer_muon.dXSecHistName = "h_dXSec"
producer_muon.list_uncInfo = [ 
["Statistical uncertainty",         "h_absUnc_stat"],
["Luminosity uncertainty",          "h_absUnc_lumi"],
["Acceptance+PDF uncertainty",      "h_absUnc_syst_accPDF"],
["Efficiency SF uncertainty",       "h_absUnc_syst_eff"],
["Detector resolution uncertainty", "h_absUnc_syst_detRes"],
["Background uncertainty",          "h_absUnc_syst_bkg"],
["FSR uncertainty",                 "h_absUnc_syst_FSR"],

["Total uncertainty",               "h_absUnc_tot"],
]

table_muon = producer_muon.CreateTable()
##############################################################


##############################################################
# -- electron channel, full phase space, FSR corrected results
producer_electron = HEPDataTableProducer()
producer_electron.channel = "electron"

producer_electron.tableTitle = "Table 2"
producer_electron.tableDesc = "Drell-Yan differential cross section as a function of the dielectron invariant mass, measured in the full phase space, with FSR correction applied."
producer_electron.tableLocation = "Data from figure 4 (bottom)"

producer_electron.rootFilePath = "%s/OfficialStyle/ROOTFile_PrintNumbersInTable_Electron.root" % os.getenv("KP_ANALYZER_PATH")
producer_electron.dXSecHistName = "h_dXSec"
producer_electron.list_uncInfo = [ 
["Statistical uncertainty",         "h_absUnc_stat"],
["Luminosity uncertainty",          "h_absUnc_lumi"],
["Acceptance+PDF uncertainty",      "h_absUnc_syst_accPDF"],
["Efficiency SF uncertainty",       "h_absUnc_syst_eff"],
["Detector resolution uncertainty", "h_absUnc_syst_detRes"],
["Background uncertainty",          "h_absUnc_syst_bkg"],
["FSR uncertainty",                 "h_absUnc_syst_FSR"],

["Total uncertainty",               "h_absUnc_tot"],
]

table_electron = producer_electron.CreateTable()
##############################################################


##############################################################
# -- muon channel, fiducial phase space, post-FSR results
producer_muonFid = HEPDataTableProducer()
producer_muonFid.channel = "muon"

producer_muonFid.tableTitle = "Table 3"
producer_muonFid.tableDesc = "Drell-Yan differential cross section as a function of the dimuon invariant mass, measured in the fiducial phase space, with no FSR correction applied."
producer_muonFid.tableLocation = "Data from figure 5 (top)"

producer_muonFid.rootFilePath = "%s/OfficialStyle/ROOTFile_PrintNumbersInTable_Muon.root" % os.getenv("KP_ANALYZER_PATH")
producer_muonFid.dXSecHistName = "h_FpoF_dXSec"
producer_muonFid.list_uncInfo = [ 
["Statistical uncertainty",         "h_FpoF_absUnc_stat"],
["Luminosity uncertainty",          "h_FpoF_absUnc_lumi"],
["Efficiency SF uncertainty",       "h_FpoF_absUnc_syst_eff"],
["Detector resolution uncertainty", "h_FpoF_absUnc_syst_detRes"],
["Background uncertainty",          "h_FpoF_absUnc_syst_bkg"],

["Total uncertainty",               "h_FpoF_absUnc_tot"],
]

table_muonFid = producer_muonFid.CreateTable()
##############################################################


##############################################################
# -- electron channel, fiducial phase space, post-FSR results
producer_electronFid = HEPDataTableProducer()
producer_electronFid.channel = "electron"

producer_electronFid.tableTitle = "Table 4"
producer_electronFid.tableDesc = "Drell-Yan differential cross section as a function of the dielectron invariant mass, measured in the fiducial phase space, with no FSR correction applied."
producer_electronFid.tableLocation = "Data from figure 5 (bottom)"

producer_electronFid.rootFilePath = "%s/OfficialStyle/ROOTFile_PrintNumbersInTable_Electron.root" % os.getenv("KP_ANALYZER_PATH")
producer_electronFid.dXSecHistName = "h_FpoF_dXSec"
producer_electronFid.list_uncInfo = [ 
["Statistical uncertainty",         "h_FpoF_absUnc_stat"],
["Luminosity uncertainty",          "h_FpoF_absUnc_lumi"],
["Efficiency SF uncertainty",       "h_FpoF_absUnc_syst_eff"],
["Detector resolution uncertainty", "h_FpoF_absUnc_syst_detRes"],
["Background uncertainty",          "h_FpoF_absUnc_syst_bkg"],

["Total uncertainty",               "h_FpoF_absUnc_tot"],
]

table_electronFid = producer_electronFid.CreateTable()
##############################################################


##############################################################
# -- combined result, full phase space, FSR-corrected results
producer_combined = HEPDataTableProducer()
producer_combined.channel = "combined"

producer_combined.tableTitle = "Table 5"
producer_combined.tableDesc = "The differential DY cross section measured for the combination of the two channels in the full phase space."
producer_combined.tableLocation = "Data from figure 6"

producer_combined.rootFilePath = "%s/OfficialStyle/ROOTFile_PrintNumbersInTable_Combined.root" % os.getenv("KP_ANALYZER_PATH")
producer_combined.dXSecHistName = "h_dXSec"
producer_combined.list_uncInfo = [ 
["Total uncertainty",               "h_absUnc_tot"],
]

table_combined = producer_combined.CreateTable()
##############################################################


##############################################################
# -- muon channel, full covariance matrix
producerCovM_muon = HEPDataTableProducer()
producerCovM_muon.channel = "muon"

producerCovM_muon.tableTitle = "Table 6"
producerCovM_muon.tableDesc = "Covariance matrix of the Drell-Yan differential cross section as a function of the dimuon invariant mass, measured in the full phase space, with FSR correction is applied. Luminosity uncertainty is not included in the covariance."
producerCovM_muon.tableLocation = "Covariance matrix of the data from figure 4 (top)"

producerCovM_muon.rootFilePath = "%s/OfficialStyle/ROOTFile_covM_muon.root" % os.getenv("KP_ANALYZER_PATH")
producerCovM_muon.covMHistName = "h_cov_noLumi"

tableCovM_muon = producerCovM_muon.CreateTableForCovM()
##############################################################

##############################################################
# -- electron channel, full covariance matrix
producerCovM_electron = HEPDataTableProducer()
producerCovM_electron.channel = "electron"

producerCovM_electron.tableTitle = "Table 7"
producerCovM_electron.tableDesc = "Covariance matrix of the Drell-Yan differential cross section as a function of the dielectron invariant mass, measured in the full phase space, with FSR correction is applied. Luminosity uncertainty is not included in the covariance."
producerCovM_electron.tableLocation = "Covariance matrix of the data from figure 4 (bottom)"

producerCovM_electron.rootFilePath = "%s/OfficialStyle/ROOTFile_covM_electron.root" % os.getenv("KP_ANALYZER_PATH")
producerCovM_electron.covMHistName = "h_cov_noLumi"

tableCovM_electron = producerCovM_electron.CreateTableForCovM()
##############################################################

##############################################################
# -- combined channel, full covariance matrix
producerCovM_combined = HEPDataTableProducer()
producerCovM_combined.channel = "combined"

producerCovM_combined.tableTitle = "Table 8"
producerCovM_combined.tableDesc = "Covariance matrix of the differential DY cross section measured for the combination of the two channels in the full phase space."
producerCovM_combined.tableLocation = "Covariance matrix of the data from figure 6"

producerCovM_combined.rootFilePath = "%s/OfficialStyle/ROOTFile_covM_combined.root" % os.getenv("KP_ANALYZER_PATH")
producerCovM_combined.covMHistName = "h_cov"

tableCovM_combined = producerCovM_combined.CreateTableForCovM()
##############################################################


submission = Submission()
submission.add_table(table_muon)
submission.add_table(table_electron)
submission.add_table(table_muonFid)
submission.add_table(table_electronFid)
submission.add_table(table_combined)
submission.add_table(tableCovM_muon)
submission.add_table(tableCovM_electron)
submission.add_table(tableCovM_combined)

for table in submission.tables:
    table.keywords["cmenergies"] = [13000]

submission.create_files("./HEPData_SMP17001")



# submission = Submission()

# table = Table("Data from figure 4 (top)")
# table.description = "Drell-Yan differential cross section as a function of the dimuon invariant mass, measured in the full phase space, with FSR correction is applied"
# table.location = "Data from Figure 4 (top)"
# # table.keywords["observables"] = ["ACC", "EFF"]
# table.keywords["reactions"] = ["P P --> LEPTON+ LEPTON- X"]
# # table.add_image("example_inputs/CMS-B2G-17-009_Figure_004-a.pdf")


# # rootFilePath = os.getenv("KP_ROOTFILE_PATH")
# rootFilePath = "%s/OfficialStyle/ROOTFile_PrintNumbersInTable_Muon.root" % os.getenv("KP_ANALYZER_PATH")
# reader_dimuon = RootFileReader(rootFilePath)
# h_dimuon_fullwFSR = reader_dimuon.read_hist_1d("h_dXSec")
# # print h_dimuon_fullwFSR.keys()
# # print h_dimuon_fullwFSR["x_edges"]

# var_dimuon_M = Variable("$M_{#mu#mu}$", is_independent=True, is_binned=True, units="GEV")
# var_dimuon_M.values = h_dimuon_fullwFSR["x_edges"]
# print var_dimuon_M.values

# var_dimuon_dXSec_fullwFSR = Variable("D(SIG)/DM", is_independent=True, is_binned=False, units="PB/GEV")
# var_dimuon_dXSec_fullwFSR.values = h_dimuon_fullwFSR["y"]
# print var_dimuon_dXSec_fullwFSR.values


# unc_dimuon_fullwFSR_total = Uncertainty("Total uncertainty", is_symmetric=True)
# h_dimuon_absUnc_tot = reader_dimuon.read_hist_1d("h_absUnc_tot")
# unc_dimuon_fullwFSR_total.values = h_dimuon_absUnc_tot["y"]
# print unc_dimuon_fullwFSR_total.values

# var_dimuon_dXSec_fullwFSR.add_uncertainty(unc_dimuon_fullwFSR_total)


# for table in submission.tables:
#     table.keywords["cmenergies"] = [13000]