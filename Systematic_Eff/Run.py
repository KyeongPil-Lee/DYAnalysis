import sys
import os
import time
from getopt import gnu_getopt as getopt
from ZprimeAnalysis_80X.Include.Split_Batchjobs import SplitJobs

def instruction():
    print sys.argv[0], " : split the jobs"
    print "  Mandatory options :"
    print "   --code CODE.cxx                  C++ Code file name"
    print "   --lumi LUMI                      Integrated lumionsity in pb"
    print "  Optional options :"
    print "   --queue queueName                      queue name (default: fastq)"
    sys.exit()

# Parse arguments
if len(sys.argv) < 2: instruction()
try:
    MainOpts, MainArgs = getopt(sys.argv[1:], 'n', ["code=", "lumi=", "queue="])
    MainOpts = dict(MainOpts)
except:
    print "!!! Error parsing arguments"
    instruction()

def Add_DYPowheg( _List_Samples ):
	List_Samples.append( ["DYPowheg_M50to120", 1, 5] )
	List_Samples.append( ["DYPowheg_M120to200", 1, 1] )
	List_Samples.append( ["DYPowheg_M200to400", 1, 1] )
	List_Samples.append( ["DYPowheg_M400to800", 1, 1] )
	List_Samples.append( ["DYPowheg_M800to1400", 1, 1] )
	List_Samples.append( ["DYPowheg_M1400to2300", 1, 1] )
	List_Samples.append( ["DYPowheg_M2300to3500", 1, 1] )
	List_Samples.append( ["DYPowheg_M3500to4500", 1, 1] )
	List_Samples.append( ["DYPowheg_M4500to6000", 1, 1] )
	List_Samples.append( ["DYPowheg_M6000toInf", 1, 1] )


# -- main -- #
# -- SampleName, isMC, nJobs -- #
List_Samples = []
# List_Samples.append( ["Data", 0, 20] )
# Add_DYPowheg( List_Samples )
List_Samples.append( ["WWTo2L2Nu", 1, 1] )
# List_Samples.append( ["WZ", 1, 1] )
# List_Samples.append( ["ZZ", 1, 1] )
# List_Samples.append( ["ttbarTo2L2Nu", 1, 10] )
# List_Samples.append( ["tW", 1, 2] )
# List_Samples.append( ["tbarW", 1, 2] )

# -- output directory -- //
TIME = time.strftime('%Y%m%d_%H%M%S', time.localtime(time.time()))
OutDir = os.path.expanduser( "~/Physics/ZprimeAnalysis_80X/Outputs/v%s_%s" % (TIME, CODE.split['.cxx'][0] ) )
os.mkdir( OutDir )

# -- convert path for code to absolute path -- #
CodeAbsPath = os.path.abspath( MainOpts['--code'] )

for Samples in List_Samples:
	SampleName = Samples[0]
	isMC = (int)Samples[1]
	nJob = (int)Samples[2]

	argument = "--code %s --sample %s --njob %d --lumi %.3lf --isMC %d --outdir %s" % (CodeAbsPath, SampleName, nJob, MainOpts['--lumi'], isMC, OutDir )

	if '--queue' in MainOpts:
		argument = argument + " --queue %s" % (MainOpts['--queue'])

	try:
	    opts, args = getopt(argument, 'n', ["code=", "sample=", "njob=", "lumi=", "isMC=", "queue=", "outdir="])
	    opts = dict(opts)
	except:
	    print "!!! Error parsing arguments"
	    sys.exit()

    SplitJobs( opts )