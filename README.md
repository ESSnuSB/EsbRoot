# EsbRoot
Software for physics simulation of ESSnuSB project.

Doxygen documentation can be found here: \
https://essnusb.irb.hr/doxygen/

# How to install
This guide is heavily based on this talk: \
http://essnusb.eu/docdbprivate/ShowDocument?docid=186 \

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
./configure
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

## 3. Install EsbRoot
Go to FairRoot install dir, source the config script and set FAIRROOTPATH:
```
cd $INSTALL_PATH/fairroot
source bin/FairRootConfig.sh
export FAIRROOTPATH="$PWD"
```
Build EsbRoot
```
cd $INSTALL_PATH
git clone https://github.com/ESSnuSB/EsbRoot
cd EsbRoot
mkdir build
cd build
cmake ..
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
