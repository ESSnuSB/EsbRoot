# ESSnuSB-soft
ESSnuSB software

# How to install
1. Install FairSoft and FairRoot as described here:\
http://essnusb.eu/docdbprivate/ShowDocument?docid=186 \
This goes quite smoothly, but it might take a long time to compile everything. If it doesnt work on your GNU/Linux distro, try to do it in our virtual machine (worked out-of-the-box for me on Mint 18.2).

2. Go to FairRoot install dir, source the config script and set FAIRROOTPATH:
cd [your FairRoot installation directory] \
source bin/FairRootConfig.sh\
export FAIRROOTPATH="$PWD"

3. compile ESSnuSB software\
cd [directory which will contain ESSnuSB-soft/ directory] 
git clone https://github.com/ESSnuSB/ESSnuSB-soft \
cd ESSnuSB-soft/ \
mkdir build \
cd build \
cmake .. \
make

The software should now be compiled!

# How to use
1. Set up the environment \
cd <ESSnuSB-soft directory>/build \
source config.sh

2. Try running macros
Go to EsbMacro directory, try running:
- ess_sim.C (root -l -b ess_sim.C) - this shoud produce evetest.root file which contains tracks and (Cherenkov) hits in a small cherenkov detector using muon as a primary track (for now).
- evetest.C - read the evetest.root file and output some information.
- diplay_event_ND.C - simple display of hits on walls of the detector
- eventDisplay.C - full 3D event display, still not working properly
