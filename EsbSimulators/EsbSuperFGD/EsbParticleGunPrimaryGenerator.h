#ifndef SIMULATION_NFPARTICLE_GUN_PRIMARY_GENERATOR_H
#define SIMULATION_NFPARTICLE_GUN_PRIMARY_GENERATOR_H 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "G4ThreeVector.hh"
#include "globals.hh"

#include "EsbData/EsbSuperFGD/EsbDetector/EsbFgdDetectorParameters.h"
#include "CLHEP/Units/SystemOfUnits.h"
#include "CLHEP/Units/PhysicalConstants.h"
#include "EsbTools/ParamReader.h"

#include <memory>


namespace esbroot {
namespace simulators {
namespace superfgd {

namespace PG
{
    static const string  events = "events";
    static const string  particlePdg = "particlePdg";
    static const string  particleEnergy = "particleEnergy";

    static const string  positon_X = "positon_X";
    static const string  positon_Y = "positon_Y";
    static const string  positon_Z = "positon_Z";

    static const string  momentum_X = "momentum_X";
    static const string  momentum_Y = "momentum_Y";
    static const string  momentum_Z = "momentum_Z";

    static const string  positon_mean = "positon_mean";
    static const string  position_sigma = "position_sigma";
}
  

class ParticleGunPrimaryGenerator : public G4VUserPrimaryGeneratorAction
{
  public:

    /** Constructor
     *@param detectorParamsFile - file (full path) containing the detector parameters
     *@param g4gunParamsFile - configuration of g4gun
     **/
    ParticleGunPrimaryGenerator(G4String detectorParamsFile, G4String g4gunParamsFile);

    /** Destructor **/
    virtual ~ParticleGunPrimaryGenerator();

    /** method from the base class **/
    virtual void GeneratePrimaries(G4Event*);  

     /** Sets the kinematics and limitation to be used in the g4 gun **/
    void SetKinematics();

    /** Returns number of events to generate **/
    int GetNumberOfEvents(){return fevents;}

    /** Returns true if there are more events to read, false otherwise
     * counter is incremented on each call to ReadEvent **/
    bool HasMoreEvents() const {return (fnextEventNo < fevents);}
  
  private:

    /** Current detector parameters **/
    data::superfgd::detector::FgdDetectorParameters fdetector;

    /** Utility class to read the gun configuration from a file **/
    tools::ParamReader fparamReader;

    /** Geant particle gun**/
    std::shared_ptr<G4ParticleGun> fparticleGun;  

    /** Total number of events **/
    int fevents;

    /** Counter for the next event **/
    int fnextEventNo;

    /** Mean value for Gaussian distribution **/
    G4double fmean;
    /** Sigma for Gaussian distribution **/
    G4double fsigma;

    /** Detector lenght in X dimention **/
    G4double ftotal_X;

    /** Detector lenght in Y dimention **/
    G4double ftotal_Y;

    /** Detector lenght in Z dimention **/
    G4double ftotal_Z;
};

}   //superfgd
}   //simulators
}   //esbroot

#endif