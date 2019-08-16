class ZXSecProducer
{
public:
  Double_t ZXSec_ee_;
  Double_t absUnc_ee_;

  Double_t ZXSec_mm_;
  Double_t absUnc_mm_;


  ZXSecProducer()
  {

  }

  void Set_ee(Double_t ZXSec_ee, Double_t absUnc_ee)
  {
    ZXSec_ee_ = ZXSec_ee;
    absUnc_ee_ = absUnc_ee;
  }

  void Set_mm(Double_t ZXSec_mm, Double_t absUnc_mm)
  {
    ZXSec_mm_ = ZXSec_mm;
    absUnc_mm_ = absUnc_mm;
  }

  void Produce()
  {
    Double_t invSq_ee = 1.0 / (absUnc_ee_*absUnc_ee_);
    Double_t invSq_mm = 1.0 / (absUnc_mm_*absUnc_mm_);

    Double_t frac_ee = invSq_ee / (invSq_ee + invSq_mm);
    Double_t frac_mm = invSq_mm / (invSq_ee + invSq_mm);

    Double_t ZXSec_ll = frac_ee*ZXSec_ee_ + frac_mm*ZXSec_mm_;

    printf("ZXSec_ll = %lf\n", ZXSec_ll);
  }

private:

};

void CalcZXSec_UnCorr()
{
  ZXSecProducer* producer_old = new ZXSecProducer();
  producer_old->Set_ee(1903.72, 68.2273);
  producer_old->Set_mm(1916.31, 36.227);
  producer_old->Produce();

  ZXSecProducer* producer_new = new ZXSecProducer();
  producer_new->Set_ee(1888.06, 36.559);
  producer_new->Set_mm(1916.31, 36.227);
  producer_new->Produce();

}