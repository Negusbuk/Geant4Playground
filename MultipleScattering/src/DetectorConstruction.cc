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
// $Id: DetectorConstruction.cc 77656 2013-11-27 08:52:57Z gcosmo $
//
/// \file DetectorConstruction.cc
/// \brief Implementation of the DetectorConstruction class

#include "DetectorConstruction.hh"
#include "MagneticField.hh"

#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"
#include "G4Mag_UsualEqRhs.hh"
#include "G4AutoDelete.hh"

#include "G4Material.hh"
#include "G4Element.hh"
#include "G4MaterialTable.hh"
#include "G4NistManager.hh"

#include "G4VSolid.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVParameterised.hh"
#include "G4PVReplica.hh"
#include "G4UserLimits.hh"

#include "G4SDManager.hh"
#include "G4VSensitiveDetector.hh"
#include "G4RunManager.hh"
#include "G4GenericMessenger.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4ios.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ThreadLocal MagneticField* DetectorConstruction::fMagneticField = 0;
G4ThreadLocal G4FieldManager* DetectorConstruction::fFieldMgr = 0;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
: G4VUserDetectorConstruction(), 
  fMessenger(0),
  fAbsorberLogical(0),
  fVisAttributes()
{
  DefineCommands();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{
  delete fMessenger;

  for (G4int i=0; i<G4int(fVisAttributes.size()); ++i)
  {
    delete fVisAttributes[i];
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  // Construct materials
  ConstructMaterials();

  Galactic = G4Material::GetMaterial("G4_Galactic");
  Air = G4Material::GetMaterial("G4_AIR");
  CFRP = G4Material::GetMaterial("G4_GRAPHITE");
  Si = G4Material::GetMaterial("G4_Si");
  Al = G4Material::GetMaterial("G4_Al");
  Steel = G4Material::GetMaterial("G4_STAINLESS-STEEL");
  Brass = G4Material::GetMaterial("G4_BRASS");
  PbWO4 = G4Material::GetMaterial("G4_PbWO4");
  Pb = G4Material::GetMaterial("G4_Pb");
  Uranium = G4Material::GetMaterial("G4_U");

  G4Material * theMaterial = Galactic;

  // Option to switch on/off checking of volumes overlaps
  //
  G4bool checkOverlaps = true;

  // geometries --------------------------------------------------------------
  // experimental hall (world volume)
  G4VSolid* worldSolid = new G4Box("worldBox", 10.*m, 10.*m, 10.*m);
  G4LogicalVolume* worldLogical = new G4LogicalVolume(worldSolid, Galactic, "worldLogical");
  G4VPhysicalVolume* worldPhysical = new G4PVPlacement(0, G4ThreeVector(),
                                                       worldLogical, "worldPhysical",
                                                       0, false, 0, checkOverlaps);

  // Absorber
  G4VSolid* AbsorberSolid = new G4Box("AbsorberBox", 9.0*m, 9.0*m, 5.0*cm);
  fAbsorberLogical = new G4LogicalVolume(AbsorberSolid, theMaterial, "AbsorberLogical");
  fAbsorberPhysical = new G4PVPlacement(0, G4ThreeVector(0.0*m, 0.0*m, 15.0*cm),
                                        fAbsorberLogical, "AbsorberPhysical",
                                        worldLogical, false, 0, false);

  G4UserLimits* userWorldLimits = new G4UserLimits(100.0*um);
  worldLogical->SetUserLimits(userWorldLimits);

  G4UserLimits* userAbsorberLimits = new G4UserLimits(10.0*um);
  fAbsorberLogical->SetUserLimits(userAbsorberLimits);

  // visualization attributes ------------------------------------------------

  G4VisAttributes* visAttributes = new G4VisAttributes(G4Colour(1.0,1.0,1.0));
  visAttributes->SetVisibility(false);
  worldLogical->SetVisAttributes(visAttributes);
  fVisAttributes.push_back(visAttributes);

  visAttributes = new G4VisAttributes(G4Colour(0.0, 0.0, 0.9));
  visAttributes->SetVisibility(false);
  fAbsorberLogical->SetVisAttributes(visAttributes);
  fVisAttributes.push_back(visAttributes);

  // return the world physical volume ----------------------------------------

  return worldPhysical;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::ConstructSDandField()
{
  // magnetic field ----------------------------------------------------------

  fMagneticField = new MagneticField();
  fFieldMgr = new G4FieldManager();
  fFieldMgr->SetDetectorField(fMagneticField);
  fFieldMgr->CreateChordFinder(fMagneticField);
  G4bool forceToAllDaughters = true;
  fAbsorberLogical->SetFieldManager(fFieldMgr, forceToAllDaughters);

  // Register the field and its manager for deleting
  G4AutoDelete::Register(fMagneticField);
  G4AutoDelete::Register(fFieldMgr);
}    

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::ConstructMaterials()
{
  G4NistManager* nistManager = G4NistManager::Instance();

  // nistManager->ListMaterials("all");

  nistManager->FindOrBuildMaterial("G4_Galactic");
  nistManager->FindOrBuildMaterial("G4_AIR");
  nistManager->FindOrBuildMaterial("G4_GRAPHITE");
  nistManager->FindOrBuildMaterial("G4_Si");
  nistManager->FindOrBuildMaterial("G4_Al");
  nistManager->FindOrBuildMaterial("G4_STAINLESS-STEEL");
  nistManager->FindOrBuildMaterial("G4_BRASS");
  nistManager->FindOrBuildMaterial("G4_PbWO4");
  nistManager->FindOrBuildMaterial("G4_Pb");
  nistManager->FindOrBuildMaterial("G4_U");

  G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::DefineCommands()
{
  // Define //detector command directory using generic messenger class
  fMessenger = new G4GenericMessenger(this,
                                      "/MSC/detector/",
                                      "Detector control");

  fMessenger->DeclareMethod("list",
                            &DetectorConstruction::ListMaterials,
                            "List known Materials.");

  G4GenericMessenger::Command& materialCmd = fMessenger->DeclareMethod("material",
                                                                    &DetectorConstruction::SetAbsorberMaterial,
                                                                    "Set Material of the Absorber.");
  materialCmd.SetParameterName("material", true);
  materialCmd.SetDefaultValue("G4_Galactic");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::ListMaterials()
{
  // get the pointer to the material table
  const G4MaterialTable* theMaterialTable = G4Material::GetMaterialTable();

  // search the material by its name
  G4Material* material;
  for (size_t j=0 ; j<theMaterialTable->size() ; j++) {
    material = (*theMaterialTable)[j];

    G4cout << material->GetName() << G4endl;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetAbsorberMaterial(G4String materialChoice)
{
  // get the pointer to the material table
  const G4MaterialTable* theMaterialTable = G4Material::GetMaterialTable();

  // search the material by its name
  G4Material* material;
  for (size_t j=0 ; j<theMaterialTable->size() ; j++) {
    material = (*theMaterialTable)[j];

    if(material->GetName() == materialChoice) {
      fAbsorberLogical->SetMaterial(material);
      G4RunManager::GetRunManager()->PhysicsHasBeenModified();
      G4cout << "Material of absorber set to " << material->GetName() << G4endl;
    }
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
