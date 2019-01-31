# EsbRoot
Software for physics simulation of ESSnuSB project

# How to install
1. Install FairSoft and FairRoot as described here:\
http://essnusb.eu/docdbprivate/ShowDocument?docid=186 \
**IMPORTANT:** Do **not** check-out specific versions of FairSoft and FairRoot.

That is, when installing FairSoft, ignore this line:
```
git checkout may18
```
When istalling FairRoot, ignore this line:
```
git checkout v-17.10d
```
This goes quite smoothly, but it might take a long time to compile everything. If it doesnt work on your GNU/Linux distro, try to do it in our virtual machine (worked out-of-the-box for me on Mint 18.x).

2. Go to FairRoot install dir, source the config script and set FAIRROOTPATH: \
cd [your FairRoot installation directory] \
source bin/FairRootConfig.sh\
export FAIRROOTPATH="$PWD"

3. compile ESSnuSB software\
cd [directory which will contain EsbRoot/ directory] \
git clone https://github.com/ESSnuSB/EsbRoot \
cd EsbRoot/ \
mkdir build \
cd build \
cmake .. \
make

The software should now be compiled!

# How to use
1. Set up the environment \
cd [EsbRoot directory]/build \
source config.sh

2. Try running macros
Go to EsbMacro directory, try running:
- ess_sim.C (root -l -b ess_sim.C) - this shoud produce evetest.root file which contains tracks and (Cherenkov) hits in a small cherenkov detector using muon as a primary track (for now).
- evetest.C - read the evetest.root file and output some information.
- diplay_event_ND.C - simple display of hits on walls of the detector
- eventDisplay.C - full 3D event display, still not working properly
