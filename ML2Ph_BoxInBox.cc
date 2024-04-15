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
// Based on code written by :
//	^Claudio Andenna  claudio.andenna@ispesl.it, claudio.andenna@iss.infn.it
//      *Barbara Caccia barbara.caccia@iss.it
//      with the support of Pablo Cirrone (LNS, INFN Catania Italy)
//	with the contribute of Alessandro Occhigrossi*
//
// ^INAIL DIPIA - ex ISPESL and INFN Roma, gruppo collegato Sanità, Italy
// *Istituto Superiore di Sanità and INFN Roma, gruppo collegato Sanità, Italy
//  Viale Regina Elena 299, 00161 Roma (Italy)
//  tel (39) 06 49902246
//  fax (39) 06 49387075
//
// more information:
// http://g4advancedexamples.lngs.infn.it/Examples/medical-linac
//
// 
// Modified code by :
// Allison O'Brien
// Rebecca Abergel
//*******************************************************//

#include "ML2Ph_BoxInBox.hh"
#include "G4SystemOfUnits.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4PSDoseDeposit3D.hh"
#include "G4PVReplica.hh"


CML2Ph_BoxInBox::CML2Ph_BoxInBox()
{// phantom size
	halfSize.set(150.*mm,150.*mm,150.*mm);
// phantom position
	centre.set(0.*mm,0.*mm,0.*mm);
}

CML2Ph_BoxInBox::~CML2Ph_BoxInBox(void)
{
}
void CML2Ph_BoxInBox::writeInfo()
{
	G4cout<<"\n\n\tcentre of the inside box: " <<centreBoxInside/mm<<" [mm]"<< G4endl;
	G4cout<<"\thalf thickness of the inside box: " <<halfBoxInside_Thickness/mm<<" [mm]\n"<< G4endl;
}
bool CML2Ph_BoxInBox::Construct(G4VPhysicalVolume *PWorld)
{	
	PVWorld = PWorld;

	G4Material* AIR = G4NistManager::Instance()->FindOrBuildMaterial("G4_AIR");
	G4Material* WATER = G4NistManager::Instance()->FindOrBuildMaterial("G4_WATER");
	// G4double A, Z;
	// A = 1.01*g/mole;
	// G4Element* elH = new G4Element ("Hydrogen","H",Z = 1.,A);

	// A = 12.011*g/mole;
	//G4Element* elC = new G4Element("Carbon","C",Z = 6.,A);  

	//A = 16.00*g/mole;
	//G4Element* elO = new G4Element("Oxygen","O",Z = 8.,A);

	//G4double d= 1.18*g/cm3;
	//G4int natoms, ncomponents;
	//G4Material* PMMA = new G4Material("Polimetilmetacrilato",d,ncomponents=3);
	//PMMA->AddElement(elC, natoms=5);
	//PMMA->AddElement(elH, natoms=8);
	//PMMA->AddElement(elO, natoms=2);

	//d= 0.1*g/cm3;
	//G4Material* lightWater = new G4Material("lightWater",d,ncomponents=2);
	//lightWater->AddElement(elH, natoms=2);
	//lightWater->AddElement(elO, natoms=1);
	
	//Voxel sizes 11/30
	G4int nx = 90;
	G4int ny = 90;
	G4int nz = 90;

	G4cout << "VoxelX, voxelY, Voxelz = " << nx << ", " << ny << ", " << nz << "; " << G4endl;

	// Voxel sizes for OutMinusInBox 2/3
	G4int nx_OutMinusInBox = 90;
	G4int ny_OutMinusInBox = 90;
	G4int nz_OutMinusInBox = 90;

	G4cout << "VoxelX_OutMinusInBox, voxelY_OutMinusInBox, Voxelz_OutMinusInBox = " << nx_OutMinusInBox << ", " << ny_OutMinusInBox << ", " << nz_OutMinusInBox << "; " << G4endl;

	// Phantom and sens sizes 2/3
	G4ThreeVector phantomSize; //Inside box
	phantomSize.setX(20. * cm);
	phantomSize.setY(2.5 * cm);
	phantomSize.setZ(2.35 * cm);

	G4ThreeVector sensSize;
	sensSize.setX(phantomSize.x() / nx);
	sensSize.setY(phantomSize.y() / ny);
	sensSize.setZ(phantomSize.z() / nz);

	G4ThreeVector phantomSize_OutMinusInBox; //OutMinusIn box
	phantomSize_OutMinusInBox.setX(30. * cm);
	phantomSize_OutMinusInBox.setY(30. * cm);
	phantomSize_OutMinusInBox.setZ(30. * cm);

	G4ThreeVector sensSize_OutMinusInBox;
	sensSize_OutMinusInBox.setX(phantomSize_OutMinusInBox.x() / nx_OutMinusInBox);
	sensSize_OutMinusInBox.setY(phantomSize_OutMinusInBox.y() / ny_OutMinusInBox);
	sensSize_OutMinusInBox.setZ(phantomSize_OutMinusInBox.z() / nz_OutMinusInBox);


// BOX INSIDE
	G4Material *boxInSideMaterial;

	boxInSideMaterial=AIR;
	G4cout <<"boxInSideMaterial name "<<boxInSideMaterial->GetName() <<" density "<< boxInSideMaterial->GetDensity()/(g/cm3) <<" g/cm3"<< G4endl;

	centreBoxInside.set(0,0,-114.75); // the centre of the inside box
	halfBoxInside_Thickness=1.175*cm; // the half thickness of the inside box

	G4Box *boxInSide=new G4Box("BoxInSide", 10.*cm, 1.25*cm, halfBoxInside_Thickness);
	G4LogicalVolume *boxInSideLV=new G4LogicalVolume(boxInSide, boxInSideMaterial, "boxInSideLV");
	boxInSidePV = new G4PVPlacement(0, centre+centreBoxInside,"BoxInsidePV", boxInSideLV,PVWorld,false,0,0);

// layer PMMA
//	G4Material *layerMaterial=G4NistManager::Instance()->FindOrBuildMaterial("AIR"); // changable
//	G4double halfPMMA_Z_Thickness=0.5*cm;
//	G4Box *layer=new G4Box("layer", halfSize.getX(), halfSize.getY(), halfPMMA_Z_Thickness);
//	G4LogicalVolume *layerLV = new G4LogicalVolume(layer, layerMaterial, "layerLV");
//	layerPV = new G4PVPlacement(0, centre+G4ThreeVector(0,0,-halfSize.getZ()+halfPMMA_Z_Thickness),"layerPV", layerLV,PVWorld,false,0,0);

//	G4cout <<"layerMaterial name "<<layerMaterial->GetName() <<" density " << layerMaterial->GetDensity()/(g/cm3) <<" g/cm3"<< G4endl;

// BOX OUTSIDE 
	// G4Material *boxOutSideMaterial=G4NistManager::Instance()->FindOrBuildMaterial("G4_LUNG_ICRP"); // changable
	// boxOutSideMaterial=lightWater;
	G4Material *boxOutSideMaterial=G4NistManager::Instance()->FindOrBuildMaterial("G4_AIR"); 
	boxOutSideMaterial = WATER;
	// G4double halfBoxOutSide_Thickness=halfSize.getZ()-halfPMMA_Z_Thickness;
	G4double halfBoxOutSide_Thickness = halfSize.getZ();
	G4Box *boxOutSide=new G4Box("BoxOutSide", halfSize.getX(), halfSize.getY(), halfBoxOutSide_Thickness);

	// boolean logic subtraction between outside box and inside box

	G4SubtractionSolid* OutMinusInBox = new G4SubtractionSolid("OutMinusInBox",	boxOutSide, boxInSide, 0, centreBoxInside);
	G4LogicalVolume *OutMinusInBoxLV = new G4LogicalVolume(OutMinusInBox, boxOutSideMaterial,"OutMinusInBoxLV",0,0,0);
	// OutMinusInBoxPV = new G4PVPlacement(0, centre+G4ThreeVector(0,0,-halfSize.getZ()+halfBoxOutSide_Thickness),
	//				"OutMinusInBoxPV",OutMinusInBoxLV,PVWorld,false,0);
	OutMinusInBoxPV = new G4PVPlacement(0, centre,"OutMinusInBoxPV",OutMinusInBoxLV,PVWorld,false,0);

	G4cout <<"boxOutSideMaterial name "<<boxOutSideMaterial->GetName() <<" density "<<boxOutSideMaterial->GetDensity()/(g/cm3) <<" g/cm3"<< G4endl;
	

	// The phantom is voxelised in 3D
	G4String yRepName("RepY");
	G4VSolid* solYRep = new G4Box(yRepName, phantomSize.x() / 2., sensSize.y() / 2., phantomSize.z() / 2.);
	G4LogicalVolume* logYRep = new G4LogicalVolume(solYRep, AIR, yRepName);
	new G4PVReplica(yRepName, logYRep, boxInSideLV, kYAxis, ny, sensSize.y());

	G4String xRepName("RepX");
	G4VSolid* solXRep = new G4Box(xRepName, sensSize.x() / 2., sensSize.y() / 2., phantomSize.z() / 2.);
	G4LogicalVolume* logXRep = new G4LogicalVolume(solXRep, AIR, xRepName);
	new G4PVReplica(xRepName, logXRep, logYRep, kXAxis, nx, sensSize.x());

	G4String zVoxName("phantomSens");
	G4VSolid* solVoxel = new G4Box(zVoxName, sensSize.x() / 2., sensSize.y() / 2., sensSize.z() / 2.);
	G4LogicalVolume* LVPhantomSens = new G4LogicalVolume(solVoxel, AIR, zVoxName); // This is the Sensitive Volume

	new G4PVReplica(zVoxName, LVPhantomSens, logXRep, kZAxis, nz, sensSize.z());

	// Voxelization for OutMinusInBox 2/3
	G4String yRepName_OutMinusInBox("RepY_OutMinusInBox");
	G4VSolid* solYRep_OutMinusInBox = new G4Box(yRepName_OutMinusInBox, phantomSize_OutMinusInBox.x() / 2., sensSize_OutMinusInBox.y() / 2., phantomSize_OutMinusInBox.z() / 2.);
	G4LogicalVolume* logYRep_OutMinusInBox = new G4LogicalVolume(solYRep_OutMinusInBox, boxOutSideMaterial, yRepName_OutMinusInBox);
	new G4PVReplica(yRepName_OutMinusInBox, logYRep_OutMinusInBox, OutMinusInBoxLV, kYAxis, ny_OutMinusInBox, sensSize_OutMinusInBox.y());

	G4String xRepName_OutMinusInBox("RepX_OutMinusInBox");
	G4VSolid* solXRep_OutMinusInBox = new G4Box(xRepName_OutMinusInBox, sensSize_OutMinusInBox.x() / 2., sensSize_OutMinusInBox.y() / 2., phantomSize_OutMinusInBox.z() / 2.);
	G4LogicalVolume* logXRep_OutMinusInBox = new G4LogicalVolume(solXRep_OutMinusInBox, boxOutSideMaterial, xRepName_OutMinusInBox);
	new G4PVReplica(xRepName_OutMinusInBox, logXRep_OutMinusInBox, logYRep_OutMinusInBox, kXAxis, nx_OutMinusInBox, sensSize_OutMinusInBox.x());

	G4String zVoxName_OutMinusInBox("phantomSens_OutMinusInBox");
	G4VSolid* solVoxel_OutMinusInBox = new G4Box(zVoxName_OutMinusInBox, sensSize_OutMinusInBox.x() / 2., sensSize_OutMinusInBox.y() / 2., sensSize_OutMinusInBox.z() / 2.);
	G4LogicalVolume* LVPhantomSens_OutMinusInBox = new G4LogicalVolume(solVoxel_OutMinusInBox, boxOutSideMaterial, zVoxName_OutMinusInBox);

	new G4PVReplica(zVoxName_OutMinusInBox, LVPhantomSens_OutMinusInBox, logXRep_OutMinusInBox, kZAxis, nz_OutMinusInBox, sensSize_OutMinusInBox.z());


	// Region for cuts
	G4Region *regVol= new G4Region("BoxInBoxR");
	G4ProductionCuts* cuts = new G4ProductionCuts;
	cuts->SetProductionCut(0.1*mm); //originally 0.01*mm
	regVol->SetProductionCuts(cuts);


	OutMinusInBoxLV->SetRegion(regVol);
	regVol->AddRootLogicalVolume(OutMinusInBoxLV);
	OutMinusInBoxLV->SetUserLimits(new G4UserLimits(0.01*mm));

	//layerLV->SetRegion(regVol);
	//regVol->AddRootLogicalVolume(layerLV);
	//layerLV->SetUserLimits(new G4UserLimits(0.01*mm));

	boxInSideLV->SetRegion(regVol);
	regVol->AddRootLogicalVolume(boxInSideLV);
	boxInSideLV->SetUserLimits(new G4UserLimits(0.01*mm));

	// Visibility
//	G4VisAttributes* simple_PMMA_VisAttWalls= new G4VisAttributes(G4Colour::Gray());
	G4VisAttributes* simple_InBox_VisAttWalls= new G4VisAttributes(G4Colour::Red());
	G4VisAttributes* simple_OutBox_VisAttWalls= new G4VisAttributes(G4Colour::Blue());

	simple_OutBox_VisAttWalls->SetVisibility(true);
// 	simple_OutBox_VisAttWalls->SetForceWireframe(true);
// 	simple_OutBox_VisAttWalls->SetForceAuxEdgeVisible(true);
	simple_OutBox_VisAttWalls->SetLineWidth(2.);
//	simple_OutBox_VisAttWalls->SetForceSolid(true);

//	simple_PMMA_VisAttWalls->SetVisibility(true);
// 	simple_PMMA_VisAttWalls->SetForceSolid(true);

	simple_InBox_VisAttWalls->SetVisibility(true);
// 	simple_InBox_VisAttWalls->SetForceSolid(true);

	OutMinusInBoxLV->SetVisAttributes(simple_OutBox_VisAttWalls);
	boxInSideLV->SetVisAttributes(simple_InBox_VisAttWalls);
	//layerLV->SetVisAttributes(simple_PMMA_VisAttWalls);
	
	//Dose scoring 11/30 inner box
	G4MultiFunctionalDetector* myScorer = new G4MultiFunctionalDetector("PhantomSD");
	G4SDManager::GetSDMpointer()->AddNewDetector(myScorer);
	LVPhantomSens->SetSensitiveDetector(myScorer);

	G4VPrimitiveScorer* totalDose = new G4PSDoseDeposit3D("TotalDose", nx, ny, nz);
	myScorer->RegisterPrimitive(totalDose);
	G4cout << "scorer registered: totalDose" << G4endl;

	// Dose scoring for outer box 2/2
	G4MultiFunctionalDetector* myScorer_OutMinusInBox = new G4MultiFunctionalDetector("PhantomSD_OutMinusInBox");
	G4SDManager::GetSDMpointer()->AddNewDetector(myScorer_OutMinusInBox);
	LVPhantomSens_OutMinusInBox->SetSensitiveDetector(myScorer_OutMinusInBox);

	G4VPrimitiveScorer* totalDose_OutMinusInBox = new G4PSDoseDeposit3D("TotalDose_OutMinusInBox", nx_OutMinusInBox, ny_OutMinusInBox, nz_OutMinusInBox);
	myScorer_OutMinusInBox->RegisterPrimitive(totalDose_OutMinusInBox);
	G4cout << "Scorer registered: totalDose_OutMinusInBox" << G4endl;

	return true;
}
