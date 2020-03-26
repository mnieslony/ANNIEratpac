////// AmBe source geometry
// The source (beryllium) is enclosed in a case (stainless steel)

{
name: "GEO",
index: "AmBe_housing_cap", //represents the black bag with the wires inside
enable: 1,
valid_begin: [0, 0],
valid_end: [0, 0],
mother: "detector",
type: "tube",
r_max: 30.0,
size_z: 25.0,
position: [0.0, -1000.0, 1075.0], // position is housing body position + 7.5 cm
material: "acrylic_black", 
color: [0.0, 1.0, 1.0, 0.1],
drawstyle: "solid",
}

{
name: "GEO",
index: "AmBe_housing_body",
enable: 1,
valid_begin: [0, 0],
valid_end: [0, 0],
mother: "detector",
type: "tube",
r_max: 30.0,
size_z: 50.0,
position: [0.0, -1000.0, 1000.0],
material: "acrylic_black", 
color: [0.0, 1.0, 1.0, 0.1],
drawstyle: "solid",
}

{
name: "GEO",
index: "AmBe_housing_air",
enable: 1,
valid_begin: [0, 0],
valid_end: [0, 0],
mother: "AmBe_housing_body",
type: "tube",
r_max: 25.0,
size_z: 45.0,
position: [0.0, 0.0, 0.0],
material: "air", 
color: [0.0, 1.0, 1.0, 0.1],
drawstyle: "solid",
}

{
name: "GEO",
index: "AmBe_BGO",
enable: 1,
valid_begin: [0, 0],
valid_end: [0, 0],
mother: "AmBe_housing_air",
type: "tube",
r_max: 25.0,
size_z: 25.0,
position: [0.0, 0.0, -10.0],
material: "BGO_scint",
color: [0.0, 0.3, 1.0, 0.1],
drawstyle: "solid",
}

{
name: "GEO",
index: "AmBe_placer",
valid_begin: [0, 0],
valid_end: [0, 0],
mother: "AmBe_housing_air",
type: "tube",
r_max: 25.0,
size_z: 5.0,
position: [0.0, 0.0, -40.0],
material: "acrylic_uva_McMaster",
color: [0.0, 1.0, 1.0, 0.5],
drawstyle: "solid",
}

{
name: "GEO",
index: "AmBe_case",
valid_begin: [0, 0],
valid_end: [0, 0],
mother: "AmBe_placer",
type: "tube",
r_max: 5.0,
size_z: 5.0,
position: [0.0, 0.0, 0.0],
material: "stainless_steel",
color: [0.0, 1.0, 1.0, 0.7],
drawstyle: "solid",
}

{
name: "GEO",
index: "AmBe_source",
valid_begin: [0, 0],
valid_end: [0, 0],
mother: "AmBe_case",
type: "tube",
r_max: 2.5,
size_z: 2.5,
position: [0.0, 0.0, 0.0],
material: "beryllium",
color: [0.0, 1.0, 1.0, 0.9],
drawstyle: "solid",
}
