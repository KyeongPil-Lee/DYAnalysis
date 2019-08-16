class PlotProducer
{
public:
  TH1D* h_absUnc_tot_ee_;
  TH1D* h_absUnc_tot_mm_;
  TH1D* h_absUnc_tot_ll_;

  TH1D* h_relUnc_tot_ee_;
  TH1D* h_relUnc_tot_mm_;
  TH1D* h_relUnc_tot_ll_;

private:

};

void ComparisonPlot()
{
  PlotProducer* producer = new PlotProducer();
  producer->Produce();
}