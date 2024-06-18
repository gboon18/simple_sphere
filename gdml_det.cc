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
/// \file persistency/gdml/G04/gdml_det.cc
/// \brief Main program of the persistency/gdml/G04 example
//
//
//
//
// --------------------------------------------------------------
//      GEANT 4 - gdml_det
//
// --------------------------------------------------------------

#include <vector>
#include <time.h>

#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#endif

#include "ActionInitialization.hh"
#include "DetectorConstruction.hh"
#include "SensitiveDetector.hh"

#include "G4RunManagerFactory.hh"

#include "QGSP_BIC_AllHP.hh"

#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "G4GDMLParser.hh"

#include "G4ParticleHPManager.hh"

int main(int argc,char **argv)
{
  time_t now = time(0);
  
  G4cout << G4endl;
  G4cout << "Usage: gdml_det <intput_gdml_file:mandatory>"
	 << G4endl;
  G4cout << G4endl;

  if (argc<2)
    {
      G4cout << "Error! Mandatory input file is not specified!" << G4endl;
      G4cout << G4endl;
      return -1;
    }

  // Detect interactive mode (if only one argument) and define UI session
  //
  G4UIExecutive* ui = 0;
  if ( argc == 2 ) {
    ui = new G4UIExecutive(argc, argv);
  }
  
  G4GDMLParser parser;
  parser.Read(argv[1]);

  //Set the seeds before initializing the random engine
  G4long seed = time(NULL);
  if(argc == 5){
    seed = atoi(argv[4]);
  }
  
  G4cout<<"Random seed is "<<seed<<G4endl;
  G4Random::setTheSeed(seed);
  
#ifdef G4MULTITHREADED
  G4MTRunManager* runManager = new G4MTRunManager;
  runManager->SetNumberOfThreads(2*(G4Threading::G4GetNumberOfCores()));
  G4cout<<"Multi threaded"<<G4endl;
#else
  auto* runManager = G4RunManagerFactory::CreateRunManager();
  G4cout<<"Single threaded"<<G4endl;
#endif

  runManager->SetUserInitialization(new DetectorConstruction(parser));

  runManager->SetUserInitialization(new QGSP_BIC_AllHP);//_HP contains high precision neutron data

    // Replaced HP environmental variables with C++ calls
  G4ParticleHPManager::GetInstance()->SetSkipMissingIsotopes( false );
  // instructs Geant4 to throw an error if it encounters missing isotopes in the high-precision neutron cross-section libraries.
  // This is useful for ensuring that your simulation covers all relevant materials accurately without silently skipping missing data.

  G4ParticleHPManager::GetInstance()->SetDoNotAdjustFinalState( true );
  // prevent the high-precision models from adjusting the final state of reactions to conserve energy and momentum explicitly.
  // This setting can be useful if you want to study the unmodified outcomes of neutron interactions as provided by the cross-section data, but it may lead to non-physical results in some cases.
  
  G4ParticleHPManager::GetInstance()->SetUseOnlyPhotoEvaporation( true );
  // forces the use of the photo-evaporation model exclusively for gamma de-excitation, which can be relevant for certain types of neutron-induced reactions.
  // Whether you need this depends on the specific nuclear reactions you are simulating and their relevance to your detector's response.
  
  G4ParticleHPManager::GetInstance()->SetNeglectDoppler( false );
  // you include the effect of Doppler broadening in the simulation of neutron interactions, which can be important for accurately simulating the energy distribution of secondary particles, especially at high temperatures.
  
  G4ParticleHPManager::GetInstance()->SetProduceFissionFragments( false );
  // If you don't need modeling the production of fission fragments (e.g. neutron interaction with heavy nuclei that can undergo fission), you can disable this to potentially simplify the simulation.
  
  G4ParticleHPManager::GetInstance()->SetUseWendtFissionModel( false );
  G4ParticleHPManager::GetInstance()->SetUseNRESP71Model( false );
  // These options control the use of specific fission models. Whether you need these models depends on the specifics of the neutron interactions you're studying and the importance of modeling fission processes accurately in your simulation.

  if (argc == 2 || argc == 3){
    runManager->SetUserInitialization(new ActionInitialization("col_det.csv"));
  }
  if (argc == 5 || argc == 4){
    runManager->SetUserInitialization(new ActionInitialization(argv[3]));
  }
  runManager->Initialize();


  // Initialize visualization
  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();

  // Get the pointer to the User Interface manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  // Process macro or start UI session
  if ( ! ui )   // batch mode
    {
      G4String command = "/control/execute ";
      G4String fileName = argv[2];
      UImanager->ApplyCommand(command+fileName);
    }
  else           // interactive mode
    {
      UImanager->ApplyCommand("/control/execute vis.mac");
      ui->SessionStart();
      delete ui;
    }

  delete visManager;
  delete runManager;

  time_t then = time(0);
  time_t diff = then - now;

  // convert now to string form
  char* dt = ctime(&diff);
  // convert now to tm struct for UTC
  tm* gmtm = gmtime(&diff);
  dt = asctime(gmtm);
  G4cout << "It took :"<< dt <<"for the job to be done." << G4endl;

  return 0;
}
