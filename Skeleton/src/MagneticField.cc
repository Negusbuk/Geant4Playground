#include "MagneticField.hh"

#include "G4GenericMessenger.hh"
#include "G4SystemOfUnits.hh"
#include "globals.hh"

MagneticField::MagneticField()
: G4MagneticField(),
  fMessenger(0),
  fBy(1.0*tesla)
{
  DefineCommands();
}

MagneticField::~MagneticField()
{ 
  delete fMessenger;
}

void MagneticField::GetFieldValue(const G4double /* point */ [4], double *bField) const
{
  /*
  G4double x = point[0];
  G4double y = point[1];
  G4double z = point[2];
  G4double t = point[3];
  */

  bField[0] = 0.;
  bField[1] = fBy;
  bField[2] = 0.;
}

void MagneticField::DefineCommands()
{
  fMessenger = new G4GenericMessenger(this,
                                      "/Negusbuk/field/",
                                      "Field control");

  // fieldValue command
  G4GenericMessenger::Command& valueCmd = fMessenger->DeclareMethodWithUnit("value","tesla",
                                                                            &MagneticField::SetField,
                                                                            "Set field strength.");
  valueCmd.SetParameterName("field", true);
  valueCmd.SetDefaultValue("1.");
}
