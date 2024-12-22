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

#include "G4RunManager.hh"
#include "G4GenericMessenger.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4ios.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
: G4VUserDetectorConstruction(), 
  fMessenger(0),
  fHodoscope1Logical(0),
  fWirePlane1Logical(0),
  fVisAttributes(),
  fArmAngle(0.*deg), fArmRotation(0), fSecondArmPhys(0)

{
    fArmRotation = new G4RotationMatrix();
    fArmRotation->rotateY(fArmAngle);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{
    delete fArmRotation;
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
    G4Material* vaccum = G4Material::GetMaterial("G4_Galactic");
    G4Material* tungsten = G4Material::GetMaterial("G4_W");
    G4Material* air = G4Material::GetMaterial("G4_AIR");
    G4Material* steel = G4Material::GetMaterial("Steel");
    G4bool checkOverlaps = true;

  
    // geometries --------------------------------------------------------------
    // experimental hall (world volume)
    G4VSolid* worldSolid 
      = new G4Box("worldBox",5.*m,5.*m,5.*m);
    G4LogicalVolume* worldLogical
      = new G4LogicalVolume(worldSolid,air,"worldLogical");
    G4VPhysicalVolume* worldPhysical
      = new G4PVPlacement(0,G4ThreeVector(),worldLogical,"worldPhysical",0,
                          false,0,checkOverlaps);

    G4VSolid* target 
      = new G4Box("targetBox",5.*1/2*cm,5.*1/2*cm,10.*1/2*cm);
    G4LogicalVolume* targetlogical
      = new G4LogicalVolume(target,tungsten,"targetLogical");
    G4VPhysicalVolume* targetphysical =new G4PVPlacement(0,G4ThreeVector(0.,0.,0.),targetlogical,
    "targetPhysical",worldLogical,false,0,checkOverlaps);

    G4VSolid* decaychamber 
      = new G4Tubs("decaychamber",0.,10.*cm,30*1/2*cm,0.*deg,360.*deg);
    G4LogicalVolume* decaychamberLogical
      = new G4LogicalVolume(decaychamber,vaccum,"decaychamberLogical");
    G4VPhysicalVolume*decaychambervolume = new G4PVPlacement(0,G4ThreeVector(0.,0.,20.*cm),decaychamberLogical,
    "chamber1Physical",worldLogical,false,0,checkOverlaps);
    
    
    G4VSolid* decaychamberst 
    = new G4Tubs("decaychamberst",10.*cm,10.6*cm,30*1/2*cm,0.*deg,360.*deg);
    G4LogicalVolume* decaychamberstLogical
    = new G4LogicalVolume(decaychamberst,steel,"decaychamberstLogical"); 
    G4VPhysicalVolume*decaychamberstvolume = new G4PVPlacement(0,G4ThreeVector(0.,0.,20.*cm),decaychamberstLogical,
    "chamberst1Physical",worldLogical,false,0,checkOverlaps);
    


    G4VSolid* Ecal
      = new G4Box("Ecal",12.*1/2*cm,12.*1/2*cm,44.*1/2*cm);
    G4LogicalVolume * Ecallogical
      = new G4LogicalVolume(Ecal,vaccum,"Ecallogical");
    G4VPhysicalVolume* Ecalphysical =new G4PVPlacement(0,G4ThreeVector(0.,0.,57.*cm),Ecallogical,
    "EcalPhysical",worldLogical,false,0,checkOverlaps);
    

    






    

    /*
    // "virtual" wire plane, "virtual" because it is a single volume
    // without the description of the wires
    G4VSolid* wirePlane1Solid 
      = new G4Box("wirePlane1Box",1.*m,30.*cm,0.1*mm);
    fWirePlane1Logical
      = new G4LogicalVolume(wirePlane1Solid,argonGas,"wirePlane1Logical");
    new G4PVPlacement(0,G4ThreeVector(0.,0.,0.),fWirePlane1Logical,
                      "wirePlane1Physical",chamber1Logical,
                      false,0,checkOverlaps);
    */
    
    // =============================================
    // Exercise 2a,b
    // Create a box of CsI, Pb or Scintillator
    // (for nist materials remember to use NIST name, e.g. G4_Pb)
    // to absorb particles.
    // Observe how different absorber influence
    //  shower dimentions.
    // Some parameters:
    //   Dimensions (x,y,z): 300x60x100 cm
    //   Position: on the far back of the "second arm" volume
    // =============================================

    /*
    G4Material* material = G4Material::GetMaterial("CsI");
    G4VSolid* hadCalorimeterSolid
    = new G4Box("HadCalorimeterBox",1.5*m,30.*cm,50.*cm);
    G4LogicalVolume* hadCalorimeterLogical
    = new G4LogicalVolume(hadCalorimeterSolid,material,"HadCalorimeterLogical");
    new G4PVPlacement(0,G4ThreeVector(0.,0.,3.*m),hadCalorimeterLogical,
                      "HadCalorimeterPhysical",secondArmLogical,
                      false,0,checkOverlaps);*/


    
    // visualization attributes ------------------------------------------------
    
    G4VisAttributes* visAttributes = new G4VisAttributes(G4Colour(1.0,1.0,1.0));
    visAttributes->SetVisibility(false);
    worldLogical->SetVisAttributes(visAttributes);
    fVisAttributes.push_back(visAttributes);
    
    visAttributes = new G4VisAttributes(G4Colour(0.8888,0.0,0.0));
    targetlogical->SetVisAttributes(visAttributes);
    fVisAttributes.push_back(visAttributes);
    
    visAttributes = new G4VisAttributes(G4Colour(0.5,0.,0.0,0.3));
    visAttributes->SetVisibility(true);
    visAttributes->SetForceSolid(true);
    decaychamberLogical->SetVisAttributes(visAttributes);
    fVisAttributes.push_back(visAttributes);
    

    
    visAttributes = new G4VisAttributes(G4Colour(0.3,0.3,0.3,0.4));
    visAttributes->SetVisibility(true);
    visAttributes->SetForceSolid(true);
    decaychamberstLogical->SetVisAttributes(visAttributes);
    fVisAttributes.push_back(visAttributes);
    

    visAttributes = new G4VisAttributes(G4Colour(0.5,0.,0.0,0.3));
    visAttributes->SetVisibility(true);
    visAttributes->SetForceSolid(true);
    Ecallogical->SetVisAttributes(visAttributes);
    fVisAttributes.push_back(visAttributes);
    

    
    // return the world physical volume ----------------------------------------
    
    return worldPhysical;
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::ConstructSDandField() {
    // 이곳에 센시티브 디텍터 및 필드 설정 로직을 구현
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::ConstructMaterials()
{
    G4NistManager* nistManager = G4NistManager::Instance();

    

    // Air 
    nistManager->FindOrBuildMaterial("G4_AIR");
  
    // Argon gas
    nistManager->FindOrBuildMaterial("G4_W");

    // Scintillator
    // (PolyVinylToluene, C_9H_10)
    nistManager->FindOrBuildMaterial("G4_Galactic");

    // 철(Fe), 크롬(Cr), 니켈(Ni) 원소 정의
    G4Material* Fe = nistManager->FindOrBuildMaterial("G4_Fe");  // 철
    G4Material* Cr = nistManager->FindOrBuildMaterial("G4_Cr");  // 크롬
    G4Material* Ni = nistManager->FindOrBuildMaterial("G4_Ni");  // 니켈

// 강철 (SS304) 비율로 합금 정의
    G4double density = 8.00*g/cm3;  // SS304 밀도
    G4Material* Steel = new G4Material("Steel", density, 3);
    Steel->AddMaterial(Fe, 0.70);  // 철 70%
    Steel->AddMaterial(Cr, 0.18);  // 크롬 18%
    Steel->AddMaterial(Ni, 0.12);  // 니켈 12%
    
    // CsI
    // =============================================
    // Exercise 1a
    //    Create material Cesium Iodide.
    //  Chemical formula CsI.
    // Some properties:
    //    Cs : A=55 Zeff=132.9*g/mol
    //     I : A=53 , Zeff=126.9*g/mol
    // Density of christal of CsI is rho=4.51*g/cm^3
    // =============================================

    //nistManager->FindOrBuildMaterial("G4_CESIUM_IODIDE");
    
    // Lead
    // =============================================
    // Exercise 1b
    //    Create material Lead from Nist Manager.
    // Note that it is actually a mixture of several isotopes.
    // If you want to build it by hand starting from isotopes you
    // can:
    //    G4Isotope* iso1= new G4Isotope( ... )
    //    ....
    //    G4Element* elem = new G4Element("name","symbol",nisotopes);
    //    elem->AddIsotope( iso1 );
    //    elem->AddIsotope( iso2 );
    //    ...
    // =============================================
    
    // Vacuum "Galactic"
    // nistManager->FindOrBuildMaterial("G4_Galactic");

    // Vacuum "Air with low density"
    // G4Material* air = G4Material::GetMaterial("G4_AIR");
    // G4double density = 1.0e-5*air->GetDensity();
    // nistManager
    //   ->BuildMaterialWithNewDensity("Air_lowDensity", "G4_AIR", density);

    G4cout << G4endl << "The materials defined are : " << G4endl << G4endl;
    G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
