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
// $Id: MSCRunAction.cc 74204 2013-10-01 07:04:43Z ihrivnac $
//
/// \file MSCRunAction.cc
/// \brief Implementation of the MSCRunAction class

#include "MSCRunAction.hh"
#include "MSCEventAction.hh"
#include "MSCAnalysis.hh"

#include "G4Run.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MSCRunAction::MSCRunAction(MSCEventAction* eventAction)
 : G4UserRunAction(),
   fEventAction(eventAction)
{ 
  // Create analysis manager
  // The choice of analysis technology is done via selectin of a namespace
  // in MSCAnalysis.hh
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  G4cout << "Using " << analysisManager->GetType() << G4endl;

  // Default settings
  analysisManager->SetVerboseLevel(1);
  analysisManager->SetFileName("MSC");

  analysisManager->CreateH1("Exit X","Exit X",           // h1 Id = 0
                            250, -25., 25);
  analysisManager->CreateH1("Exit Y","Exit Y",           // h1 Id = 1
                            250, -25., 25);


  analysisManager->CreateH2("Exit XY","Exit X vs. Y",           // h2 Id = 0
                            250, -25., 25, 250, -25., 25.);

  if (fEventAction) {
    analysisManager->CreateNtuple("steps", "Steps");
    analysisManager->CreateNtupleDColumn("ParentID"); // column Id = 0
    analysisManager->CreateNtupleDColumn("TrackID");  // column Id = 1
    analysisManager->CreateNtupleDColumn("x");        // column Id = 2
    analysisManager->CreateNtupleDColumn("y");        // column Id = 3
    analysisManager->CreateNtupleDColumn("z");        // column Id = 4
    analysisManager->CreateNtupleDColumn("p");        // column Id = 5
    analysisManager->CreateNtupleDColumn("dE");       // column Id = 6
    analysisManager->CreateNtupleDColumn("l");        // column Id = 7
    analysisManager->CreateNtupleIColumn("state");    // column Id = 8
    analysisManager->FinishNtuple();
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MSCRunAction::~MSCRunAction()
{
  delete G4AnalysisManager::Instance();  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MSCRunAction::BeginOfRunAction(const G4Run* /*run*/)
{ 
  //inform the runManager to save random number seed
  //G4RunManager::GetRunManager()->SetRandomNumberStore(true);
  
  // Get analysis manager
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

  // Open an output file 
  // The default file name is set in MSCRunAction::MSCRunAction(),
  // it can be overwritten in a macro
  analysisManager->OpenFile();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MSCRunAction::EndOfRunAction(const G4Run* /*run*/)
{
  // save histograms & ntuple
  //
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->Write();
  analysisManager->CloseFile();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
