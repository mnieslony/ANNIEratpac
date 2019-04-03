#include <RAT/GeoAnnieInnerStructureFactory.hh>
#include <RAT/Materials.hh>
#include <RAT/DB.hh>
#include <RAT/Log.hh>

#include <G4VSolid.hh>
#include <G4PVPlacement.hh>
#include <G4SubtractionSolid.hh>
#include <G4Box.hh>
#include <G4Tubs.hh>
#include <G4Polyhedra.hh>
#include <G4Material.hh>
#include <G4Ellipsoid.hh>
#include <G4VPhysicalVolume.hh>
#include <G4OpticalSurface.hh>
#include <G4LogicalBorderSurface.hh>
#include <CLHEP/Units/SystemOfUnits.h>
#include <G4GDMLParser.hh>
#include <G4Color.hh>
#include <vector>

using namespace std;

namespace RAT {

G4VPhysicalVolume *GeoAnnieInnerStructureFactory::Construct(DBLinkPtr table) {
    string volumeName = table->GetIndex();
    
    // Get references to the mother volume
    G4LogicalVolume *motherLog = FindMother(table->GetS("mother"));
    //     G4VPhysicalVolume *motherPhys = FindPhysMother(table->GetS("mother"));
    
    // Get InnerStructure information
    DBLinkPtr InnerStructureinfo = DB::Get()->GetLink("GEO","InnerStructure");
    vector<G4double> inner_structure_center = InnerStructureinfo->GetDArray("inner_structure_center"); // InnerStructure center
    G4int enable_structure = InnerStructureinfo->GetI("enable_structure"); // InnerStructure center
    G4String gdml_file = InnerStructureinfo->GetS("gdml_file");
    
    G4double rot_angle = InnerStructureinfo->GetD("rotation_angle");
    G4ThreeVector StructureCenter(inner_structure_center[0],inner_structure_center[1],inner_structure_center[2]);  
    G4RotationMatrix* rotm = new G4RotationMatrix();
    rotm->rotateZ(rot_angle*CLHEP::deg);
    
    // Get structure from GDML file
    G4VPhysicalVolume* innerstructure_phys;
    G4LogicalVolume* innerstructure_log;
    G4VPhysicalVolume* innerstructure_phys_placement;
    G4GDMLParser parser;
    
    if(enable_structure != 0){
      parser.Read(gdml_file);
      innerstructure_phys = parser.GetWorldVolume();
      
      innerstructure_log = innerstructure_phys->GetLogicalVolume();
      innerstructure_phys_placement = new G4PVPlacement(rotm, StructureCenter, innerstructure_log, "innerstructure_phys", motherLog, false, 0, false);
      
      innerstructure_log->SetVisAttributes(G4Color(0.1,0.,1.0,1));
    }
    
    //     G4Material *frameMaterial = G4Material::GetMaterial(InnerStructureinfo->GetS("frame_material")); // frame material
    //     G4Material *ncvMaterial = G4Material::GetMaterial(InnerStructureinfo->GetS("ncv_material")); // ncv material
    //     G4Material *liquidMaterial = G4Material::GetMaterial(InnerStructureinfo->GetS("liquid_material")); // liquid material
    
    //     DBLinkPtr OPTICSinfo = DB::Get()->GetLink("GEO","InnerStructure");
    //     G4OpticalSurface *transparent_surf = GetSurface(OPTICSinfo->GetS("transparent_surface"));
    //     G4OpticalSurface *opaque_surf = GetSurface(OPTICSinfo->GetS("opaque_surface"));
    //     G4OpticalSurface *reflective_surf = GetSurface(OPTICSinfo->GetS("reflective_surface"));
    //     G4OpticalSurface *metal_surf = GetSurface(OPTICSinfo->GetS("metal_surface"));
    
    if(enable_structure != 0){
      return innerstructure_phys_placement;
    } else {
      return NULL; 
    }
}

G4OpticalSurface *GeoAnnieInnerStructureFactory::GetSurface(string surface_name) {
    if (Materials::optical_surface.count(surface_name) == 0)
        Log::Die("error: surface "+ surface_name + " does not exist");
    return Materials::optical_surface[surface_name];
}

G4VisAttributes *GeoAnnieInnerStructureFactory::GetVisAttributes(DBLinkPtr table) {
    try {
        int invisible = table->GetI("invisible");
        if (invisible) return new G4VisAttributes(G4VisAttributes::Invisible);
    } catch (DBNotFoundError &e) { };
    
    G4VisAttributes *vis = new G4VisAttributes();
    
    try {
        const vector<double> &color = table->GetDArray("color");
        if (color.size() == 3) // RGB
            vis->SetColour(G4Colour(color[0], color[1], color[2]));
        else if (color.size() == 4) // RGBA
            vis->SetColour(G4Colour(color[0], color[1], color[2], color[3]));
        else
            warn << "error: " << table->GetName()  << "[" << table->GetIndex() << "].color must have 3 or 4 components" << newline;
    } catch (DBNotFoundError &e) { };
    
    try {
        string drawstyle = table->GetS("drawstyle");
        if (drawstyle == "wireframe")
            vis->SetForceWireframe(true);
        else if (drawstyle == "solid")
            vis->SetForceSolid(true);
        else
            warn << "error: " << table->GetName() << "[" << table->GetIndex() << "].drawstyle must be either \"wireframe\" or \"solid\".";
    } catch (DBNotFoundError &e) { };
    
    try {
        int force_auxedge = table->GetI("force_auxedge");
        vis->SetForceAuxEdgeVisible(force_auxedge == 1);
    } catch (DBNotFoundError &e) { };

    return vis;
}

} // namespace RAT
