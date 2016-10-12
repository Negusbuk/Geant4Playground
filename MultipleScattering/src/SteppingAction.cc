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

SteppingAction::SteppingAction(const DetectorConstruction* detectorConstruction)
  : G4UserSteppingAction(),
    fDetConstruction(detectorConstruction)
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

  if (step->GetTrack()->GetParentID()!=0) return;

  if (step->GetPreStepPoint()->GetPosition().z()>40.*cm) return;

  double preStepPos = step->GetPreStepPoint()->GetPosition().z();

  int state = 0;
  if (preStepPos<10.0*cm) {
    state = 0;
  } else if (preStepPos>=10.0*cm && preStepPos<20.0*cm) {
    state = 1;
  } else if (preStepPos==20.0*cm) {
    state = 2;
  } else {
    state = 3;
  }

  // Get analysis manager
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

  if (state==2) {
    analysisManager->FillH1(0,
                            step->GetPreStepPoint()->GetPosition().x()/mm);
    analysisManager->FillH1(1,
                            step->GetPreStepPoint()->GetPosition().y()/mm);

    analysisManager->FillH2(0,
                            step->GetPreStepPoint()->GetPosition().x()/mm,
                            step->GetPreStepPoint()->GetPosition().y()/mm);
  }

  if (fFillNTuple) {
    analysisManager->FillNtupleIColumn(0, step->GetTrack()->GetParentID());
    analysisManager->FillNtupleIColumn(1, step->GetTrack()->GetTrackID());
    analysisManager->FillNtupleIColumn(2, step->GetTrack()->GetCurrentStepNumber());
    analysisManager->FillNtupleDColumn(3, step->GetPreStepPoint()->GetPosition().x()/mm);
    analysisManager->FillNtupleDColumn(4, step->GetPreStepPoint()->GetPosition().y()/mm);
    analysisManager->FillNtupleDColumn(5, step->GetPreStepPoint()->GetPosition().z()/mm);
    analysisManager->FillNtupleDColumn(6, step->GetTrack()->GetMomentum().mag()/GeV);
    analysisManager->FillNtupleDColumn(7, step->GetTotalEnergyDeposit()/keV);
    analysisManager->FillNtupleDColumn(8, step->GetStepLength()/mm);
    analysisManager->FillNtupleIColumn(9, state);
    analysisManager->AddNtupleRow();
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingAction::DefineCommands()
{
  // Define //detector command directory using generic messenger class
  fMessenger = new G4GenericMessenger(this,
                                      "/MSC/stepping/",
                                      "Stepping control");

  G4GenericMessenger::Command& ntupleCmd = fMessenger->DeclareProperty("ntuple", fFillNTuple);
  ntupleCmd.SetParameterName("flg", true);
  ntupleCmd.SetDefaultValue("false");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
