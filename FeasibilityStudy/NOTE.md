## Luminosity

### 2016

* Command

  ```
  brilcalc lumi --normtag /afs/cern.ch/user/l/lumipro/public/Normtags/normtag_DATACERT.json -i /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions16/13TeV/ReReco/Final/Cert_271036-284044_13TeV_23Sep2016ReReco_Collisions16_JSON.txt -u /pb 
  ```

* Result

  ```
  +-------+------+--------+--------+-------------------+------------------+
  | nfill | nrun | nls    | ncms   | totdelivered(/pb) | totrecorded(/pb) |
  +-------+------+--------+--------+-------------------+------------------+
  | 144   | 393  | 232259 | 232241 | 37381.753         | 35863.818        |
  +-------+------+--------+--------+-------------------+------------------+
  ```



### 2017

* Command

  ```
  brilcalc lumi --normtag /cvmfs/cms-bril.cern.ch/cms-lumi-pog/Normtags/normtag_BRIL.json -i /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions17/13TeV/ReReco/Cert_294927-306462_13TeV_EOY2017ReReco_Collisions17_JSON.txt -u /pb
  ```

* Result

  ```
  +-------+------+--------+--------+-------------------+------------------+
  | nfill | nrun | nls    | ncms   | totdelivered(/pb) | totrecorded(/pb) |
  +-------+------+--------+--------+-------------------+------------------+
  | 175   | 474  | 206562 | 205443 | 44168.972         | 41526.280        |
  +-------+------+--------+--------+-------------------+------------------+
  ```


### 2016 + 2017

35863.818 + 41526.280 = 77390.098 ~ 77.4 /fb