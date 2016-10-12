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
// $Id: ShowerActionInitialization.cc 68058 2013-03-13 14:47:43Z gcosmo $
//
/// \file ShowerActionInitialization.cc
/// \brief Implementation of the ShowerActionInitialization class

#include "ShowerActionInitialization.hh"
#include "ShowerPrimaryGeneratorAction.hh"
#include "ShowerRunAction.hh"
#include "ShowerEventAction.hh"
#include "StackingAction.hh"
#include "TrackingAction.hh"
#include "ShowerSteppingAction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ShowerActionInitialization::ShowerActionInitialization(const ShowerDetectorConstruction* detConstruction)
 : G4VUserActionInitialization(),
   fDetConstruction(detConstruction)
{

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ShowerActionInitialization::~ShowerActionInitialization()
{

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ShowerActionInitialization::BuildForMaster() const
{
  ShowerEventAction* eventAction = 0;
  SetUserAction(new ShowerRunAction(eventAction));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ShowerActionInitialization::Build() const
{
  ShowerEventAction* eventAction = new ShowerEventAction;

  SetUserAction(new ShowerPrimaryGeneratorAction);
  SetUserAction(new ShowerRunAction(eventAction));
  SetUserAction(eventAction);
  SetUserAction(new TrackingAction);
  SetUserAction(new StackingAction);
  SetUserAction(new ShowerSteppingAction(fDetConstruction, eventAction));
}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
