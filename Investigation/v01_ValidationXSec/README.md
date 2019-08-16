## Motivation

* The efficiency correction in the fiducial cross section in the electron channel was missed
* The cross section is updated after applying the efficiency correction
* Need to **validate** the updated result so that it is confirmed that there is no more issue
  * "Validate" mean: is the change same with our expectation?
  * Expectation
    * Data: only efficiency correction is updated. Nothing more is changed.
    * MC: Also only efficiency correction is updated. Nothing more is changed.



## How to validate?

### 1) Fiducial cross section after update / before update == efficiency?

* Central value: after/before == efficinecy?
* Uncertainty: unchanged?



### 2) Fiducial MC cross section after update / before update == efficiency?

* Before update: from Ridhi / After udpate: mine
* MC cross section from Ridhi == my MC cross section? 





## Results

### Data

Validation: done (with the latest root file provided by Ridhi: v9 root file)



### MC

Validation is failed: need investigation



## Investigation on the difference of MC cross section

Both results come from gen-level only



### Difference in the low mass region (M10-50)

* KP: nominal M10-50 sample
  * /DYJetsToLL_M-10to50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM
    * nEvent = 30899063

* Ridhi: high-statistics M10-50 sample
  * /DYJetsToLL_M-10to50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12_**ext3**-v1/MINIAODSIM
    * nEvent = 76558711



* Reproduce MC cross section with high-statistics M10-50 sample
  * Have flat ratio with Ridhi's result





### Normalization factor check

* Cross section
  * M10-50: Same
  * M50-100:  5869.58346/3.0 (KP) vs. 5870./3 (Ridhi): 0.007% difference (negligblie)
    * KP's result is better one (M50-inf sample x-section - M>100 sample x-section)
    * But the difference is negligblie. No problem.
  * M > 100
    * Ridhi * 1.032418 = KP
    * Only SF (1.033504) difference
    * Was it not used in electron channel? - ask Ridhi
* Sum(weight)
  * M10-50: difference = 0.000062 -> negligible
  * M50-inf: KP * 1.017494 = Ridhi - > why? ask Ridhi
    * Check the sum(weight) of
      * Full M50-inf events
      * Cut by fromHardProcessFinalState
  * M > 100 sample except for M2000-3000 sample: negligible (<0.1%)
  * M2000-3000: KP * 0.463457 = Ridhi -> why? ask Ridhi
    * No the other high mass sample for M2000-3000
    * But her and my result (MC cross section) show very good agreement: maybe not a serious problem



### Difference of Sum(weight) in M50-100 sample

* Test of dilepton mass cut at M=100 GeV on post-FSR mass
  * Sum(weight)
    * KP: 6058012
    * Ridhi: 6163990
    * KP+post-FSR mass cut: 6098494.000000
  * Still the difference exists
* Check the sum(weight) of M50-inf sample (not truncated at 100 GeV)
  * Sum(weight)
    * 6307433.000000
  * Too large compared to the Ridhi's sum(weight)
* Check DY->mumu sum(weight) (M50-100)
  * Sum(weight)
    * 6061181.0
  * Different with Ridhi's sum(weight)
* Different MC sample?
  * KP: /DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM
    * nEvent = 28751199
  * Ridhi: ?
    * Several options
      * /DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_HCALDebug_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM
        * nEvent = 29193937
      * /DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12_ext1-v2/MINIAODSIM
        * nEvent = 163982720
      * /DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12_ext4-v1/MINIAODSIM
        * nEvent = 121212419
    * First option could be possible
      * First option/KP (nEvent) = 1.0153
        * Similar to the difference in sum(weight)
    * But she confirmed that she used same sample :(
* Check whether I processed full statistics
  * It was found that **1 root file was not processed in my ntuples among 346 files**
    * Fraction: 0.00289017341
  * Fraction is too small to explain full differences (1.7%)
  * But, at least the amount of wiggle in the cross section can be explained (~0.2% diff.)



### Produce MC x-section without additional SF

* KP +
  * High-statistics M10-50 sample
  * Remove additional SF on M > 100 GeV sample
* Result: Ratio becomes flat over full mass range



### Check the normalizaton by luminosity

- Golden JSON
  - Old: 2318.348
  - New: 2257.998 (2.67% lower)
- Luminosity that Ridhi used in acceff.C code: 2258.066
  - It is different with my values, but close to new luminosity



The difference between KP and Ridhi's result: flat ~2.67%

* It could happen if
  * Ridhi normalize the MC mass distribution to the **old** luminosity
  * But she divide it by **new** luminosity when she made cross section
  * Give it try from my side -> got ratio very close to 1
* Ridhi confirms that she made the mistake & provide new root file fixing the bug
  * **It shows very good agreement with the ratio close to 1**: Validation for MC is finished



## Update figure/AN note/paper

### Paper

Things to be updated: figure 5, table 6 and nothing else



* Figure 5: updated
  * Send it Hwidong?
* Table 6: updated in AN note



* Synchronization of table format between the AN note and the paper
  * Validation (difference check between paper (.tex format) and the output from code)
    * Systematic uncertainty table
      * Muon: done
      * Electron: done
    * Cross section (full phase space)
      * Muon: done
      * Electron: done
      * Combined result: done
    * Cross section (fiducial)
      * Muon: done
      * Electron: should be different (as updated)
  * Synchronization the format between AN note and paper
    * Systematic uncertainty table: done
    * Cross section (full phase space)
      * Muon: done
      * Electron: done
      * Combined result: done
    * Cross section (fiducial)
      * Muon: done
      * Electron: done

