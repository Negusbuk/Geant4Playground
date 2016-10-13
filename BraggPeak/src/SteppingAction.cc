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
// $Id: SteppingAction.cc 68058 2013-03-13 14:47:43Z gcosmo $
// 
/// \file SteppingAction.cc
/// \brief Implementation of the SteppingAction class

#include "SteppingAction.hh"
#include "EventAction.hh"
#include "DetectorConstruction.hh"
#include "Analysis.hh"

#include "G4Step.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4GenericMessenger.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::SteppingAction(const DetectorConstruction* detectorConstruction,
                                           EventAction* eventAction)
  : G4UserSteppingAction(),
    fFillNTuple(true),
    fDetConstruction(detectorConstruction),
    fEventAction(eventAction)
{
  DefineCommands();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::~SteppingAction()
{ 

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingAction::UserSteppingAction(const G4Step* step)
{
  //if (step->GetTrack()->GetDefinition()->GetPDGCharge() == 0.) return;

  G4double edep = step->GetTotalEnergyDeposit()/keV;
  if (edep <= 0.) return;

  if (step->GetPreStepPoint()->GetPosition().z()>30.0*cm) return;
  if (step->GetPreStepPoint()->GetPosition().z()<5.0*cm) return;
  if (step->GetStepLength()>1.0*mm) return;

  // Get analysis manager
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

  fEventAction->AddDE(step->GetPreStepPoint()->GetPosition().z()/mm, edep);

  if (step->GetTrack()->GetParentID()!=0) return;

  if (fFillNTuple) {
    analysisManager->FillNtupleDColumn(0, step->GetTrack()->GetParentID());
    analysisManager->FillNtupleDColumn(1, step->GetTrack()->GetTrackID());
    analysisManager->FillNtupleDColumn(2, step->GetPreStepPoint()->GetPosition().x()/mm);
    analysisManager->FillNtupleDColumn(3, step->GetPreStepPoint()->GetPosition().y()/mm);
    analysisManager->FillNtupleDColumn(4, step->GetPreStepPoint()->GetPosition().z()/mm);
    analysisManager->FillNtupleDColumn(5, step->GetTrack()->GetMomentum().mag()/GeV);
    analysisManager->FillNtupleDColumn(6, edep);
    analysisManager->FillNtupleDColumn(7, step->GetStepLength()/mm);
    analysisManager->AddNtupleRow();
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingAction::DefineCommands()
{
  // Define //detector command directory using generic messenger class
  fMessenger = new G4GenericMessenger(this,
                                      "//stepping/",
                                      "Stepping control");

  G4GenericMessenger::Command& ntupleCmd = fMessenger->DeclareProperty("ntuple", fFillNTuple);
  ntupleCmd.SetParameterName("flg", true);
  ntupleCmd.SetDefaultValue("false");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
