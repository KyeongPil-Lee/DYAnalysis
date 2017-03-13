#!/bin/bash

#########################################################
# -- qsub commands: #$ + command (details: man qsub) -- #
#########################################################
# -- shell used for executing the script -- #
#$ -S /bin/sh

# -- combine standard output & error file -- #
#$ -j y

# -- send the mail when the job is aborted or ended -- #
#$ -m ae -M kplee@cern.ch

# -- stay in the directory where qsub command is executed -- #
#$ -cwd

cwd=$(pwd)

export SCRAM_ARCH=slc6_amd64_gcc530
export VO_CMS_SW_DIR=/cvmfs/cms.cern.ch
source $VO_CMS_SW_DIR/cmsset_default.sh

# -- CMSSW enviornment -- #
cd /cvmfs/cms.cern.ch/slc6_amd64_gcc530/cms/cmssw/CMSSW_8_0_13
cmsenv

cd ${cwd}

./local_run.sh z v20161222_M600to700_DY_NNLO_MMHT2014nnlo68cl input_v20161222_M600to700_DY_NNLO_MMHT2014nnlo68cl.txt v20161222_histograms_M600to700.txt v20161222_M600to700_DY_NNLO_MMHT2014nnlo68cl.dat ../ 24 >&log_v20161222_M600to700_DY_NNLO_MMHT2014nnlo68cl
./finish.sh v20161222_M600to700_DY_NNLO_MMHT2014nnlo68cl NNLO.v20161222_M600to700_DY_NNLO_MMHT2014nnlo68cl.dat

echo "job is completed"

# -- &>log: "Invalid null command" Error occurs. please use >&log. -- #

# -- PLEASE ENTER AFTER THE LAST LINE! ... IF YOU DON'T, LAST LINE WILL NOT BE EXECUTED BY BATCH JOB -- # 