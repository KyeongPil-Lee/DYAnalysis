## Definition of uncertainty histograms

### Electron channel

* Include/DiffXsec_Electron_v8.root
  * h_RelUnc_Syst: all systematic uncertianties including acc. + PDF uncertainty. Luminosity uncertainty is not included
* Include/ROOTFile_RelUnc_All_Electron_v2.root
  * h_RelSysUnc_Tot_Percent: all systematic uncertainties **except** for acc.+PDF uncertainty. Luminosity uncertainty is not included
    * Quad. sum(h_RelSysUnc_Tot_Percent, h_RelSysUnc_Acc._Percent) 
      = h_RelUnc_Syst in Include/DiffXsec_Electron_v8.root