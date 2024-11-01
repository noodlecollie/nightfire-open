Nightfire BSP Format
====================

This is a documentation page on the Nightfire BSP format. Information here was established by me when attempting to load Nightfire BSPs into the Xash3D engine directly, and is based in part on the information from the pages linked below.

* The [archived research page of NFBSP](https://web.archive.org/web/20090630223318/http://wiki.nfbsp.com:80/index.php/Binary_Space_Partition_-_Version_42).
* [JBN BSP Lump Tools](https://code.google.com/archive/p/jbn-bsp-lump-tools/downloads), existing tools for modifying/decompiling the Nightfire BSP format.
* Some information about various BSP file formats:
  * **General**
    * [BSP as an Algorithm (Wikipedia)](https://en.wikipedia.org/wiki/Binary_space_partitioning)
    * [Lumps and Compilation (Wikipedia, archived)](https://ipfs.io/ipfs/QmXoypizjW3WknFiJnKLwHCnL72vedxjQkDDP1mXWo6uco/wiki/BSP_(file_format).html)
  * **Quake**
    * [BSP Spec 1 (Gamers.org)](http://www.gamers.org/dEngine/quake/spec/quake-spec34/qkspec_4.htm)
    * [BSP Spec 2 (Flipcode.com)](http://www.flipcode.com/archives/Quake_2_BSP_File_Format.shtml)
  * **Half-Life/GoldSrc**
    * [BSP Spec 1 (HLBSP)](http://hlbsp.sourceforge.net/index.php?content=bspdef)
* [LibBSP](https://github.com/wfowler1/LibBSP) and a general-purpose, open source [BSP decompiler](https://github.com/wfowler1/bsp-decompiler), which demonstrate how to work with the BSP file format. Written by [Will Fowler](https://github.com/wfowler1).

# Important Concepts

A BSP file is essentially a database of all the different elements that make up the game world. At its most basic level, this includes vertices that are used for drawing polygons and planes which are used for collision detection; other elements make use of these more basic ones to build up complex structures utilised by the game for various purposes (see the next section).

A key concept of a BSP file is the **lump**. This is analogous to a table in a database: most of the time a lump is comprised of one or more items with the same structure, like a database table is comprised of records (rows) with attributes (columns). The exception to this is for binary data - for example, entity properties described using ASCII text, or visibility data used to specify which sections of the map can "see" which other sections. The beginning offsets of all lumps in the file, along with their respective lengths in bytes, are given by the BSP header.

A property on a lump item can refer to another item in a different lump by index. The data in the item itself usually doesn't specify *which* lump the index should be used with - it is up to whatever program is reading the BSP to know which lump to index into. For example, the Half-Life `Edge` lump contains items comprised of two plain integers, which are indices into the `Vertices` lump and specify the vertices that the edge links together.

An index can also be paired together with a count, to reference a contiguous block of items in another lump. This is what Half-Life brush models do: they specify an index into the `Surfaces` lump, and a count of how many surfaces the brush model uses.

Note, however, that if a lump item wishes to reference a number of other items that are *not* contiguous, it must use another lump for indirection. An example of this would be a surface in Half-Life: multiple surfaces may share edges, so a surface cannot rely on all the edges that it requires being laid out consecutively in the `Edges` lump. Instead, the surfaces references a contiguous block in the `SurfEdges` lump. The sole purpose of items in this lump is to refer to individual edges by index; a block of surfedges then acts as the list of edges that the surface uses.

# Elements of a Nightfire BSP

The Nightfire BSP format is a little more complicated than Half-Life's format. The overall structure of the map is given by the `Models` lump: item 0 is the world itself, and other items represent brush models that exist in the world (eg. doors, triggers, breakables, etc.). Note that these are not the entities themselves - they are simply the visual representations made out of brushes.

Interestingly, it is not just the world that makes reference to leaves - models also do so. Non-world leaves can be distiguished from world leaves by having an invalid visibility offset of `0xffffffff` (or -1). In addition to the standard surfaces, Nightfire BSPs also include data about individual brushes and their surfaces.

Nightfire surfaces make use of "draw indices" instead of edges - surfaces directly specify the vertices they use in triplets, to form triangles. Surfaces are also used directly for collision testing, rather than separate "clip node" trees.

# Nightfire BSP File Structure

Data is in little-endian format.

## Header

Contains the BSP version, plus data about the offsets and lengths of all the other lumps in the file. Offsets are specified from the beginning of the file.

```c
// Length: 8

struct Lump
{
  int Offset;
  int Length;
}

// Length: 148

struct Header
{
  int Version;          // For Nightfire BSPs, this is 42
  Lump Entities;        // Lump 0
  Lump Planes;          // Lump 1
  Lump Textures;        // Lump 2
  Lump Materials;       // Lump 3
  Lump Vertices;        // Lump 4
  Lump ZeroVertices;    // Lump 5
  Lump DrawIndices;     // Lump 6
  Lump Visibility;      // Lump 7
  Lump Nodes;           // Lump 8
  Lump Surfaces;        // Lump 9
  Lump Lighting;        // Lump 10
  Lump Leaves;          // Lump 11
  Lump LeafSurfaces;    // Lump 12
  Lump LeafBrushes;     // Lump 13
  Lump Models;          // Lump 14
  Lump Brushes;         // Lump 15
  Lump BrushSides;      // Lump 16
  Lump Projections;     // Lump 17
}
```

## 0: Entities

This lump contains raw ASCII data and can be treated as a string, as per the length specified in the header. It is comprised of multiple keyvalue nodes, where the keyvalues for each entity are enclosed within a set of curly braces. For example:

```
{
"overbright" "192"
"MissionName" "austria"
"angles" "0 0 0"
"startdark" "1"
"sounds" "2"
"skycloudhigh_height" "1000"
"skycloudhigh_speed" "0.035  0.0029"
"skycloudlow_height" "800"
"skycloudlow_speed" "0.025  -0.0019"
"skyterrain" "Sky_terrain_Austria"
"skycloudlow" "Sky_lower_Austria"
"skydome" "Sky_dome_Austria"
"bsp_options" "-maxnodesize 2048"
"message" "M1_AUSTRIA04"
"MaxRange" "32767"
"skyname" "night_winter_"
"radfile" "m1_austria04.rad"
"rad_options" "-noslide -bounce 1 -chop 128 -smooth 75 -extra"
"vis_options" "-full"
"mapversion" "510"
"classname" "worldspawn"
}
{
"origin" "-112 4208 -96"
"_falloff" "2"
"angles" "-65 300 0"
"_light" "61 70 114 24"
"classname" "light_environment"
}

...
```

## 1: Planes

Planes correspond to world splits as computed by the BSP compilation stage of the map. They are used by `Nodes` partition the world space in a BSP tree, and by `Surfaces` (and other items that use these) for things like collision and hitscan testing.

The plane `Type` specifies which axis the normal lies along (or is closest to):
* Values 0-2 specify that the plane is perfectly perpendicular to the X, Y or Z axis respectively.
* Values 3-5 are similar, but mean that the plane is not exactly aligned perpendicular to an axis. A value of 3 means that the X axis is the largest magnitude component of the normal, and values 4 and 5 mean the same for Y and Z respectively.

Planes are specified in [Hesse normal form](https://en.wikipedia.org/wiki/Hesse_normal_form), meaning they are described by a normal vector and a perpendicular distance from the origin. The point `Distance * Normal` lies on the plane, as do all other points `p` where `dot(p, Normal) - Distance == 0.`

When leaves and brushes make reference to planes, they follow the convention that the plane normal points out of the volume concerned.

```c
// Length: 20

struct Plane
{
  float Normal[3];
  float Distance;
  uint32_t Type;
}
```

## 2: Textures

This lump holds texture path strings, relative to the Nightfire `textures` directory. It is unknown whether a null-terminating byte is always included, so do not assume that one will be present.

```c
// Length: 64

struct Texture
{
  char[64] Path;
}
```

## 3: Materials

This lump holds strings that specify rendering effects, eg. `wld_lightmap` or `wld_fullbright`. These are used by Nightfire to render things like envmapped glass, or other special effects. The path is relative to Nightfire's `materials` directory.

```c
// Length: 64

struct Material
{
  char[64] Path;
}
```

## 4: Vertices

Points in 3D space. Used for constructing faces in the map.

```c
// Length: 12

struct Vertex
{
  float Point[3];
}
```

## 5: Zero Vertices

Matches `Vertices` in length but it is filled with null bytes. Reasons for its existence are currently unknown. My best guess is that it was originally intended to be used for something like vertex normals, but the feature never made it into the game.

```c
// Length: 12

struct ZeroVertex
{
  float Point[3];
}
```

## 6: Draw Indices

These are referenced by items in the `Surfaces` lump, corresponding to the `DrawIndex` property held by the item. Once a surface has retrieved its list of vertices from the `Vertices` lump, the draw indices will index this list, with index 0 pointing to the first item. In order, the vertices indexed will build the trianges that form the surface.

```c
// Length: 4

struct DrawIndex
{
  uint32_t TriangleCorner;
}
```

## 7: Visibility

For detailed information on how to interpret the visibility data format in traditional BSP files, see the **Quake/Half-Life Visibility Data Format** section.

The Nightfire visibility data is in a slightly different format to that described in the above section. The data is not run-length encoded, which makes interpreting it easier, but the size of each "record" is not necessarily minimal (ie. the floor of (numLeaves + 7) / 8). The size can be deduced by checking the lowest non-zero visibility offset in the leaves lump, as it does appear that all records are the same size. The extra padding on each record always appears to be zeroed, and it is suspected (but not confirmed) that rows are padded to be 4-byte aligned.

There is **no** vis record corresponding to leaf 0, and similarly **no** bits in any vis record represent visibility to leaf 0. This means that bit `n` represents whether or not the given leaf can see leaf `n+1`.

In the `Leaves` lump, leaf 0 is given a vis offset of zero. This should be ignored and treated as invalid.

## 8: Nodes

Each node represents one level in the BSP tree, corresponding to a planar split. It has two children that correspond to volumes either side of the split (if a node didn't have children it would be a leaf, and would belong in the `Leaves` lump).

The bounds here are assumed to encompass all children below this node in the tree.

Note that as in Half-Life, negative child indices indicate leaves rather than nodes. If an index `n` is negative, it refers to leaf `-(n+1)` (or equivalently, `-n - 1`).

```c
// Length: 36

struct Node
{
  uint32_t PlaneIndex; // Index into Planes
  int32_t Children[2]; // Index into Nodes or Leaves
  float Mins[3];
  float Maxs[3];
}
```

## 9: Surfaces

Describes an actual face (comprised of triangles) that is drawn in the map. `VertexIndex` indexes into the `Vertices` lump, and `DrawIndex` indexes into the `Draw Indices` lump. Each draw index is an index into the collection of vertices used by each item, between 0 and `VertexCount - 1` inclusive.

It is assumed that all triangles on a surface lie exactly on the surface's plane, though it is not clear whether this is strictly required.

As a note, a surface has `3 * numLightStyles * X` bytes of lightmap data. `X` depends on the lightmap density of the surface, and corresponds to the number of lightmap cells present. **TODO:** *Perhaps provide a formula for this, when known? Is something to do with the surface extents.*

Note that unlike Half Life BSPs, Nightfire surfaces have independent projections for their texture and lightmap. This also means that multiple faces can reference the same projection, and that in theory the same projection could be used for both a texture and a lightmap. See the `Projections` lump for more information.

If there is no lightmap data in the file, the lighting offset will be -1.

```c
// Length: 48

struct Surface
{
  uint32_t PlaneIndex;              // Index into Planes
  uint32_t VerticesIndex;           // Index into Vertices
  uint32_t VerticesCount;           // Count for Vertices block
  uint32_t DrawIndicesIndex;        // Index into DrawIndices
  uint32_t DrawIndicesCount;        // Count for DrawIndices block
  uint32_t Flags;
  uint32_t TextureIndex;            // Index into Textures
  uint32_t MaterialIndex;           // Index into Materials
  uint32_t TextureProjectionIndex;  // Index into Projections
  uint32_t LightmapProjectionIndex; // Index into Projections
  uint8_t LightStyles[4];
  int32_t LightingOffset;           // Offset into Lighting
}
```

## 10: Lighting

Lighting data is essentially an RGB bitmap. It is indexed by `Surfaces` via their `LightingOffset` member, and beginning from this offset the face's lightmaps are stored consecutively. The number of lightmaps corresponds to the number of valid light styles the face has, and the dimensions of each lightmap are the same.

**TODO:** *Need more information on how to calculate the dimensions of a lightmap. The calculations are related to the lightmap projection used by the face, and the real-world extents of the face itself.*

```c
// Length: 3

struct Lighting
{
  uint8_t RGB[3];
}
```

## 11: Leaves

This is a leaf in the BSP tree - a volume that is not subdivided further, so has no children. Each leaf references `Leaf Surfaces` and `Leaf Brushes`.

The leaf type in Nightfire appears to only ever be 1 or 2. 1 is an empty leaf, where the player can enter the leaf and where surfaces may be referenced for drawing. 2 is a solid leaf, which references no surfaces.

Some leaves correspond to brush model volumes rather than BSP tree volumes, and as such are not used for visibility tests. In this case, the visibility offset will be `0xffffffff` (or -1).

```c
// Length: 48

struct Leaf
{
  uint32_t Type;
  int32_t VisibilityOffset;   // Offset into Visibility
  float Mins[3];
  float Maxs[3];
  uint32_t LeafSurfacesIndex; // Index into LeafSurfaces
  uint32_t LeafSurfacesCount; // Count for LeafSurfaces block
  uint32_t LeafBrushesIndex;  // Index into LeafBrushes
  uint32_t LeafBrushesCount;  // Count for LeafBrushes block
}
```

## 12: Leaf Surfaces

Indexes into the `Surfaces` lump. Each leaf references a block of leaf surfaces, which when read as a list provide all the surfaces that should be rendered from within the leaf.

```c
// Length: 4

struct LeafSurface
{
  uint32_t SurfaceIndex; // Index into Surfaces
}
```

## 13: Leaf Brushes

Indexed by `Leaves`. Each item indexes into the `Brushes` lump.

```c
// Length: 4

struct LeafBrush
{
  uint32_t BrushIndex; // Index into Brushes
}
```

## 14: Models

Defines a brush model within the map. The surfaces of the brush model must be consecutive in the surfaces lump, as unlike leaves there is no indirection lump to allow lists of random surfaces to be specified. This is OK - since brush models do not block visibility or form part of the immutable BSP tree, they do not need to adhere to the same structure as BSP surfaces.

Model 0 is the world; other models are brush entities.

Based on the Half Life map format, the array of 4 integers in the middle of the `Model` structure is suspected to be hulls. Hull indices in Nightfire maps are apparently always zero - this is probably because they used to point to clip node trees, which are no longer part of the file.

Unlike Half Life, Nightfire brush models also include extra information about the `Brushes` they reference (see `Brushes` and `BrushSides`). This information includes attributes about each of the brushes (**TODO:** *what actually are these?*), as well as texture indices and plane indices for each of the sides of a brush.

```c
// Length: 56

struct Model
{
  float Mins[3];
  float Maxs[3];
  uint32_t Hulls[4];
  uint32_t LeavesIndex;   // Index into Leaves
  uint32_t LeavesCount;   // Count for Leaves block
  uint32_t SurfacesIndex; // Index into Surfaces
  uint32_t SurfacesCount; // Count for Surfaces block
}
```

## 15: Brushes

Defines a brush within the map, which is comprised of a list of sides.

```c
// Length: 12

struct Brush
{
  uint32_t Attributes;      // What are these?
  uint32_t BrushSidesIndex; // Index into BrushSides
  uint32_t BrushSidesCount; // Count for BrushSides block
}
```

## 16: BrushSides

Defines a side used by one or more brushes. Items in this lump were probably used by Nightfire as a convenient way of linking a specific group of surfaces to one brush volume.

```c
// Length: 8

struct BrushSide
{
  uint32_t SurfaceIndex; // Index into Surfaces
  uint32_t PlaneIndex;   // Index into Planes
}
```

## 17: Projections

Defines a projection onto a face. These are used for both textures and lightmaps, as Nightfire allows a surface's lightmap grid to be rotated and scaled.

The projection is defined by two four-component vectors S and T. The first three components specify the axis in 3D space, and the fourth component specifies the translation of the texture along this axis.

**TODO:** *Is this the texture translation or the face translation (in other words, which way is positive and which is negative)?*

```c
// Length: 32

struct Projection
{
  float S[4];
  float T[4];
}
```

# Indexing Lumps

```
Models         Index into: Leaves
                           Surfaces

Leaves         Index into: Leaf Surfaces
                           Leaf Brushes
                           Visibility

Brushes        Index into: Brush Sides

Leaf Surfaces  Index into: Surfaces

Leaf Brushes   Index into: Brushes

Brush Sides    Index into: Planes
                           Surfaces

Nodes          Index into: Nodes
                           Leaves
                           Planes

Surfaces       Index into: Planes
                           Draw Indices
                           Materials
                           Textures
                           Vertices
                           Lighting
                           Projections

Draw Indices   Index into: Face-specific vertex list

Visibility     Index into: Leaves (encoded)

Planes         Index into: None

Materials      Index into: None

Textures       Index into: None

Vertices       Index into: None

ZeroVertices   Index into: None

Lighting       Index into: None

Projections    Index into: None

Entities       Index into: None
```

# Comparison between GoldSrc (v30) and Nightfire (v42) BSP lumps

```
GoldSrc (v30)       Nightfire (v42) Equivalent

(0)  Entities       (0)  Entities
(1)  Planes         (1)  Planes
(2)  Textures       (2)  Textures
(3)  Vertices       (4)  Vertices
(4)  Visibility     (7)  Visibility
(5)  Nodes          (8)  Nodes
(6)  Tex Info       (17) Projections
(7)  Faces          (9)  Surfaces
(8)  Lighting       (10) Lighting
(9)  Clip Nodes     (*)
(10) Leaves         (11) Leaves
(11) Mark Surfaces  (12) Leaf Surfaces
(12) Edges          (*)
(13) Surf Edges     (*)
(14) Models         (14) Models
(*)                 (3)  Materials
(*)                 (5)  Zero Vertices
(*)                 (6)  Draw Indices
(*)                 (13) Leaf Brushes
(*)                 (15) Brushes
(*)                 (16) Brush Sides
```

# Quake/Half-Life Visibility Data Format

*Note: Nightfire BSPs do not run-length encode their visibility data.*

The `Visibility` lump has the most complex structure of all the lumps. Each leaf in the `Leaves` lump holds an offset into the `Visibility` lump, and the data beginning from this offset is a bit array concerning which other leaves the given leaf can see.

For a bit `n` in the bit array, the given leaf:

* Can see leaf `n` if bit `n` is 1.
* Cannot see leaf `n` if bit `n` is 0.

For example, this bit array of 8 leaves:

```
01100000
```

implies that our given leaf can only see two other leaves - 1 and 2 - out of all leaves 0-7.

It should be noted that for any given leaf `n`, bit `n` should always be 1, as a leaf can always see itself. (If it were 0, the leaf would not be drawn when the player stands inside it.)

Since every leaf needs to store a bit corresponding to every other leaf, there are `n^2` bits required to store information about `n` leaves. To help reduce the amount of space required to store the visibility data, the data is run-length encoded. This takes advantage of the assumption that, on the whole, any given leaf should only be able to see a small fraction of the total number of leaves in the map. Because of this, we should expect that the vast majority of bits in each leaf's bit array will be zero. If these zeroes are run-length encoded, the amount of space required to store the visibility data can be significantly reduced.

Because bytes are the smallest units of memory a computer can process at a time, run-length encoding only works for *zero bytes* - ie., bytes where all eight of their bits are zeroes. The rule is that, if a zero byte is encountered within the visibility data, the following byte will always specify the run length of zero bytes. This secondary byte will never itself be zero. This means that a run of up to 255 consecutive zero bytes can be encoded in the visibility data using just two bytes - one original zero, and one specifying the run length.

Note that if even one bit in a given byte is not zero, the data cannot be run-length encoded. For example, the following raw data:

```
         |          Run of 4 bytes         |
01100000 00000000 00000000 00000000 00000000 00000001 10000000 10011010
	(60 00 00 00 00 01 80 9A) - 8 bytes
```

is run-length encoded to:

```
         | Zero    Count                   |
01100000 00000000 00000100 -------- -------- 00000001 10000000 10011010
	(60 00 04 01 80 9A) - 6 bytes
```

The following pseudo-C++ demonstrates how to interpret the visibility data:

```c++
// Assuming these are all initialised:
unsigned char* visLumpData = ... ;	// Raw vis data read in.
int visLumpDataLength = ... ;		// Length of the data.
int totalLeaves = ... ;				// Total number of leaves in the map.
Leaf ourLeaf = ... ;				// The leaf we're processing.

// We begin from the offset specified by ourLeaf.
int dataOffset = ourLeaf.visibilityOffset;

// Let's assume that leafVisibility is an object to keep track
// of the visibility of all other leaves, from the point of view
// of ourLeaf.
// Eg. you could ask whether a given leaf is visible by calling:
//
//     bool visible = leafVisibility.isLeafVisible(leafNumber);

// By default, leaves should not be considered visible.
leafVisibility.setAllLeavesNotVisible();

// Walk over the bit array.
int leafToCheck = 0;
while (leafToCheck < totalLeaves && dataOffset < visLumpDataLength)
{
	// Is there a run of zero bytes?
	if (visLumpData[dataOffset] == 0)
	{
		// Make sure we don't overrun.
		// If the BSP is written properly, this shouldn't happen.
		if (dataOffset + 1 >= visLumpDataLength)
		{
			break;
		}

		// Find out from the next byte how long it is.
		unsigned char runLength = visLumpData[currentOffset + 1];

		// We know that there will be runLength * 8 bits that are zero
		// in this run of zero bytes, therefore we can skip that many
		// leaves and leave them as not visible. (Remember, we marked
		// all leaves as not visible before we began.)
		leafToCheck += runLength * 8;

		// Skip past the 2 bytes denoting the run.
		dataOffset += 2;

		// Go round again.
		continue;
	}

	// There's at least one bit in this byte that's not zero.
	// Process all bits and record visibility information.
	for (int bit = 0; bit < 8; ++bit)
	{
		// Create a mask for the bit we're checking.
		unsigned char mask = 1 << bit;

		// See if there's any data at that bit.
		if (visLumpData[dataOffset] & mask)
		{
			// Mark the current leaf as visible.
			leafVisibility.markLeafAsVisible(leafToCheck, true);
		}

		// Record that we checked the leaf.
		++leafToCheck;
	}
}
```
