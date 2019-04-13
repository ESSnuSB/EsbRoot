#ifndef ESBROOT_SIMULATION_SUPERFGD_FGD_RUN_MANAGER_H
#define ESBROOT_SIMULATION_SUPERFGD_FGD_RUN_MANAGER_H 1

#include <memory>
#include "G4RunManager.hh"

#include "EsbSimulators/EsbSuperFGD/EsbFileWriter.h"

namespace esbroot {
namespace simulators {
namespace superfgd {

class FgdRunManager : public G4RunManager
{
public:

    /** Default constructor **/
    FgdRunManager();

    /** Destructor **/
    virtual ~FgdRunManager();
    
    /** Retrieve the current set file writer **/
    std::shared_ptr<FileWriter> GetFileWriter(){return ffileWriter;}

    /** Set file writer to be used to write the events to an output file
     *@param fileWriter - file writer class
     **/
    void SetFileWriter(std::shared_ptr<FileWriter> fileWriter) {ffileWriter=fileWriter;}

    /** Set the flag to ignore total deposited energy in the event
     *@param ignoreEdep - file writer class
     **/
    void SetIgnoreEdep(bool ignoreEdep) {fignoreEdep=ignoreEdep;}

    /** Retrieve the flag to ignore total deposited energy **/
    bool GetIgnoreEdep(){return fignoreEdep;}
    
private:
    std::shared_ptr<FileWriter> ffileWriter;
    bool fignoreEdep;
};

}   //superfgd
}   //simulators
}   //esbroot

#endif