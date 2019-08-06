## Definition of uncertainty histograms

### Electron channel

* Include/DiffXsec_Electron_v8.root
  * h_RelUnc_Syst: all systematic uncertianties including acc. + PDF uncertainty. Luminosity uncertainty is not included
* Include/DiffXsec_Electron_v10.root
  * Data & MC fiducial cross section is updated
    * Data: apply efficiency correction
    * MC: use generator information only (previously, it was unfolded reco-level result)
* Include/ROOTFile_RelUnc_All_Electron_v2.root
  * h_RelSysUnc_Tot_Percent: all systematic uncertainties **except** for acc.+PDF uncertainty. Luminosity uncertainty is not included
    * Quad. sum(h_RelSysUnc_Tot_Percent, h_RelSysUnc_Acc._Percent) 
      = h_RelUnc_Syst in Include/DiffXsec_Electron_v8/10.root