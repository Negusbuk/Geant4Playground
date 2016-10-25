#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"

class G4ParticleGun;
class G4GenericMessenger;
class G4Event;
class G4ParticleDefinition;

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
  PrimaryGeneratorAction();
  virtual ~PrimaryGeneratorAction();

  virtual void GeneratePrimaries(G4Event*);

  void SetMomentum(G4double val) { fMomentum = val; }
  G4double GetMomentum() const { return fMomentum; }

private:
  void DefineCommands();

  G4ParticleGun* fParticleGun;
  G4GenericMessenger* fMessenger;
  G4ParticleDefinition* fPositron;
  G4ParticleDefinition* fMuon;
  G4ParticleDefinition* fPion;
  G4ParticleDefinition* fKaon;
  G4ParticleDefinition* fProton;
  G4ParticleDefinition* fNeutron;
  G4double fMomentum;
};

#endif
