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
// $Id: MSCActionInitialization.cc 68058 2013-03-13 14:47:43Z gcosmo $
//
/// \file MSCActionInitialization.cc
/// \brief Implementation of the MSCActionInitialization class

#include "MSCActionInitialization.hh"
#include "MSCPrimaryGeneratorAction.hh"
#include "MSCRunAction.hh"
#include "MSCEventAction.hh"
#include "MSCSteppingAction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MSCActionInitialization::MSCActionInitialization(const MSCDetectorConstruction* detConstruction)
 : G4VUserActionInitialization(),
   fDetConstruction(detConstruction)
{

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MSCActionInitialization::~MSCActionInitialization()
{

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MSCActionInitialization::BuildForMaster() const
{
  MSCEventAction* eventAction = 0;
  SetUserAction(new MSCRunAction(eventAction));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MSCActionInitialization::Build() const
{
  MSCEventAction* eventAction = new MSCEventAction;

  SetUserAction(new MSCPrimaryGeneratorAction);
  SetUserAction(new MSCRunAction(eventAction));
  SetUserAction(eventAction);
  SetUserAction(new MSCSteppingAction(fDetConstruction));
}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
