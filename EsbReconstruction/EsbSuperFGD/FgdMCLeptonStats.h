#ifndef ESBROOT_ESBDRECONSTRUCTION_FGD_MC_LEPTON_STATS_H
#define ESBROOT_ESBDRECONSTRUCTION_FGD_MC_LEPTON_STATS_H

// EsbRoot headers
#include "EsbReconstruction/EsbSuperFGD/FgdMCGenFitRecon.h"
#include "EsbReconstruction/EsbSuperFGD/FgdMCEventRecord.h"

namespace esbroot {
namespace reconstruction {
namespace superfgd {

class FgdMCLeptonStats : public FgdMCGenFitRecon
{

 public:

  /** Default constructor **/  
  FgdMCLeptonStats();

  /** Constructor with argument
   *@param name       Name of task
   *@param geoConfigFile  - Configuration file detector
   *@param mediaFile  - Configuration file for the used mediums
   *@param eventData  - events data file (generated from fgd generator)
   *@param verbose  - Verbosity level
   *@param debugLlv - debug level for genfit
   *@param visualize -  to visualize the event using genfit::EventDisplay
   *@param visOption -  option to be passed to genfit::EventDisplay
  **/  
  FgdMCLeptonStats(const char* name
              , const char* geoConfigFile
              , const char* mediaFile
              , const char* eventData
              , Int_t verbose = 1
              , double debugLlv = 0
              , bool visualize = false
              , std::string visOption ="D");

  /** Destructor **/
  virtual ~FgdMCLeptonStats();

  /** Virtual method Init **/
  virtual InitStatus Init() override;

  /** Virtual method Exec **/
  virtual void Exec(Option_t* opt) override;

protected:

  std::string feventData;//!<!

  std::vector<FgdMCEventRecord> feventRecords;//!<!
  	   
  ClassDef(FgdMCLeptonStats, 2);

};

} //superfgd
} //reconstruction
} //esbroot

#endif // ESBROOT_ESBDRECONSTRUCTION_FGD_MC_LEPTON_STATS_H
