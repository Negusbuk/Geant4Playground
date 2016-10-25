#include "MagneticField.hh"

#include "G4GenericMessenger.hh"
#include "G4SystemOfUnits.hh"
#include "globals.hh"

MagneticField::MagneticField()
: G4MagneticField(), fMessenger(0), fBy(1.0*tesla)
{
    // define commands for this class
    DefineCommands();
}

MagneticField::~MagneticField()
{ 
    delete fMessenger; 
}

void MagneticField::GetFieldValue(const G4double [4],double *bField) const
{
    bField[0] = 0.;
    bField[1] = fBy;
    bField[2] = 0.;
}

void MagneticField::DefineCommands()
{
    // Define //field command directory using generic messenger class
    fMessenger = new G4GenericMessenger(this, 
                                        "/Negusbuk/field/",
                                        "Field control");

    // fieldValue command 
    G4GenericMessenger::Command& valueCmd
      = fMessenger->DeclareMethodWithUnit("value","tesla",
                                  &MagneticField::SetField,
                                  "Set field strength.");
    valueCmd.SetParameterName("field", true);
    valueCmd.SetDefaultValue("1.");
}
