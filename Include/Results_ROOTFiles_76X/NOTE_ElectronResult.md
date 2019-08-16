## 06 August 2019

### DiffXsec_Electron_v12.root





### ROOTFile_RelUnc_All_Electron_v3.root

* from v2, Update the uncertainty from the efficiency SF and total systematic uncertainty with the updated TnP efficiencies & uncertainties
  * Code: ```ElecChannel/FinalFormat/v01_HybridMethod/Update_RelUncROOTFile.cxx```



## XX April 2019

### DiffXsec_Electron_v11.root

* Update the uncertainty from the efficiency SF using new electron efficiency
  * CAVEAT: The central value of the differential cross sections are not changed!



## Previous updates

### Electron channel

- Include/DiffXsec_Electron_v8.root
  - h_RelUnc_Syst: all systematic uncertianties including acc. + PDF uncertainty. Luminosity uncertainty is not included
- Include/DiffXsec_Electron_v10.root
  - Data & MC fiducial cross section is updated
    - Data: apply efficiency correction
    - MC: use generator information only (previously, it was unfolded reco-level result)
- Include/ROOTFile_RelUnc_All_Electron_v2.root
  - h_RelSysUnc_Tot_Percent: all systematic uncertainties **except** for acc.+PDF uncertainty. Luminosity uncertainty is not included
    - Quad. sum(h_RelSysUnc_Tot_Percent, h_RelSysUnc_Acc._Percent) 
      = h_RelUnc_Syst in Include/DiffXsec_Electron_v8/10.root