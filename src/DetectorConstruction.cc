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
/// \file DetectorConstruction.cc
/// \brief Implementation of the DetectorConstruction class
 
#include "DetectorConstruction.hh"
#include "SensitiveDetector.hh"
#include "G4SDManager.hh"
#include "G4GDMLParser.hh"
 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction(const G4GDMLParser& parser)
 : G4VUserDetectorConstruction(),
   fParser(parser)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  return fParser.GetWorldVolume();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::ConstructSDandField()
{
  //------------------------------------------------ 
  // Sensitive detectors
  //------------------------------------------------ 

  G4SDManager* SDman = G4SDManager::GetSDMpointer();
     
  G4String detSDname = "detector";
  SensitiveDetector* detSD = 
    new SensitiveDetector(detSDname);
  SDman->AddNewDetector( detSD );
   
  G4String colSDname = "collimator";
  SensitiveDetector* colSD = 
    new SensitiveDetector(colSDname);
  SDman->AddNewDetector( colSD );
   
  ///////////////////////////////////////////////////////////////////////
  //
  // Example how to retrieve Auxiliary Information for sensitive detector
  //
  const G4GDMLAuxMapType* auxmap = fParser.GetAuxMap();
  G4cout << "Found " << auxmap->size()
            << " volume(s) with auxiliary information."
            << G4endl << G4endl;
  for(G4GDMLAuxMapType::const_iterator iter=auxmap->begin();
      iter!=auxmap->end(); iter++) 
  {
    G4cout << "Volume " << ((*iter).first)->GetName()
           << " has the following list of auxiliary information: "
           << G4endl << G4endl;
    for (G4GDMLAuxListType::const_iterator vit=(*iter).second.begin();
         vit!=(*iter).second.end(); vit++)
    {
      G4cout << "--> Type: " << (*vit).type
                << " Value: " << (*vit).value << G4endl;
    }
  }
  G4cout << G4endl;

  // The same as above, but now we are looking for
  // sensitive detectors setting them for the volumes

  for(G4GDMLAuxMapType::const_iterator iter=auxmap->begin();
      iter!=auxmap->end(); iter++) 
  {
    G4cout << "Volume " << ((*iter).first)->GetName()
           << " has the following list of auxiliary information: "
           << G4endl << G4endl;
    for (G4GDMLAuxListType::const_iterator vit=(*iter).second.begin();
         vit!=(*iter).second.end();vit++)
    {
      // if ((*vit).type=="det" || (*vit).type=="coll")
      if ((*vit).type=="det")
      {
        G4cout << "Attaching sensitive detector " << (*vit).value
               << " to volume " << ((*iter).first)->GetName()
               <<  G4endl << G4endl;

        G4VSensitiveDetector* mydet = 
          SDman->FindSensitiveDetector((*vit).value);
        if(mydet) 
        {
          G4LogicalVolume* myvol = (*iter).first;
          myvol->SetSensitiveDetector(mydet);
        }
        else
        {
          G4cout << (*vit).value << " detector not found" << G4endl;
        }
      }//if ((*vit).type=="det")
      // if ((*vit).type=="coll")
      // {
      //   G4cout << "Attaching sensitive collimator " << (*vit).value
      //          << " to volume " << ((*iter).first)->GetName()
      //          <<  G4endl << G4endl;

      //   G4VSensitiveDetector* mydet = 
      //     SDman->FindSensitiveDetector((*vit).value);
      //   if(mydet) 
      //   {
      //     G4LogicalVolume* myvol = (*iter).first;
      //     myvol->SetSensitiveDetector(mydet);
      //   }
      //   else
      //   {
      //     G4cout << (*vit).value << " collimator not found" << G4endl;
      //   }
      // }//if ((*vit).type=="coll")
    }
  }
}
