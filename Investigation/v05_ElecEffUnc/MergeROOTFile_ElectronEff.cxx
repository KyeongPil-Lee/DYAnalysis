class EffROOTFileMerger
{
public:
  TString fileName_reco_;
  TString fileName_ID_;

  Bool_t addUncFromDiffWithOld_;
  TString fileName_reco_old_;
  TString fileName_ID_old_;

  TString fileName_output_;


  void EffROOTFileMerger()
  {
    fileName_reco_ = "";
    fileName_ID_ = "";

    addUncFromDiffWithOld_ = kFALSE;
    fileName_reco_old_ = "";
    fileName_ID_old_ = "";

    fileName_output_ = "";
  }

  void Set_EffROOTFile( TString fileName_reco, TString fileName_ID )
  {
    fileName_reco_ = fileName_reco;
    fileName_ID_ = fileName_ID;
  }

  void Set_OldEffROOTFile( TString fileName_reco, TString fileName_ID )
  {
    fileName_reco_old_ = fileName_reco;
    fileName_ID_old_ = fileName_ID;
  }

private:

};

void MergeROOTFile_ElectronEff()
{
  EffROOTFileMerger* merger = new EffROOTFileMerger();
  merger->Set_EffROOTFile( "ROOTFile_RecoSF_fromDM.root", "ROOTFile_IDSF_fromDM.root" );

  merger->addUncFromDiffWithOld_ = kTRUE;
  merger->Set_OldEffROOTFile( "ROOTFile_RecoSF_SMP17001.root", "ROOTFile_IDSF_SMP17001.root" );
  merger->fileName_output_ = "ROOTFile_ElecRecoIDEff_April2019.root";


}