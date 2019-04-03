#include "Simulation/include/NFParticleGunPrimaryGenerator.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "Randomize.hh"


namespace esbroot {
namespace simulators {
namespace superfgd {

ParticleGunPrimaryGenerator::ParticleGunPrimaryGenerator(G4String detectorParamsFile, G4String g4gunParamsFile)
    : G4VUserPrimaryGeneratorAction()
        , fsigma(0.)
        , fmean(0.)
{
    fdetector.loadPartParams(detectorParamsFile);
    fparamReader.loadParams(g4gunParamsFile);

    fparticleGun  = make_shared<G4ParticleGun>();
}

ParticleGunPrimaryGenerator::~ParticleGunPrimaryGenerator()
{
}


void ParticleGunPrimaryGenerator::SetKinematics()
{
    double step_X  = fdetector.ParamAsDouble(data::superfgd::detector::DP::length_X) * fdetector.GetLenghtUnit();
    double step_Y  = fdetector.ParamAsDouble(data::superfgd::detector::DP::length_Y) * fdetector.GetLenghtUnit();
    double step_Z  = fdetector.ParamAsDouble(data::superfgd::detector::DP::length_Z) * fdetector.GetLenghtUnit();

    ftotal_X = step_X * fdetector.ParamAsInt(data::superfgd::detector::DP::number_cubes_X);
    ftotal_Y = step_Y * fdetector.ParamAsInt(data::superfgd::detector::DP::number_cubes_Y);
    ftotal_Z = step_Z * fdetector.ParamAsInt(data::superfgd::detector::DP::number_cubes_Z);

    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    G4ParticleDefinition* particle = particleTable->FindParticle(fparamReader.ParamAsInt(PG::particlePdg));
    fparticleGun->SetParticleDefinition(particle);

    G4ThreeVector momentum(fparamReader.ParamAsDouble(PG::momentum_X),
                            fparamReader.ParamAsDouble(PG::momentum_Y),
                            fparamReader.ParamAsDouble(PG::momentum_Z));
    fparticleGun->SetParticleMomentumDirection(momentum);

    double x = ftotal_X * fparamReader.ParamAsDouble(PG::positon_X);
    double y = ftotal_Y * fparamReader.ParamAsDouble(PG::positon_Y);
    double z = ftotal_Z * fparamReader.ParamAsDouble(PG::positon_Z);

    G4ThreeVector position(x,y,z);
    fparticleGun->SetParticlePosition(position);

    G4double e = fparamReader.ParamAsDouble(PG::particleEnergy) * CLHEP::GeV;
    fparticleGun->SetParticleEnergy(e);

    fmean = fparamReader.ParamAsDouble(PG::positon_mean);
    fsigma = fparamReader.ParamAsDouble(PG::position_sigma);
    fevents = fparamReader.ParamAsInt(PG::events);
}

void ParticleGunPrimaryGenerator::GeneratePrimaries(G4Event* anEvent)
{
    fnextEventNo++;
    G4ThreeVector position = fparticleGun->GetParticlePosition(); 

    G4double x0 = position.x() + (G4RandGauss::shoot(fmean, fsigma))*ftotal_X/2;
    G4double y0 = position.y() + (G4RandGauss::shoot(fmean, fsigma))*ftotal_Y/2;
    G4double z0 = position.z(); // start at center

    fparticleGun->SetParticlePosition(G4ThreeVector(x0,y0,z0));
    fparticleGun->GeneratePrimaryVertex(anEvent);
}

}   //superfgd
}   //simulators
}   //esbroot
