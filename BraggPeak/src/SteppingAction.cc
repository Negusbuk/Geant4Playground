#include "SteppingAction.hh"
#include "EventAction.hh"
#include "DetectorConstruction.hh"
#include "Analysis.hh"

#include "G4Step.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4GenericMessenger.hh"

SteppingAction::SteppingAction(const DetectorConstruction* detectorConstruction,
                                           EventAction* eventAction)
  : G4UserSteppingAction(),
    fFillNTuple(true),
    fDetConstruction(detectorConstruction),
    fEventAction(eventAction)
{
  DefineCommands();
}

SteppingAction::~SteppingAction()
{ 

}

void SteppingAction::UserSteppingAction(const G4Step* step)
{
  //if (step->GetTrack()->GetDefinition()->GetPDGCharge() == 0.) return;

  G4double edep = step->GetTotalEnergyDeposit()/keV;
  if (edep <= 0.) return;

  if (step->GetPreStepPoint()->GetPosition().z()>30.0*cm) return;
  if (step->GetStepLength()>1.0*mm) return;

  double preStepPos = step->GetPreStepPoint()->GetPosition().z();

  // Get analysis manager
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

  if (step->GetTrack()->GetParentID()!=0) return;

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

  if (fFillNTuple) {
    analysisManager->FillNtupleIColumn(0, step->GetTrack()->GetParentID());
    analysisManager->FillNtupleIColumn(1, step->GetTrack()->GetTrackID());
    analysisManager->FillNtupleIColumn(2, step->GetTrack()->GetCurrentStepNumber());
    analysisManager->FillNtupleDColumn(3, step->GetPreStepPoint()->GetPosition().x()/mm);
    analysisManager->FillNtupleDColumn(4, step->GetPreStepPoint()->GetPosition().y()/mm);
    analysisManager->FillNtupleDColumn(5, step->GetPreStepPoint()->GetPosition().z()/mm);
    analysisManager->FillNtupleDColumn(6, step->GetTrack()->GetMomentum().mag()/GeV);
    analysisManager->FillNtupleDColumn(7, edep);
    analysisManager->FillNtupleDColumn(8, step->GetStepLength()/mm);
    analysisManager->FillNtupleIColumn(9, state);
    analysisManager->AddNtupleRow();
  }
}

void SteppingAction::DefineCommands()
{
  // Define //detector command directory using generic messenger class
  fMessenger = new G4GenericMessenger(this,
                                      "/Negusbuk/stepping/",
                                      "Stepping control");

  G4GenericMessenger::Command& ntupleCmd = fMessenger->DeclareProperty("ntuple", fFillNTuple);
  ntupleCmd.SetParameterName("flg", true);
  ntupleCmd.SetDefaultValue("false");
}
