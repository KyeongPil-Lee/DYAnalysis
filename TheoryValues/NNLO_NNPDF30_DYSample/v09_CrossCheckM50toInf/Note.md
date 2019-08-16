### Differences with the reference

1) PI contribution -> no PI contribution

2) nlo PDF -> nnlo PDF (NNPDF30_nnlo_as_0118)

3) Several variables

* Renormalization & factorization scale: 91.1876 -> 91.0
* Fermi constant: 1.18d-5 -> 1.16637d-5
  * 1.16637d-5 is closer to the precise value: [link](https://physics.nist.gov/cgi-bin/cuu/Value?gf)
* W mass: 80.398d0 -> 80.403d0
* Absolute uncertainty: 3d-2 -> 1.95652782d0



### Jobs for cross check

* RefSetup (already done)
* RefSetup + no PI (run in cms1)
* RefSetup + no PI + nnlo PDF (run in cms1)
* RefSetup + no PI + nnlo PDF + Syncronization of variable values (run in cms2)



### Results

* Reference setup
  *  **1921.25** +- 0.155836 (integration) +- 33.188 (PDF) pb
* RefSetup + no PI (run in cms1)
* RefSetup + no PI + nnlo PDF
* RefSetup + no PI + nnlo PDF + Syncronization of variable values
  * **1956.71** +- 1.43959 (integration) +- 41.6385 (PDF) pb

