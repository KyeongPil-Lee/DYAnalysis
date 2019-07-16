#!/bin/bash

root -b -q 'sumWeight_ele.cxx++("/scratch/kplee/DY2015Ntuple_ee/DY_10to50.root")' >&sumWeight_DY_10to50.log&
root -b -q 'sumWeight_ele_M100.cxx++("/scratch/kplee/DY2015Ntuple_ee/DY_50toInf.root")' >&sumWeight_DY_50to100.log&
root -b -q 'sumWeight_ele.cxx++("/scratch/kplee/DY2015Ntuple_ee/DY_100to200.root")' >&sumWeight_DY_100to200.log&
root -b -q 'sumWeight_ele.cxx++("/scratch/kplee/DY2015Ntuple_ee/DY_200to400.root")' >&sumWeight_DY_200to400.log&
root -b -q 'sumWeight_ele.cxx++("/scratch/kplee/DY2015Ntuple_ee/DY_400to500.root")' >&sumWeight_DY_400to500.log&
root -b -q 'sumWeight_ele.cxx++("/scratch/kplee/DY2015Ntuple_ee/DY_500to700.root")' >&sumWeight_DY_500to700.log&
root -b -q 'sumWeight_ele.cxx++("/scratch/kplee/DY2015Ntuple_ee/DY_700to800.root")' >&sumWeight_DY_700to800.log&
root -b -q 'sumWeight_ele.cxx++("/scratch/kplee/DY2015Ntuple_ee/DY_800to1000.root")' >&sumWeight_DY_800to1000.log&
root -b -q 'sumWeight_ele.cxx++("/scratch/kplee/DY2015Ntuple_ee/DY_1000to1500.root")' >&sumWeight_DY_1000to1500.log&
root -b -q 'sumWeight_ele.cxx++("/scratch/kplee/DY2015Ntuple_ee/DY_1500to2000.root")' >&sumWeight_DY_1500to2000.log&
root -b -q 'sumWeight_ele.cxx++("/scratch/kplee/DY2015Ntuple_ee/DY_2000to3000.root")' >&sumWeight_DY_2000to3000.log&

echo "submission: finished"
