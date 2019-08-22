# Procedure for HEP data preparation

## Create ROOT files for the HEPData input

### Purpose

* Collect all necessary histograms (differential cross sections, relative uncertainties from each source, covariance matrices)
* Keep up to 2 significant digits in all histograms (to be consistent with the paper)



### Procedure

```
cd $KP_ANALYZER_PATH/HEPData/MakeInputFile
root -b -q Convert_TwoSigDigts.cxx
```



### Output

```
ROOTFile_InputForHEPData_Upto2SigDigits_electron.root
ROOTFile_InputForHEPData_Upto2SigDigits_muon.root
ROOTFile_InputForHEPData_Upto2SigDigits_combined.root
```



## Create HEPData entry

```
cd $KP_ANALYZER_PATH/HEPData
python CreateHEPDataEntry.py
```



### Output

```
./HEPData_SMP17001
submission.tar.gz
```



## Upload in a sandbox

* Go to HEPData site & login with CERN account: <https://www.hepdata.net/login/>
* Click "sandbox" on top right
* Upload ```submission.tar.gz``` file -> process
* Copy the link (21 Aug.): <https://www.hepdata.net/record/sandbox/1566375034>



## Validation

### 1. Input file vs. ROOT file downloaded from HEPData

* Download the root file from the HEPData (on top right)
* Move it to ```$KP_ANALYZER_PATH/HEPData/Validation```
* Change the root file name to: ```HEPData_SMP17001.root```

```
cd $KP_ANALYZER_PATH/HEPData/Validation
root -b -q Validation_HEPDataEntry.cxx
```





### 2. Comparison: paper vs. HEPData

* Compare the numbers in the paper vs. HEPData by eye one by one
  * Use iPad



## Submit the HEPData entry

(Coming soon)

