#include "PrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4GenericMessenger.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction()
: G4VUserPrimaryGeneratorAction(),     
  fParticleGun(0), fMessenger(0), 
  fPositron(0), fMuon(0), fPion(0), fKaon(0), fProton(0),
  fMomentum(1.000*GeV)
{
  G4int n_particle = 1;
  fParticleGun  = new G4ParticleGun(n_particle);

  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;
  fPositron = particleTable->FindParticle(particleName="e+");
  fMuon = particleTable->FindParticle(particleName="mu+");
  fPion = particleTable->FindParticle(particleName="pi+");
  fKaon = particleTable->FindParticle(particleName="kaon+");
  fProton = particleTable->FindParticle(particleName="proton");
  fNeutron = particleTable->FindParticle(particleName="neutron");

  // default particle kinematics
  fParticleGun->SetParticlePosition(G4ThreeVector(0.*m,0.*m,0.*m));
  fParticleGun->SetParticleDefinition(fProton);

  // define commands for this class
  DefineCommands();
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete fParticleGun;
  delete fMessenger;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* event)
{
  G4ParticleDefinition* particle = fProton;
  fParticleGun->SetParticleDefinition(particle);

  G4double pp = fMomentum;
  G4double mass = particle->GetPDGMass();
  G4double Ekin = std::sqrt(pp*pp+mass*mass)-mass;
  fParticleGun->SetParticleEnergy(Ekin);

  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0., 0., 1.));

  fParticleGun->GeneratePrimaryVertex(event);
}

void PrimaryGeneratorAction::DefineCommands()
{
  // Define //generator command directory using generic messenger class
  fMessenger = new G4GenericMessenger(this,
                                      "/Negusbuk/generator/",
                                      "Primary generator control");

  // momentum command
  G4GenericMessenger::Command& momentumCmd = fMessenger->DeclarePropertyWithUnit("momentum",
                                                                                 "GeV",
                                                                                 fMomentum,
                                                                                 "Mean momentum of primaries.");
  momentumCmd.SetParameterName("p", true);
  momentumCmd.SetRange("p>=0.");
  momentumCmd.SetDefaultValue("1.");
}
