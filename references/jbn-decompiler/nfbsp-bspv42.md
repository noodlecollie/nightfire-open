# Binary Space Partition - Version 42

From https://web.archive.org/web/20090630223318/http://wiki.nfbsp.com/index.php/Binary_Space_Partition_-_Version_42

[nightfire-bsp-format.md](nightfire-bsp-format.md) will probably be more informative.

## Header

```c
struct Header { // Length: 148
  int Version;
  int EntitiesOffset;
  int EntitiesLength;
  int PlanesOffset;
  int PlanesLength;
  int ShadersOffset;
  int ShadersLength;
  int FogsOffset;
  int FogsLength;
  int VerticesOffset;
  int VerticesLength;
  int ZeroVerticesOffset;
  int ZeroVerticesLength;
  int DrawIndicesOffset;
  int DrawIndicesLength;
  int VisibilityOffset;
  int VisibilityLength;
  int NodesOffset;
  int NodesLength;
  int SurfacesOffset;
  int SurfacesLength;
  int LightingOffset;
  int LightingLength;
  int LeavesOffset;
  int LeavesLength;
  int LeafSurfacesOffset;
  int LeafSurfacesLength;
  int LeafBrushesOffset;
  int LeafBrushesLength;
  int ModelsOffset;
  int ModelsLength;
  int BrushesOffset;
  int BrushesLength;
  int BrushSidesOffset;
  int BrushSidesLength;
  int TextureMapsOffset;
  int TextureMapsLength;
}
```

## Entities

The only pure ASCII portion of the BSP. It has no associated structure and can simply be treated as a string according to the length specified in the header. For a list of classnames that may be found in this lump, see the List of Entities by name or type.

## Planes

```c
struct Plane { // Length: 20
  float NormalX;
  float NormalY;
  float NormalZ;
  float Distance;
  int Type;
}
```

## Shaders

```c
struct Shader { // Length: 64
  byte[64] Texture;
}
```

## Fogs

```c
struct Fog { // Length: 64
  byte[64] WorldEffect;
}
```

## Vertices

```c
struct Vertex { // Length: 12
  float X;
  float Y;
  float Z;
}
```

## Zero Vertices

Matches vertices in length but it is filled with null bytes.

```c
struct ZeroVertex { // Length: 12
  float X;
  float Y;
  float Z;
}
```

## Draw Indices

```c
struct DrawIndex { // Length: 4
  int TriangleCorner;
}
```

### Explanation of Draw Indicies

A row from Faces:

```
6B1D 0000 AEE7 0000 0400 0000 F854 0100 0600 0000 0000 0000 3300 0000 0000 0000 D74E 0000 1300 0000 00FF FFFF 68A5 1900
                  a         b         c         d

a = 59310 (vert index)
b = 4     (vert count)
c = 87288 (draw indices index)
d = 6     (draw indices count)
```

To convert index to offset, we take the index number times size of each item. For verts, it is 12; for draw indices, it is 4.

```
59310 * 12 = 711720
87288 * 4 = 349152

Data from verts lump:
 0000 F044 0000 C8C2 0000 4042
 0080 EE44 0000 C8C2 0000 4042
 0080 EE44 0000 C8C2 0000 A042
 0000 F044 0000 C8C2 0000 A042

Data from draw indices lump:
 0000 0000
 0100 0000
 0200 0000
 0000 0000
 0200 0000
 0300 0000
```

Now, lets assume that the verts are loaded into an array using default key numbers. All array keys start from zero.

```
Key  Hex data stored in the array
  0  0000 F044 0000 C8C2 0000 4042
  1  0080 EE44 0000 C8C2 0000 4042
  2  0080 EE44 0000 C8C2 0000 A042
  3  0000 F044 0000 C8C2 0000 A042
```

Now, since it loaded 6 points, that means it is forming 2 triangles using slightly varying verts.

Draw indicies are just key values. When it says 0, it is loading the data stored at key 0 which in this example is 0000 F044 0000 C8C2 0000 4042.

```
Key  Raw Hex                        Ordered Pairs

First Triangle:
  0  0000 F044 0000 C8C2 0000 4042  (1920, -100, 48)
  1  0080 EE44 0000 C8C2 0000 4042  (1908, -100, 48)
  2  0080 EE44 0000 C8C2 0000 A042  (1908, -100, 80)

Second Triangle:
  0  0000 F044 0000 C8C2 0000 4042  (1920, -100, 48)
  2  0080 EE44 0000 C8C2 0000 A042  (1908, -100, 80)
  3  0000 F044 0000 C8C2 0000 A042  (1920, -100, 80)
```

These two triangles form a square. It is easy to see that these triangles only have two varying dimensions, making them two-dimensional.

Though most sets have two triangles, they do NOT have to fit together to make a square. They can make any geometric shape that two triangles can make. Also, two or more sets of meshes can be used together to make more complex shapes.

## Visibility

```c
struct Visibility { // Length:?
  // Unknown
}
```

### Explanation of Visibility

Leaves are grouped into clusters for the purpose of storing the visibility information. This is to conserve space in the PVS since it's likely that nearby leaves will have similar potentially visible areas. The visibility lump contains an array of bsp_vis_offset structures with the same number of elements as there are clusters. The bsp_vis_offset structure format is:

```c
struct bsp_vis_offset {
  uint32 pvs; // offset (in bytes) from the beginning of the visibility lump
  uint32 phs; // ?
}
```

The rest of the visibility lump is the actual visibility information. For every cluster the visibility state (either visible or occluded) is stored for every other cluster in the world. Clusters are always visible from within themselves. Because this is such a massive amount of data, this array is stored as a bit vector (one bit per element) and 0's are run-length-encoded. Here's an example of a C-routine to decompress the PVS into a byte array (this was adapted from the "Quake Specifications" document):

```c
int v = offset;
memset(cluster_visible, 0, num_clusters);
for (int c = 0; c < num_clusters; v++) {
  if (pvs_buffer[v] == 0) {
    v++;
    c += 8 * pvs_buffer[v];
  } else {
    for (buint8 bit = 1; bit != 0; bit *= 2, c++) {
      if (pvs_buffer[v] & bit) {
        cluster_visible[c] = 1;
      }
    }
  }
}
```

## Nodes

```c
struct Node { // Length: 36
  int PlaneIndex;
  int Child0; // negative numbers are -(leafs+1), not nodes
  int Child1;
  float MinX;
  float MinY;
  float MinZ;
  float MaxX;
  float MaxY;
  float MaxZ;
}
```

## Surfaces

```c
struct Surface { // Length: 48
  int PlaneIndex;
  int VertexIndex;
  int VertexCount;
  int DrawIndex;
  int DrawCount;
  SurfaceFlags Flags;
  int ShaderIndex;
  int FogIndex;
  int TextureMapShaderIndex;
  int TextureMapLightingIndex;
  sbyte[] Styles = new sbyte[4];
  int LightingOffset; // start of [numstyles * ? * 3] samples
}
```

## Lighting

```c
struct Lighting { // Length: 3
  byte Red;
  byte Green;
  byte Blue;
}
```

## Leaves

```c
struct Leaf { // Length: 48
  int Type; // 1 loads face and brush information, 2 does not.
  int VisibilityOffset;
  float MinX;
  float MinY;
  float MinZ;
  float MaxX;
  float MaxY;
  float MaxZ;
  int LeafSurfacesIndex;
  int LeafSurfacesCount;
  int LeafBrushesIndex;
  int LeafBrushesCount;
}
```

## Leaf Surfaces

```
struct LeafSurface { // Length: 4
  int SurfaceIndex;
}
```

## Leaf Brushes

```c
struct LeafBrush { // Length: 4
  int BrushIndex;
}
```

## Models

```c
struct Model { // Length: 56
  float MinX;
  float MinY;
  float MinZ;
  float MaxX;
  float MaxY;
  float MaxZ;
  int Unknown0; // null
  int Unknown1; // null
  int Unknown2; // null
  int Unknown3; // null
  int BrushesIndex;
  int BrushesCount;
  int SurfacesIndex;
  int SurfacesCount;
}
```

## Brushes

```c
struct Brush { // Length: 12
  int Attributes;
  int SidesIndex;
  int SidesCount;
}
```

## Brush Sides

```c
struct BrushSide { // Length: 8
  int ShaderIndex;
  int PlaneIndex; // positive plane side faces out of the leaf
}
```

## Texture Maps

```c
struct TextureMap { // Length: 32
  float SX;
  float SY;
  float SZ;
  float SOffset;
  float TX;
  float TY;
  float TZ;
  float TOffset;
}
```
