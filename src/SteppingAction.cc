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
//

#include "SteppingAction.hh"
#include "EventAction.hh"
#include "DetectorConstruction.hh"

#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::SteppingAction(EventAction* eventAction)
: fEventAction(eventAction)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingAction::UserSteppingAction(const G4Step* step)
{

  // get volume of the current step
  G4LogicalVolume* volume
    = step->GetPreStepPoint()->GetTouchableHandle()
    ->GetVolume()->GetLogicalVolume();

  G4Track* track = step->GetTrack();
  G4ParticleDefinition* particle = track->GetDefinition();
  G4String particleName = particle->GetParticleName();
  
  G4int pid = track->GetDynamicParticle()->GetParticleDefinition()->GetPDGEncoding();
  G4double edep = step->GetTotalEnergyDeposit();///CLHEP::keV;//For _HP library
  G4double kine = track->GetKineticEnergy();///CLHEP::keV;

  G4ThreeVector pos = track->GetPosition();
  G4double gtime   = track->GetGlobalTime();

  G4double trackid = track->GetTrackID();
  G4double parenid = track->GetParentID();
  
  G4StepStatus PreStepStatus = step->GetPreStepPoint()->GetStepStatus();
  G4StepStatus PostStepStatus = step->GetPostStepPoint()->GetStepStatus();

  auto prevol   = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume();
  auto postvol = step->GetPostStepPoint()->GetTouchableHandle()->GetVolume();
  G4LogicalVolume* PreStepPoint = NULL;
  G4LogicalVolume* PostStepPoint = NULL;
  if(prevol) PreStepPoint = prevol->GetLogicalVolume();
  if(postvol) PostStepPoint = postvol->GetLogicalVolume();
  G4String preName = "";
  G4String postName = "";
  G4String prepvname = ""; // physical volume name
  G4String postpvname = "";
  if(PreStepPoint) {preName = PreStepPoint->GetName(); prepvname = prevol->GetName();}
  if(PostStepPoint) {postName = PostStepPoint->GetName(); postpvname = postvol->GetName();}
  
  int inout = -999;
  
  if((preName == "worldVOL" || preName == "LV_neutron_source_vacuum_sphere") && postName == "LV_detector_material_sphere") inout = -1;
  else if(preName == "LV_detector_material_sphere" && (postName == "worldVOL" || postName == "LV_neutron_source_vacuum_sphere")) inout = 1;
  else if(preName == "LV_detector_material_sphere" && postName == "LV_detector_material_sphere") inout = 0;
  else{inout = -999;}
  
  if(inout !=  -999){
    fEventAction->SetEdep(edep);
    fEventAction->AddEdep(0, edep);    
    fEventAction->FillKine(kine);
    fEventAction->SetPidPosTime(pid, particleName, pos, gtime);
    fEventAction->FillNtuple(trackid, parenid);
  }//if(inout != -999){
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
