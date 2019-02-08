This directory contains ESSnuSB interaction generators, i.e. implementations of FairGenerator abstract class. Pythia generators are left as an example.


In order to run install Genie the following steps should be done:


1./ During installation if it cannot find Mathmore library 
check its existence in the "root" folder. It should be :

$ROOTSYS/lib/root is located libMathMore.so (on the current version with FairRoot installation)

in the document in looks lie:
update Genie/configure 
$mathmore_lib = "$ROOTSYS/lib/libMathMore.so";
  if( ! -f $mathmore_lib ) {
     die ("*** Error *** ROOT needs to be built with GSL/MathMore enabled.");
  }

to
$mathmore_lib = "$ROOTSYS/lib/root/libMathMore.so";

2./ run the configuration file with 
$configure --disable-lhapdf5
to disable lhapdf5 since we do not have it installed

3./
export GENIE=/path_to_GENIE_installation
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$GENIE/lib
export PATH=$PATH:$GENIE/src

4./ Donwload cross_sections.xml for genie
and  set the following variable
export CSFILE=/path_to_cross_sections.xml
e.g.
export CSFILE="/home/georgi/Downloads/genie_xsec/v3_00_02/NULL/G0000a00000-k500-e1000/data/gxspl-FNALsmall.xml"

5./ edint ~/build/config.sh -> the path of the Genie libraries has to be added 
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$GENIE"/lib"
(NOTE this should be updated in the CMake file to set it automatically)


6./ For the moment, root cannot find its libraries.
So when running through 
$root 
the following libraries have to be loaded manually
gSystem->Load("libGFwMsg");
gSystem->Load("libGFwReg");
gSystem->Load("libGFwAlg");
gSystem->Load("libGFwParDat");
gSystem->Load("libPhysics");
gSystem->Load("libGFwInt");
gSystem->Load("libGFwGHEP");
gSystem->Load("libxml2");
gSystem->Load("libTree");
gSystem->Load("libGFwEG");
gSystem->Load("libGFwNtp");
gSystem->Load("libHist");
gSystem->Load("libGFwNum");
gSystem->Load("libGFwUtl");
gSystem->Load("liblog4cpp");
gSystem->Load("libGTlFlx");
gSystem->Load("libGeom");
gSystem->Load("libGTlGeo");






