{
name: "GEO",
index: "world",
valid_begin: [0, 0],
valid_end: [0, 0],
mother: "", // world volume has no mother
type: "box",
size: [20000.0, 20000.0, 20000.0], // mm, half-length
material: "rock",
invisible: 1,
}

{
name: "GEO",
index: "wbls",
valid_begin: [0, 0],
valid_end: [0, 0],
mother: "world",
type: "sphere",
r_max: 10000.0,
position: [0.0, 0.0, 0.0],
material: "wbls_1pct_0420",
color: [0.4, 0.4, 0.6, 0.05],
}

{
name: "GEO",
index: "target",
valid_begin: [0, 0],
valid_end: [0, 0],
mother: "wbls",
type: "sphere",
r_max: 7000.0,
position: [0.0, 0.0, 0.0],
material: "wbls_1pct_0420",
color: [0.4, 0.4, 0.6, 0.05],
}

