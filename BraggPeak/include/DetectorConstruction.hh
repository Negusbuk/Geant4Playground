#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4RotationMatrix.hh"
#include "G4FieldManager.hh"
#include "G4UIcmdWithAString.hh"

#include <vector>

class MagneticField;

class G4VPhysicalVolume;
class G4Material;
class G4VSensitiveDetector;
class G4VisAttributes;
class G4GenericMessenger;

class DetectorConstruction : public G4VUserDetectorConstruction
{
public:
  DetectorConstruction();
  virtual ~DetectorConstruction();

  virtual G4VPhysicalVolume* Construct();
  virtual void ConstructSDandField();

  void ListMaterials();
  void SetAbsorberMaterial(G4String materialChoice);

  G4VPhysicalVolume* GetAbsorberPV() const { return fAbsorberPhysical; }

  void ConstructMaterials();

private:

  void DefineCommands();

  G4GenericMessenger* fMessenger;

  static G4ThreadLocal MagneticField* fMagneticField;
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

#endif
