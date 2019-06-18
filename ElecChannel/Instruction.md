# Instruction for e channel machinery

## Central value for the cross section

```
# -- dressed level result
cd ElecChannel/CentralValue/Dressed
source script_FinalCorrections_v1.sh
# -- output: FinalCorr_v11.root

# -- fiducial cross section result
cd ../Fiducial
root -b -q FinalCorrections_v1.C
# -- output: FinalCorr_postFSR_v11.root
```



## Diff. x-section with full uncertainty

```
# -- dressed level result
cd ElecChannel/FinalResult/Dressed
root -b -q TotalError_XSec.c
# -- output: FinalCorr_v12_May2019.root

# -- fiducial cross section result
cd ../Fiducial
root -b -q TotalError_XSec.c
# -- output: FinalCorr_postFSR_v12_May2019.root
```



## Create a ROOT file with full results

```
cd ElecChannel/OutputFormat
root -b -q input.c
# -- output: DiffXsec_Electron_v11.root
```







### Email from Ridhi

>Hi Kyeongpil,
>
>
>
>Please find the required codes in the attached folder which contains three sub-folders:
>
>\1. Xsec - Calculates the dressed and fiducial differential cross sections.
>
>\2. Systematics - Uses input from Xsec and Systematics folders to calculate the systematic uncertainty for dressed and fiducial differential cross sections.
>
>\3. Input - Uses input from Xsec and Systematics folders to obtain root file used to plot the cross section results. 
>
>
>
>The instructions to run the codes are below:
>
>
>
>Step 1. Run FinalCorrections_v1.C in the directories Xsec/Dressed and Xsec/Fiducial
>
>This will produce the outputs FinalCorr_v11.root and FinalCorr_postFSR_v11.root respectively in each folder.
>
>
>
>Step 2. Run TotalError_XSec.c in the directories Systematics/Dressed and Systematics/Fiducial
>
>This will produce the outputs FinalCorr_v12_May2019.root and FinalCorr_postFSR_v12_May2019.root respectively in each folder.
>
>
>
>Step 3. Run Input.c in the directory Input
>
>This will produce the output DiffXsec_Electron_v11.root
>
>
>
>I have included all the input files which are required to run these codes in the respective folders. The codes should run fine without any error.
>
>
>
>Please let me know if you face any problem and if there is anything else which you need.
>
>
>
>Best Regards,
>
>Ridhi