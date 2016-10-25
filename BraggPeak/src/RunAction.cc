#include "RunAction.hh"
#include "EventAction.hh"
#include "Analysis.hh"

#include "G4Run.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

RunAction::RunAction(EventAction* eventAction)
 : G4UserRunAction(),
   fEventAction(eventAction)
{ 
  // Create analysis manager
  // The choice of analysis technology is done via selectin of a namespace
  // in Analysis.hh
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  G4cout << "Using " << analysisManager->GetType() << G4endl;

  // Default settings
  analysisManager->SetVerboseLevel(1);
  analysisManager->SetFileName("");

  analysisManager->CreateP1("dE vs Z","dE vs Z",            // p1 Id = 0
                            300, 0, 300);

  if (fEventAction) {
    analysisManager->CreateNtuple("steps", "Steps");
    analysisManager->CreateNtupleIColumn("ParentID"); // column Id = 0
    analysisManager->CreateNtupleIColumn("TrackID");  // column Id = 1
    analysisManager->CreateNtupleIColumn("StepID");   // column Id = 2
    analysisManager->CreateNtupleDColumn("x");        // column Id = 3
    analysisManager->CreateNtupleDColumn("y");        // column Id = 4
    analysisManager->CreateNtupleDColumn("z");        // column Id = 5
    analysisManager->CreateNtupleDColumn("p");        // column Id = 6
    analysisManager->CreateNtupleDColumn("dE");       // column Id = 7
    analysisManager->CreateNtupleDColumn("l");        // column Id = 8
    analysisManager->CreateNtupleIColumn("state");    // column Id = 9
    analysisManager->FinishNtuple();
  }
}

RunAction::~RunAction()
{
  delete G4AnalysisManager::Instance();  
}

void RunAction::BeginOfRunAction(const G4Run* /*run*/)
{ 
  //inform the runManager to save random number seed
  //G4RunManager::GetRunManager()->SetRandomNumberStore(true);
  
  // Get analysis manager
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

  // Open an output file 
  // The default file name is set in RunAction::RunAction(),
  // it can be overwritten in a macro
  analysisManager->OpenFile();
}

void RunAction::EndOfRunAction(const G4Run* /*run*/)
{
  // save histograms & ntuple
  //
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

  analysisManager->Write();
  analysisManager->CloseFile();
}
