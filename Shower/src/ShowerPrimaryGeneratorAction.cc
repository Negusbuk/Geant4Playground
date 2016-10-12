//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// $Id: ShowerPrimaryGeneratorAction.cc 77781 2013-11-28 07:54:07Z gcosmo $
//
/// \file ShowerPrimaryGeneratorAction.cc
/// \brief Implementation of the ShowerPrimaryGeneratorAction class

#include "ShowerPrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4GenericMessenger.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ShowerPrimaryGeneratorAction::ShowerPrimaryGeneratorAction()
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

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ShowerPrimaryGeneratorAction::~ShowerPrimaryGeneratorAction()
{
  delete fParticleGun;
  delete fMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ShowerPrimaryGeneratorAction::GeneratePrimaries(G4Event* event)
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

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ShowerPrimaryGeneratorAction::DefineCommands()
{
  // Define /Shower/generator command directory using generic messenger class
  fMessenger = new G4GenericMessenger(this,
                                      "/Shower/generator/",
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

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
