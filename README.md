# EsbRoot
Software for physics simulation of the ESSnuSB project

Doxygen documentation can be found here: \
https://essnusb.irb.hr/doxygen/

# How to install
This guide is heavily based on this talk: \
http://essnusb.eu/docdbprivate/ShowDocument?docid=186 \

## FairRoot prerequisites

If you have a clean OS check
https://github.com/FairRootGroup/FairSoft/blob/master/DEPENDENCIES
what you need to install first.

## 0. Prepare the directory
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

## 4 Install GenFit (optional for now)

Install the prerequisites for GenFit:

sudo apt-get install libeigen3-dev
```
Download the getfit git repository from 
https://github.com/GenFit/GenFit
```
Follow the build instructions from 
https://github.com/GenFit/GenFit/blob/master/README.build

```
NOTE: the following environmental variables have to be exported
export ROOTSYS=/** PATH TO FAIRSOFT **/fairsoft
export GENFIT_INCLUDE=/** PATH TO Downloaded GenFit directory **/
export GENFIT_LIBRARY=/** PATH TO the bin directory in the used build folder**/
```

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
export GENFIT_INCLUDE=/** PATH TO Downloaded GenFit directory **/
export GENFIT_LIBRARY=/** PATH TO the bin directory in the used build folder**/
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
