# Procedure for HEP data preparation

* Reference: https://www.hepdata.net/submission



## Submission procedure

1. Coordinator -> Assign an uploader and reviewer for a HEPData entry submssion
2. Uploader -> Get a mail & privilegeds to upload the data
3. Once uploader uploaded the data & the entry is ready to review -> click the "Notify Reviewer" button
4. Reviewer will get a mail with the link for the entry & start to review -> mark each table as "Passed" (it will be "finalise" by coordinator later)
5. The entry will be published & searchable in HEPData



* All submission procedure can be checked by Dashboard



### Advice for uploaders

* HEPData format YAML: documentation -> https://hepdata-submission.readthedocs.io/en/latest/
* Third-party helpful library to prepare YAML: https://github.com/HEPData/hepdata_lib/
* YAML for an existing HEPData entry will also be helpful
  * For our case, 8 TeV HEPData entry: https://www.hepdata.net/record/ins1332509
    * YAML: Download All -> YAML



## Instruction from SMP HEPData contact

* Login in HEPData homepage
* Use sandbox button to test my YAML file
* Use submit button when everything is ready



## Instruction from HEPData webpage

* Reference: https://hepdata-submission.readthedocs.io/en/latest/introduction.html



* Submission: archive files (.zip, .tar, .tar.gz, .tgz) containing YAML files
  * YAML format: https://en.wikipedia.org/wiki/YAML
* Main file: ```submission.yaml```
  * Links to all tables and auxiliary files (scripts, ROOT file, links to somewhere ...)
* Publication information (paper title, authors, abstract, journal reference + DOI ...)
  * Automatically generated when a Coordinator attached INSPIRE ID



* Examples
  * https://github.com/HEPData/hepdata_lib
    * Library from the CMS collaboration
    * Reading ROOT histogram / writing into YAML format



* Data files
  * Variables
    * Independent variable (e.g. x-axis)
    * Dependent variable (what was measured, y-axis)
  * Structure of variable
    * header: definition of variable + unit (unless it is dimentionless)
    * For the dependent variable: qualifier can be defined
      * extra information: energy, pT & eta cuts ...





## hepdata_lib

### Install

```
sudo pip install hepdata_lib
```

### Reading from ROOT files

```
from hepdata_lib import RootFileReader
reader = RootFileReader("/path/to/myfile.root")
```

* Read individual objects

  ```
  RootFileReader.read_graph
  RootFileReader.read_hist_1d
  RootFileReader.read_hist_2d
  ```

### Writing data



## HEPData entry procedure

### Read the central value of the cross section (full phase space, muon channel)





