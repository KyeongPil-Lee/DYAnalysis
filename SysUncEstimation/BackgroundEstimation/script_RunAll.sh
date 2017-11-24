#!bin/bash

source LoadLib_SysUnc_BkgEst.sh ttbar >&log_LoadLib_SysUnc_BkgEst_ttbar.log&
sleep 30 # -- for compile time -- #
source LoadLib_SysUnc_BkgEst.sh tW >&log_LoadLib_SysUnc_BkgEst_tW.log&

WORK_PID=`jobs -l | awk '{print $2}'` # -- WORK_PID: all PIDs in background -- #
wait $WORK_PID

source LoadLib_SysUnc_BkgEst.sh DYtautau >&log_LoadLib_SysUnc_BkgEst_DYtautau.log&
sleep 30 # -- for compile time -- #
source LoadLib_SysUnc_BkgEst.sh wjets >&log_LoadLib_SysUnc_BkgEst_wjets.log&

WORK_PID=`jobs -l | awk '{print $2}'` # -- WORK_PID: all PIDs in background -- #
wait $WORK_PID

source LoadLib_SysUnc_BkgEst.sh dijet >&log_LoadLib_SysUnc_BkgEst_dijet.log&
sleep 30 # -- for compile time -- #
source LoadLib_SysUnc_BkgEst.sh WW >&log_LoadLib_SysUnc_BkgEst_WW.log&

WORK_PID=`jobs -l | awk '{print $2}'` # -- WORK_PID: all PIDs in background -- #
wait $WORK_PID

source LoadLib_SysUnc_BkgEst.sh WZ >&log_LoadLib_SysUnc_BkgEst_WZ.log&
sleep 30 # -- for compile time -- #
source LoadLib_SysUnc_BkgEst.sh ZZ >&log_LoadLib_SysUnc_BkgEst_ZZ.log&

WORK_PID=`jobs -l | awk '{print $2}'` # -- WORK_PID: all PIDs in background -- #
wait $WORK_PID


hadd ROOTFile_RelUnc_BkgEst_CorrelationBtwBins.root ROOTFile_RelUnc_BkgEst_CorrelationBtwBins_*.root

echo "finished"