#!/bin/bash
if [ $HOME == "/Users/KyeongPil_Lee" ];
  then
  echo "Change ROOT version: ROOT6 -> ROOT5"
  source /Users/KyeongPil_Lee/ROOT5/root/bin/thisroot.sh
fi

root -l -b << EOF

TString IncludePath = gSystem->Getenv("KP_ANALYZER_PATH")
gInterpreter->AddIncludePath( TString::Format("%s", IncludePath.Data()) )

TString IncludePath2 = gSystem->Getenv("KP_ROOUNFOLD_PATH")
gInterpreter->AddIncludePath( TString::Format("%s", IncludePath2.Data()) )

TString BasePath = gSystem->Getenv("KP_ROOUNFOLD_PATH")
gSystem->Load(BasePath+"/libRooUnfold.so")

.x SmearedDXSec_EffSF_tagChange.cxx

.q

EOF

if [ $HOME == "/Users/KyeongPil_Lee" ];
  then
  echo "Return to latest ROOT version: ROOT5 -> ROOT6"
  source /Users/KyeongPil_Lee/ROOT6/root/bin/thisroot.sh
fi

echo "finished"