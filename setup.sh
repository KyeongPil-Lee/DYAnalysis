#!/bin/bash

if [ $KP_ANALYZER_PATH ]; then
    echo "KP_ANALYZER_PATH is already defined: use a clean shell!"
    return 1
fi

# -- analyzer path (do not insert "/" in the end of the path)-- #
export KP_ANALYZER_PATH=$(pwd)
export KP_INCLUDE_PATH=$KP_ANALYZER_PATH/Include
export KP_ROOTFILE_PATH=$KP_INCLUDE_PATH/Results_ROOTFiles_76X

# -- root setup -- #
export ROOT_INCLUDE_PATH=${KP_ANALYZER_PATH}:${ROOT_INCLUDE_PATH}

# -- ntuple path -- #
export KP_DATA_PATH=""
if [ $HOSTNAME == "tamsa2" ]; # -- 147.47.242.67 -- # 
then 
	KP_DATA_PATH="/data4/Users/kplee/DYntuple"

	# # -- cmssw setup (for ROOT6 & compatible with RooUnfold in tamsa2) -- #
	# export SCRAM_ARCH=slc6_amd64_gcc493
	# export VO_CMS_SW_DIR=/cvmfs/cms.cern.ch
	# source $VO_CMS_SW_DIR/cmsset_default.sh

	# # cd /share_home/kplee/ntupleMaker/DYntupleMaker/v20160421_1st_76X/CMSSW_7_6_3_patch2
	# cd /share_home/kplee/CMSSW/CMSSW_7_6_3_patch2
	# eval `scramv1 runtime -sh` # -- equivalent to cmsenv (cmsenv doesn't work. why?) -- #

	export SCRAM_ARCH=slc6_amd64_gcc491
	export VO_CMS_SW_DIR=/cvmfs/cms.cern.ch
	source $VO_CMS_SW_DIR/cmsset_default.sh
	cd /cvmfs/cms.cern.ch/slc6_amd64_gcc491/cms/cmssw/CMSSW_7_4_1 # -- has ROOT 6.02/05, and unfolding package was compiled under this version! -- #
	eval `scramv1 runtime -sh`

	cd $KP_ANALYZER_PATH
		
elif [ $HOSTNAME == "cms.snu.ac.kr" ]; then
	KP_DATA_PATH=/home/kplee/data1/DYntuple/76X

elif [ $HOSTNAME == "muon" ]; then
	KP_DATA_PATH="/scratch/kplee/DY2015Ntuple_ee"

	export SCRAM_ARCH=slc6_amd64_gcc491
	export VO_CMS_SW_DIR=/cvmfs/cms.cern.ch
	source $VO_CMS_SW_DIR/cmsset_default.sh
	cd /cvmfs/cms.cern.ch/slc6_amd64_gcc491/cms/cmssw/CMSSW_7_4_1 # -- has ROOT 6.02/05, and unfolding package was compiled under this version! -- #
	eval `scramv1 runtime -sh`

	cd $KP_ANALYZER_PATH

elif [ $HOME == "/Users/KyeongPil_Lee" ]; then # -- macbook -- #
	KP_DATA_PATH="/Users/KyeongPil_Lee/Physics/Data/DY2015"
fi

# -- RooUnfold package path -- #
export KP_ROOUNFOLD_PATH=""
if [ $HOME == "/Users/KyeongPil_Lee" ]; then # -- macbook -- #
	KP_ROOUNFOLD_PATH=/Users/KyeongPil_Lee/ROOT5/Unfolding/RooUnfold
elif [ $HOSTNAME == "tamsa2.snu.ac.kr" ]; then
	KP_ROOUNFOLD_PATH=/share_home/kplee/Unfolding
elif [ $HOSTNAME == "muon" ]; then
	KP_ROOUNFOLD_PATH=/home/kplee/Physics/RooUnfold
fi
export ROOT_INCLUDE_PATH=${KP_ROOUNFOLD_PATH}:${ROOT_INCLUDE_PATH}

# -- ROOT thisroot.sh PATH -- #
export KP_ROOT5_PATH=""
export KP_ROOT6_PATH=""
if [ $HOME == "/Users/KyeongPil_Lee" ]; then
	KP_ROOT5_PATH="/Users/KyeongPil_Lee/ROOT5/root/bin/thisroot.sh"
	KP_ROOT6_PATH="/Users/KyeongPil_Lee/ROOT6/root/bin/thisroot.sh"
# elif [ "$HOSTNAME" == "tamsa2.snu.ac.kr" -o "$HOSTNAME" == "cms.snu.ac.kr" ]; then
elif [ "$HOSTNAME" == "cms.snu.ac.kr" ]; then
	KP_ROOT5_PATH="/share/apps/root_v5-34-32/root/bin/thisroot.sh"
	KP_ROOT6_PATH="/cvmfs/cms.cern.ch/slc6_amd64_gcc530/lcg/root/6.06.00-ikhhed6/bin/thisroot.sh" # -- CMSSW_8_0_25 -- #
fi

# -- setup for lxplus
if [[ $HOSTNAME =~ "cern.ch" ]]; then
	export SCRAM_ARCH=slc6_amd64_gcc491
	export VO_CMS_SW_DIR=/cvmfs/cms.cern.ch
	source $VO_CMS_SW_DIR/cmsset_default.sh
	cd /cvmfs/cms.cern.ch/slc6_amd64_gcc491/cms/cmssw/CMSSW_7_4_1 # -- has ROOT 6.02/05, and unfolding package was compiled under this version! -- #
	eval `scramv1 runtime -sh`

	cd $KP_ANALYZER_PATH

	KP_ROOUNFOLD_PATH="/afs/cern.ch/user/k/kplee/work/private/Physics/RooUnfold"
fi

# -- Directory for official style plots -- #
export KP_PLOT_PATH=$KP_ANALYZER_PATH/Local/Plots

echo "================ environment ================"
echo "KP_ANALYZER_PATH: " ${KP_ANALYZER_PATH}
echo "KP_INCLUDE_PATH:  " ${KP_INCLUDE_PATH}
echo "KP_ROOTFILE_PATH: " ${KP_ROOTFILE_PATH}
echo "KP_ROOT5_PATH:    " ${KP_ROOT5_PATH}
echo "KP_ROOT6_PATH:    " ${KP_ROOT6_PATH}
echo "KP_PLOT_PATH:     " ${KP_PLOT_PATH}

echo "KP_DATA_PATH:     " ${KP_DATA_PATH}
if [ -z $KP_DATA_PATH ]; then
    echo "     [WARNING]: ntuples are not available in this machine"
fi

echo "KP_ROOUNFOLD_PATH:" ${KP_ROOUNFOLD_PATH}
if [ -z $KP_ROOUNFOLD_PATH ]; then
	echo "     [WARNING]: RooUnfold package is not available in this machine"
fi

echo "============================================="
echo "setup is finished. Welcome :)"
