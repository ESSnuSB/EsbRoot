#ifndef ESBROOT_SIMULATION_SUPERFGD_FILE_WRITER_H
#define ESBROOT_SIMULATION_SUPERFGD_FILE_WRITER_H 1

#include <iostream>
#include <memory>

#include <TFile.h>
#include <TTree.h>

#include "Framework/EventGen/EventRecord.h"
#include "Framework/GHEP/GHepParticle.h"

#include "EsbData/EsbSuperFGD/EsbDetector/EsbCubeHit.h"
#include "EsbData/EsbSuperFGD/EsbDetector/EsbFiberHit.h"
#include "EsbData/EsbSuperFGD/EsbDetector/EsbFgdDetectorParameters.h"
#include "EsbData/EsbSuperFGD/EsbDetector/EsbG4EventRecord.h"
#include "EsbData/EsbSuperFGD/EsbDetector/EsbG4StepsRecord.h"

#define PI 3.14159265

using namespace std;
using namespace genie;

namespace esbroot {
namespace simulators {
namespace superfgd {

class FileWriter
{
public:

    /** Default constructor **/
    FileWriter(const char* filename, int compressionLevel); //compressionLevel - from 0 (not compressed) to 9 (max compression)

    /** Destructor **/
    virtual ~FileWriter();

    /** Add event to be written to the external file
     *@param event - step in the simulation
     **/
    void AddEvent(EventRecord* event);

    /** Add the particle to be process for angle and momentum
     *@param p - particle [used only for muons and protons]
     **/
    void AddGHepParticle(GHepParticle* p);

    /** Add fiber hit to be process and written to file
     *@param hit - fiber hit
     **/
    void AddFiberHit(FiberHit* hit);

    /** Add CubeHit to be process and written to file
     *@param hit - cube hit
     **/
    void AddCubeHit(CubeHit* hit);

    /** Sum all the steps for the given cube in the event
     *@param hit - cube hit
     *@param dp - current detector parameters
     **/
    void SumStep(CubeHit* hit, data::superfgd::detector::DetectorParameters& dp);

    /** Write hit to external file **/
    void WriteHit();

    /** Add the vertex of the event
     *@param xpos - x coordinates
     *@param ypos - y coordinates
     *@param zpos - z coordinates
     **/
    void AddVertexPos(double xpos, double ypos, double zpos);

    /** Set verbosity level for file writer
     *@param verbose - output to log
     **/
    void SetVerbose(bool verbose){fverbose=verbose;}

private:
    shared_ptr<TFile> ffile;
    shared_ptr<TTree> fhitsTree;

    shared_ptr<G4EventRecord>  fcubeHits;
    std::vector<G4StepsRecord> fsteps;
    std::map<int,std::vector<G4StepsRecord>> ftracks;
    G4StepsRecord fvertex;

    bool fverbose;
};

}   //superfgd
}   //simulators
}   //esbroot

#endif