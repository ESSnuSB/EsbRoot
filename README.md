# EsbRoot
Software for physics simulation of the ESSnuSB project

Doxygen documentation can be found here: \
https://essnusb.irb.hr/doxygen/

# How to install
This guide is heavily based on this talk: \
http://essnusb.eu/docdbprivate/ShowDocument?docid=186 \

## 0.1 Install prerequisites
Debian-based distributions (e.g. Linux Mint): \
sudo apt-get install cmake cmake-data g++ gcc gfortran debianutils build-essential make patch sed libx11-dev libxft-dev libxext-dev libxpm-dev libxmu-dev libglu1-mesa-dev libgl1-mesa-dev libncurses5-dev curl libcurl4-openssl-dev bzip2 libbz2-dev gzip unzip tar subversion git xutils-dev flex bison lsb-release python-dev libc6-dev-i386 libxml2-dev wget libssl-dev libkrb5-dev automake autoconf libtool

## 0.2 Prepare the directory
For example, we will install everything in ~/ESSnuSB/soft/ . Change the INSTALL_PATH variable if you want to put it somewhere else.
```
export INSTALL_PATH=~/ESSnuSB/soft/
mkdir -p $INSTALL_PATH
cd $INSTALL_PATH
```
We will put soures in $INSTALL_PATH/src
```
mkdir src
cd src
```
## 1. Install FairSoft
``` 
git clone https://github.com/ESSnuSB/FairSoft.git FairSoft
cd FairSoft
./configure.sh
```
In the configrue script chose:
```
1) GCC (on Linux)|| Intel Compiler (Linux) || CC (Solaris) || Portland Compiler || Clang (Mac OSX)
1) No Debug Info|| Debug Info || Optimize || Optimize with Debug Info
2) No (install FairMQOnly)|| Yes
1) Yes (install Simulation engines and event generators)|| No
2) Internet (install G4 additional files from internet)|| Don't install || Directory
1) Yes (install the python bindings for ROOT and Geant4)|| No
path: $INSTALL_PATH/fairsoft
```
Go get a cofee until it compiles. It takes a while.

## 2. Install FairRoot
Prepare the environment:
```
export SIMPATH=$INSTALL_PATH/fairsoft
export PATH=$SIMPATH/bin:$PATH
```
Get FairRoot
```
cd $INSTALL_PATH/src
git clone https://github.com/ESSnuSB/FairRoot.git FairRoot
```
Build and install
```
cd FairRoot
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX="$INSTALL_PATH/fairroot" ..
make
make install
```
Now FairRoot and Fairsoft should be installed in $INSTALL_PATH. Take a look

## 3 Install Genie (optional for now)
Install the prerequisites for Genie: log4cpp, libxml2, and LHAPDF. On Ubuntu based system (like Mint), you can do this using a command:
```
sudo apt-get install liblog4cpp5-dev libxml2-dev liblhapdf-dev
```

Source the FairRoot configuration script to get all the environmental variables.
```
source $INSTALL_PATH/fairroot/bin/FairRootConfig.sh
```

The build system of Genie-Generator has been modified so that the entire Genie generator is compiled into a single library. This allows easy linking and usage from ROOT. So, you need to get Genie from the ESSnuSB repository
```
cd $INSTALL_PATH/src
git clone https://github.com/ESSnuSB/Genie-Generator.git Genie
cd Genie
git checkout essnusb-current
```
This checks out Genie version currently recommended to use with EsbRoot. You can see the verson by looking at the file
```
$INSTALL_PATH/src/Genie/VERSION
```

Link and enter the Genie directory 
```
cd $INSTALL_PATH
ln -s $INSTALL_PATH/src/Genie GENIE
cd GENIE
```

Budild Genie
```
export GENIE="$PWD"
./configure
make megalib
```
Genie should now be built.

```
NOTE: to use Genie, the cross sections have to be downloaded (see: http://scisoft.fnal.gov/scisoft/packages/genie_xsec/).
The following variable has to be defined in the macro files 
generators::GenieGenerator::GlobalState.fXsecSplineFileName = "PATH_TO_CROSS_SECTION.xml"; 
```

## 4 Install GenFit and PathFinder (optional for now)

Install the prerequisites for GenFit:

sudo apt-get install libeigen3-dev
```
Download the getfit git repository from 
https://github.com/ESSnuSB/GenFit
```
Follow the build instructions from 
https://github.com/ESSnuSB/GenFit/blob/master/README.build

```
NOTE: the following environmental variables have to be exported
export ROOTSYS=/** PATH TO FAIRSOFT **/fairsoft
export GENFIT_HOME=/** PATH TO Downloaded GenFit directory **/
export GENFIT_BUILD=/** PATH TO the build directory in the used build folder**/
```

Download PathFinder from 
```
https://github.com/ESSnuSB/PathFinder
```

The only prerequisite is to have CERN ROOT installed (which should be installed with Fairsoft installation).
Create a folder and execute 
```
$cmake PATH_TO_PATHFINDERSOURCES
$make
```
After the build is successdull two enviormental variables have to be defined.
One to the include directory which is located 'include' folder of the downloaded Pathdinder source 
directory, and the second environemntal variable is to the 'lib' folder in the Pathfinder build directory (the used cmake folder).
```
export PATHFINDER_HOME=/** PATHFINDER extracted directory containing the sources **/
export PATHFINDER_BUILD=/** PATHFINDER build directory **/
```
When building the EsbRoot application with the -DWITH_GENFIT=ON option, both 
Genfit and Pathfinder will be included in EsbRoot.

## 5. Install EsbRoot
Go to FairRoot install dir, source the config script and set FAIRROOTPATH:
```
cd $INSTALL_PATH/fairroot
source bin/FairRootConfig.sh
export FAIRROOTPATH="$PWD"
```
Get EsbRoot and prepare build directory
```
cd $INSTALL_PATH
git clone https://github.com/ESSnuSB/EsbRoot
cd EsbRoot
mkdir build
cd build
```
To compile without Genie support, just do
```
cmake ..
make
```

To compile with Genie support, do
```
export GENIE=$INSTALL_PATH/GENIE
cmake -DWITH_GENIE=ON ..
make
```
To compile with Genie and GenFit support, do
```
export GENIE=$INSTALL_PATH/GENIE
export GENFIT_HOME=/** PATH TO Downloaded GenFit directory **/
export GENFIT_BUILD=/** PATH TO the build directory in the used build folder**/
export PATHFINDER_HOME=/** PATHFINDER extracted directory containing the sources **/
export PATHFINDER_BUILD=/** PATHFINDER build directory **/
cmake -DWITH_GENIE=ON -DWITH_GENFIT=ON ..
make
```

The software should now be compiled!

# How to use
1. Set up the environment
```
cd [EsbRoot directory]/build
source config.sh
```

2. Try running macros

Go to EsbMacro directory, try running:
- ess_sim.C (```root -l -b ess_sim.C```) - this shoud produce evetest.root file which contains tracks and (Cherenkov) hits in a small cherenkov detector using muon as a primary track (for now).
- evetest.C - read the evetest.root file and output some information.
- diplay_event_ND.C - simple display of hits on walls of the detector
- eventDisplay.C - full 3D event display, still not working properly
- ess_sim_genie.C - example how to run with Genie generator. You will need to download the Genie cross-sections, see the code for comments.

