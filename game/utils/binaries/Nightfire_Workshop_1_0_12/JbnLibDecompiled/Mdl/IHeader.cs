// Decompiled with JetBrains decompiler
// Type: JbnLib.Mdl.IHeader
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Interfaces;
using JbnLib.Shared;

namespace JbnLib.Mdl
{
  public interface IHeader : IStructure
  {
    string Identifier { get; set; }

    int Version { get; set; }

    string Name { get; set; }

    int FileLength { get; set; }

    EyePosition EyePosition { get; set; }

    Point3D BoundingBoxMin { get; set; }

    Point3D BoundingBoxMax { get; set; }

    Point3D ClippingBoxMin { get; set; }

    Point3D ClippingBoxMax { get; set; }

    TypeFlag TypeFlag { get; set; }

    CountOffsetPair Bones { get; set; }

    CountOffsetPair BoneControllers { get; set; }

    CountOffsetPair HitBoxes { get; set; }

    CountOffsetPair Sequences { get; set; }

    CountOffsetPair SequenceGroups { get; set; }

    CountOffsetPair Textures { get; set; }

    int TextureInfo { get; set; }

    int SkinReferences { get; set; }

    int SkinFamilies { get; set; }

    int SkinsOffset { get; set; }

    CountOffsetPair BodyGroups { get; set; }

    CountOffsetPair Attachments { get; set; }

    CountOffsetPair SoundGroups { get; set; }

    int SoundsOffset { get; set; }

    int TransitionsCount { get; set; }

    int TransitionFlagsOffset { get; set; }

    int TransitionsOffset { get; set; }
  }
}
