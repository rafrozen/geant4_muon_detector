/**
 * @file muonDetectorConstruction.cc
 * @brief 搭建探测器
 * @author loyxin
 * @version 1.0
 * @date 2017-09-10
 */
#include "muonDetectorConstruction.hh"
#include "muonDetectorMessenger.hh"

muonDetectorConstruction::muonDetectorConstruction()
: G4VUserDetectorConstruction()
{ 
  fMaterial = new muonMaterial();
  fMessenger = new muonDetectorMessenger(this);
}


muonDetectorConstruction::~muonDetectorConstruction()
{ }


#include <G4Box.hh>
#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4OpticalSurface.hh>
#include <G4MaterialPropertiesTable.hh>
#include <G4LogicalBorderSurface.hh>
#include <G4LogicalSkinSurface.hh>

G4VPhysicalVolume* muonDetectorConstruction::Construct()
{

  // world  box material air
  // name "world" point solidWorld logicalWorld physWorld
  G4Material* world_mat = fMaterial->GetfAir();

  G4Box* solidWorld = new G4Box("World",0.5*world_sizeXY, 0.5*world_sizeXY, 0.5*world_sizeZ);
  G4LogicalVolume* logicalWorld = new G4LogicalVolume(solidWorld, world_mat, "World");
  G4VPhysicalVolume* physWorld = new G4PVPlacement(0, G4ThreeVector(), logicalWorld, "World", NULL, false, 0, checkOverlaps);
  
  Constructmuondetector(logicalWorld,physWorld);

  ConstructPMT(logicalWorld);

  return physWorld;
}


#include <G4Trd.hh>

#include <G4SubtractionSolid.hh>
#include <G4Colour.hh>
#include <G4VisAttributes.hh>


void muonDetectorConstruction::Constructmuondetector(G4LogicalVolume* logicalWorld,G4VPhysicalVolume* physWorld)
{

  //shape material
  G4Material* shape_mat = fMaterial->Getfdetector();

  // point solidDector logicShape no physical point
  G4Trd* solidDetector1 = 
  new G4Trd("muondector1", shape_x1, shape_x1,shape_y1,shape_y2, shape_z);
  G4LogicalVolume* logicShape1 = 
  new G4LogicalVolume(solidDetector1, shape_mat, "muondector1");
  new G4PVPlacement(rotm, pos1, logicShape1, "muondector1", 
  logicalWorld, false, 0, checkOverlaps);


  G4Trd* solidDetector2 = 
  new G4Trd("muondector2", shape_x2, shape_x2,shape_y1,shape_y2, shape_z);
  G4LogicalVolume* logicShape2 = 
  new G4LogicalVolume(solidDetector2, shape_mat, "muondector2");
  new G4PVPlacement(rotm, pos2, logicShape2, "muondector2", 
  logicalWorld, false, 1, checkOverlaps);


  G4Colour red (1.0, 0.0, 0.0) ; // red
  G4VisAttributes* ShapeVisAtt= new G4VisAttributes(red);
  logicShape1->SetVisAttributes(ShapeVisAtt);
  logicShape2->SetVisAttributes(ShapeVisAtt);


  ConstructReflection(logicalWorld,solidDetector1,solidDetector2,physWorld);
}
#include <G4LogicalBorderSurface.hh>
void muonDetectorConstruction::ConstructReflection(G4LogicalVolume* logicalWorld, G4Trd* solidDetector1,G4Trd* solidDetector2,G4VPhysicalVolume* physWorld )
{
  // Al2O3
  G4Material* Al2O3 = fMaterial->Getfreflect();

  G4Trd* solid1 = new G4Trd("Al2O3", rshape_x1, rshape_x1,rshape_y1,rshape_y2, rshape_z);
  G4SubtractionSolid* Al2O3solid1 = new G4SubtractionSolid("Trd-Trd",solid1,solidDetector1,0,G4ThreeVector(-0.0*mm,-0.*mm,-1.*mm) );
  G4LogicalVolume* logic1Al2O3 = new G4LogicalVolume(Al2O3solid1, Al2O3, "Al2O31");
  G4PVPlacement* physi1 =  new G4PVPlacement(rotm, rpos1, logic1Al2O3, "Al2O31", logicalWorld, false, 0, checkOverlaps);
  
  G4Trd* solid2 = new G4Trd("Al2O3", rshape_x2, rshape_x2,rshape_y1,rshape_y2, rshape_z);
  G4SubtractionSolid* Al2O3solid2 = new G4SubtractionSolid("Trd-Trd",solid2,solidDetector2,0,G4ThreeVector(-0.0*mm,-0.*mm,-1.*mm) );
  G4LogicalVolume* logic2Al2O3 = new G4LogicalVolume(Al2O3solid2, Al2O3, "Al2O32");
  G4PVPlacement* physi2 = new G4PVPlacement(rotm, rpos2, logic2Al2O3, "Al2O32", logicalWorld, false, 1, checkOverlaps);
  
  // //设置光学性质

  // G4double fPolish = 1.;
  // G4double fReflectivity = 1.;//全反射

  // G4OpticalSurface* Al2O3Surface = new G4OpticalSurface("PhotonDetSurface",glisur,ground,dielectric_metal,fPolish);

  // G4MaterialPropertiesTable* Al2O3SurfaceProperty = new G4MaterialPropertiesTable();

  // G4double p_mppc[] = {1.70*eV, 3.47*eV};
  // const G4int nbins = sizeof(p_mppc)/sizeof(G4double);
  // G4double refl_mppc[] = {fReflectivity,fReflectivity};
  // assert(sizeof(refl_mppc) == sizeof(p_mppc));
  // G4double effi_mppc[] = {1, 1};
  // assert(sizeof(effi_mppc) == sizeof(p_mppc));

  // Al2O3SurfaceProperty->AddProperty("REFLECTIVITY",p_mppc,refl_mppc,nbins);
  // Al2O3SurfaceProperty->AddProperty("EFFICIENCY",p_mppc,effi_mppc,nbins);

  // Al2O3Surface->SetMaterialPropertiesTable(Al2O3SurfaceProperty);

  // new G4LogicalBorderSurface("Al2O3Surface",physWorld,physi1,Al2O3Surface);
  // new G4LogicalBorderSurface("Al2O3Surface",physWorld,physi2,Al2O3Surface);
}

void muonDetectorConstruction::ConstructPMT(G4LogicalVolume* logicalWorld)
{

  G4Material* Quartz = fMaterial->GetfPMT();

  G4Box* solidPMT1 = new G4Box("PMT_solid",PMT_x1,PMT_y,PMT_thick);
  G4LogicalVolume* logicalPMT1 = new G4LogicalVolume(solidPMT1,Quartz,"PMT_log1",0,0,0);
  
  G4Box* solidPMT2 = new G4Box("PMT_solid",PMT_x2,PMT_y,PMT_thick);
  G4LogicalVolume* logicalPMT2 = new G4LogicalVolume(solidPMT2,Quartz,"PMT_log2",0,0,0);
  
  G4Colour green (0.0, 1.0, 0.0) ; // green
  G4VisAttributes* PMTVisAtt= new G4VisAttributes(green);
  logicalPMT1->SetVisAttributes(PMTVisAtt);
  logicalPMT2->SetVisAttributes(PMTVisAtt);

  new G4PVPlacement(rotm,PMTpos1,logicalPMT1,"PMT1",logicalWorld,false,0,checkOverlaps);
  new G4PVPlacement(rotm,PMTpos2,logicalPMT2,"PMT2",logicalWorld,false,1,checkOverlaps);

  //设置光学性质

  // G4double fPolish = 0.;
  // G4double fReflectivity = 0.;//全吸收

  // G4OpticalSurface* photonDetSurface = new G4OpticalSurface("PhotonDetSurface",glisur,ground,dielectric_metal,
  // fPolish);

  // G4MaterialPropertiesTable* photonDetSurfaceProperty =
  // new G4MaterialPropertiesTable();

  // G4double p_mppc[] = 
  // {
  //   1.70*eV,1.73*eV,1.76*eV,1.79*eV,1.82*eV,
  //   1.85*eV,1.88*eV,1.91*eV,1.94*eV,1.97*eV,
  //   2.00*eV,2.03*eV,2.06*eV,2.09*eV,2.12*eV,
  //   2.15*eV,2.18*eV,2.21*eV,2.24*eV,2.27*eV,
  //   2.30*eV,2.33*eV,2.36*eV,2.39*eV,2.42*eV,
  //   2.45*eV,2.48*eV,2.51*eV,2.54*eV,2.57*eV,
  //   2.60*eV,2.63*eV,2.66*eV,2.69*eV,2.72*eV,
  //   2.75*eV,2.78*eV,2.81*eV,2.84*eV,2.87*eV,
  //   2.90*eV,2.93*eV,2.96*eV,2.99*eV,3.02*eV,
  //   3.05*eV,3.08*eV,3.11*eV,3.14*eV,3.17*eV,
  //   3.20*eV,3.23*eV,3.26*eV,3.29*eV,3.32*eV,
  //   3.35*eV,3.38*eV,3.41*eV,3.44*eV,3.47*eV};

  // const G4int nbins = sizeof(p_mppc)/sizeof(G4double);
  // G4double refl_mppc[] = 
  // {fReflectivity,fReflectivity,fReflectivity,fReflectivity,fReflectivity,fReflectivity,fReflectivity,fReflectivity,fReflectivity,fReflectivity,
  //   fReflectivity,fReflectivity,fReflectivity,fReflectivity,fReflectivity,fReflectivity,fReflectivity,fReflectivity,fReflectivity,fReflectivity,
  //   fReflectivity,fReflectivity,fReflectivity,fReflectivity,fReflectivity,fReflectivity,fReflectivity,fReflectivity,fReflectivity,fReflectivity,
  //   fReflectivity,fReflectivity,fReflectivity,fReflectivity,fReflectivity,fReflectivity,fReflectivity,fReflectivity,fReflectivity,fReflectivity,
  //   fReflectivity,fReflectivity,fReflectivity,fReflectivity,fReflectivity,fReflectivity,fReflectivity,fReflectivity,fReflectivity,fReflectivity,
  //   fReflectivity,fReflectivity,fReflectivity,fReflectivity,fReflectivity,fReflectivity,fReflectivity,fReflectivity,fReflectivity,fReflectivity};

  // assert(sizeof(refl_mppc) == sizeof(p_mppc));
  // G4double effi_mppc[] =    
  // {1,1,1,1,1,1,1,1,1,1,
  //   1,1,1,1,1,1,1,1,1,1,
  //   1,1,1,1,1,1,1,1,1,1,
  //   1,1,1,1,1,1,1,1,1,1,
  //   1,1,1,1,1,1,1,1,1,1,
  //   1,1,1,1,1,1,1,1,1,1};
  // assert(sizeof(effi_mppc) == sizeof(p_mppc));

  // photonDetSurfaceProperty->AddProperty("REFLECTIVITY",p_mppc,refl_mppc,nbins);
  // photonDetSurfaceProperty->AddProperty("EFFICIENCY",p_mppc,effi_mppc,nbins);

  // photonDetSurface->SetMaterialPropertiesTable(photonDetSurfaceProperty);

  // new G4LogicalSkinSurface("PhotonDetSurface",logicalPMT,photonDetSurface);
}

#include "EnergyTimeSD.hh"
#include "PMTSD.hh"
// 设置敏感探测器 记录 能量沉积 位置
#include <G4SDManager.hh>
#include <G4MultiFunctionalDetector.hh>
#include <G4PSEnergyDeposit.hh>
#include <G4VPrimitiveScorer.hh>
void muonDetectorConstruction::ConstructSDandField()
{
  G4SDManager* sdManager = G4SDManager::GetSDMpointer();
  sdManager->SetVerboseLevel(2);
  if(SDflag)
  {
  EnergyTimeSD* muondectorEn = new EnergyTimeSD("muondectorEn");
  SetSensitiveDetector("muondector1",muondectorEn);
  SetSensitiveDetector("muondector2",muondectorEn);
  sdManager->AddNewDetector(muondectorEn);

  PMTSD*  PMT_ET = new PMTSD("PMT_ET");
  SetSensitiveDetector("PMT_log1",PMT_ET);
  SetSensitiveDetector("PMT_log2",PMT_ET);
  sdManager->AddNewDetector(PMT_ET);
  }
}
