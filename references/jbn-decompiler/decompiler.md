*The following was taken from the [JBN BSP Lump Tools](https://code.google.com/archive/p/jbn-bsp-lump-tools/) repository.*

# Introduction

The decompiler is without a doubt one of the most complex things I have ever coded, mostly because of the maze of BSP data I had to sort though to get the information I needed.


# Decompiling

As with most things, there's more than one way to write a BSP decompiler. I've seen people do all kinds of things to recreate the information that the BSP compilers have mashed into an optimized pile of solids. I feel the process I've developed is the most efficient and accurate way of decompiling a BSP. It involves a lot of repetitive tasks and looping code.

## Entities
In the .MAP file, prior to compilation, everything is handled as an entity. Many of the entities (namely point-based ones) are copied almost directly into the Entities lump (Lump00) of the BSP. All brush-based entities have their Brushes compiled, grouped together and referenced by a number ("model" attribute), with the brushes contained in the worldspawn entity in model 0. Knowing this, it was clear I had to start by reading the entities lump first.

For each entity, the decompiler looks at the model number which the entity references (worldspawn is assumed to reference 0). If the entity doesn't reference any model (or the model is an MDL file), the entity is simply copied into the .MAP file. If the entity does reference a model, the solid information is first inserted into the entity (and the "model" attribute deleted), which is then also copied into the .MAP file. If the entity references a model and has an "origin" attribute, then every brush in the entity must be shifted by that "origin" by adding the value of "origin" to all points used to define the brush. In addition, a special origin brush must be created at the coordinates specified by the "origin" attribute. The decompiler does this as a 32x32x32 block.

A point-based entity will look like this:

```
{ // Entity #
... Attributes ...
"classname" "MyPointEnt"
}
```

Whereas a brush-based entity with its brushes will look like this:

```
{ // Entity #
... Attributes ...
"classname" "MyBrushEnt"
{ // Brush #1
... Surfaces ...
}
{ // Brush #2
... Surfaces ...
}
... More brushes ...
}
```

## Leaves
If the entity does reference a model, then we must look at the corresponding model in Lump14. Each model references a number of leaves starting at a specific leaf, and for those we need to look at Lump11.

For each leaf referenced, we need to grab its brush information. The lump references brushes through a middleman lump called Mark Brushes (Lump13), which is essentially a list of references into Lump15. Each leaf references a number of mark brushes starting from a specific one. Sometimes a leaf references no brushes, in which case it can be skipped. Sometimes several leaves in the same model reference a brush multiple times. Therefore it is necessary to maintain a running list of brushes which have already been decompiled in the current model to ensure no brush is decompiled twice.

## Brushes
If the leaf does reference mark brushes, then we must find the brushes (Lump15) referenced by the mark brushes (Lump13). Through this maze of data, we have finally arrived at the brushes, which are the actual solid pieces of the map. These are the structures which will actually be written into the entities which used them.

For each brush, we need to grab its surface info. This is done through the Brush Sides (Lump16) lump. Brush sides are referenced by the amount of sides on a brush, starting with a specific side. We also need to mark this brush as used in the list of brushes used for this model. Once all the side information is gathered the brush can be written to the current entity.

A brush looks like this:

```
{ // Brush #
... Surfaces ...
}
```

And is inserted directly into the entity.

## Surfaces
We must now reconstruct the sufaces making up the brush from scratch. This is very easy if the surface (Lump09) references Vertex (Lump04) data. The face could reference any number of vertices, but since we're only trying to reconstruct a the plane, we only need three of them. If you grab the first three vertices in order, it will create a valid plane for your brush every time. However, if the surface does not reference vertices you must recreate the plane points from the plane defined in the Planes lump (Lump01). These points do not have to be vertices of the brush, and they do not have to be on the brush, they simply have to properly define the plane.

For each surface, we must grab the vertices (if it has them) and put them in order, and if it doesn't have them we must find points on the referenced plane instead. We must also grab its Texture (Lump02), Material (Lump03), and Texture Scaling information (Lump17). Also, sometimes planes are referenced backward, and to the game engine it doesn't matter. However, it matters to the map editor. Therefore, the plane must be flipped if necessary, which is easily done by changing the order of the points.

Texture scaling information needs to be manipulated a little bit. You CAN output it as it is, and it will work in the map editor. However, if this step is not taken, texture scale values will not appear properly in the map editor, although *textures will still have the correct scaling anyway*. First, you must find the lengths of the U and V vectors, and these are the proper scale values and should be used in your output surface. Then you must divide the U and V vectors by their lengths to make their lengths 1. These are the values you write to the output surface as the U and V axes.

Also, if the current entity has an "origin" attribute, the dot product of point defined by origin must be and the U and V components of the texture scale must be found, then this dot product must be divided by the texture scale for the relevant axis. This result must then be _subtracted from_ the shift. Otherwise, the texture shifting on any object with an "origin" attribute will be wrong.

A surface in the .MAP will look something like this:

```
( First point ) ( Second point ) ( Third point ) TextureName [ U axis components and U axis shift ] [ V axis components and V axis shift ] 0 Uscale Vscale Flags MaterialName [ Lightmap scale and rotation ]
```

## Required Lumps
The lumps required by this process are:
  * 00 - Entities
  * 01 - Planes
  * 02 - Textures
  * 03 - Materials
  * 04 - Vertices
  * 06 - Draw indices (optional)
  * 09 - Surfaces
  * 11 - Leaves
  * 13 - Mark Brushes
  * 14 - Models
  * 15 - Brushes
  * 16 - Brush Sides
  * 17 - Texture Scaling Matrix

## Path of references
Entities --> Models --> Leaves --> Mark Brushes --> Brushes --> Brush Sides --> Surfaces --> Planes, Vertices, Draw Indices, Textures, Materials, and Texture Scale Matrices

# Alternative Method
I have not tried this (yet) but there may be an alternative method to decompiling, at least to recreate the _appearance_ of the map. Entities should be handled like they are above. However, models also reference surfaces as well as brushes. Each surface could be decompiled into its own brush. This would create a map that isn't very faithful to the original creation, but may look almost exactly identical to it. However, this would generate a brush for every side of a box, for example. It is an extremely inefficient idea.

## Required Lumps
The lumps required by this process would be:
  * 00 - Entities
  * 02 - Textures
  * 03 - Materials
  * 04 - Vertices
  * 06 - Draw indices (optional)
  * 09 - Surfaces
  * 14 - Models
  * 17 - Texture Scaling Matrix

## Path of references
Entities --> Models --> Surfaces --> Vertices, Draw Indices, Textures, Materials, and Texture Scale Matrices

# Known Issues

* Any clip variants (player, clip, NPC clip, etc.) are treated as normal clip.

# See Also

[NFBSP BSP v42 documentation](nfbsp-bspv42.md)
