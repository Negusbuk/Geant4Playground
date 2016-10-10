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
// $Id: MSCDetectorConstruction.hh 76474 2013-11-11 10:36:34Z gcosmo $
//
/// \file MSCDetectorConstruction.hh
/// \brief Definition of the MSCDetectorConstruction class

#ifndef MSCDetectorConstruction_h
#define MSCDetectorConstruction_h 1

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4RotationMatrix.hh"
#include "G4FieldManager.hh"
#include "G4UIcmdWithAString.hh"

#include <vector>

class MSCMagneticField;

class G4VPhysicalVolume;
class G4Material;
class G4VSensitiveDetector;
class G4VisAttributes;
class G4GenericMessenger;

/// Detector construction

class MSCDetectorConstruction : public G4VUserDetectorConstruction
{
public:
  MSCDetectorConstruction();
  virtual ~MSCDetectorConstruction();

  virtual G4VPhysicalVolume* Construct();
  virtual void ConstructSDandField();

  void ListMaterials();
  void SetAbsorberMaterial(G4String materialChoice);

  G4VPhysicalVolume* GetAbsorberPV() const { return fAbsorberPhysical; }

  void ConstructMaterials();

private:

  void DefineCommands();

  G4GenericMessenger* fMessenger;

  static G4ThreadLocal MSCMagneticField* fMagneticField;
  static G4ThreadLocal G4FieldManager* fFieldMgr;

  G4LogicalVolume* fAbsorberLogical;
  G4VPhysicalVolume* fAbsorberPhysical;

  std::vector<G4VisAttributes*> fVisAttributes;

  G4Material* Galactic;
  G4Material* Air;
  G4Material* CFRP;
  G4Material* Si;
  G4Material* Al;
  G4Material* Steel;
  G4Material* Brass;
  G4Material* PbWO4;
  G4Material* Pb;
  G4Material* Uranium;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
