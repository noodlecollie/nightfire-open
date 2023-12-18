// Decompiled with JetBrains decompiler
// Type: JbnLib.Mdl.Header
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Interfaces;
using JbnLib.Lang;
using JbnLib.Shared;

namespace JbnLib.Mdl
{
  public class Header : IHeader, IStructure
  {
    internal string _Identifier;
    internal int _Version;
    internal string _Name;
    internal int _Length;
    internal EyePosition _EyePosition;
    internal Point3D _BoundingBoxMin;
    internal Point3D _BoundingBoxMax;
    internal Point3D _ClippingBoxMin;
    internal Point3D _ClippingBoxMax;
    internal TypeFlag _TypeFlag;
    internal CountOffsetPair _Bones;
    internal CountOffsetPair _BoneControllers;
    internal CountOffsetPair _HitBoxes;
    internal CountOffsetPair _Sequences;
    internal CountOffsetPair _SequenceGroups;
    internal CountOffsetPair _Textures;
    internal int _TextureInfo;
    internal int _SkinReferences;
    internal int _SkinFamilies;
    internal int _SkinsOffset;
    internal CountOffsetPair _BodyGroups;
    internal CountOffsetPair _Attachments;
    internal CountOffsetPair _SoundGroups;
    internal int _SoundsOffset;
    internal int _TransitionsCount;
    internal int _TransitionFlagsOffset;
    internal int _TransitionsOffset;
    internal LevelOfDetailFlags _LevelOfDetailFlags;

    public static int Length => 248;

    public string Identifier
    {
      get => this._Identifier;
      set => this._Identifier = value;
    }

    public int Version
    {
      get => this._Version;
      set => this._Version = value;
    }

    public string Name
    {
      get => this._Name;
      set => this._Name = value;
    }

    public int FileLength
    {
      get => this._Length;
      set => this._Length = value;
    }

    public EyePosition EyePosition
    {
      get => this._EyePosition;
      set => this._EyePosition = value;
    }

    public Point3D BoundingBoxMin
    {
      get => this._BoundingBoxMin;
      set => this._BoundingBoxMin = value;
    }

    public Point3D BoundingBoxMax
    {
      get => this._BoundingBoxMax;
      set => this._BoundingBoxMax = value;
    }

    public Point3D ClippingBoxMin
    {
      get => this._ClippingBoxMin;
      set => this._ClippingBoxMin = value;
    }

    public Point3D ClippingBoxMax
    {
      get => this._ClippingBoxMax;
      set => this._ClippingBoxMax = value;
    }

    public TypeFlag TypeFlag
    {
      get => this._TypeFlag;
      set => this._TypeFlag = value;
    }

    public CountOffsetPair Bones
    {
      get => this._Bones;
      set => this._Bones = value;
    }

    public CountOffsetPair BoneControllers
    {
      get => this._BoneControllers;
      set => this._BoneControllers = value;
    }

    public CountOffsetPair HitBoxes
    {
      get => this._HitBoxes;
      set => this._HitBoxes = value;
    }

    public CountOffsetPair Sequences
    {
      get => this._Sequences;
      set => this._Sequences = value;
    }

    public CountOffsetPair SequenceGroups
    {
      get => this._SequenceGroups;
      set => this._SequenceGroups = value;
    }

    public CountOffsetPair Textures
    {
      get => this._Textures;
      set => this._Textures = value;
    }

    public int TextureInfo
    {
      get => this._TextureInfo;
      set => this._TextureInfo = value;
    }

    public int SkinReferences
    {
      get => this._SkinReferences;
      set => this._SkinReferences = value;
    }

    public int SkinFamilies
    {
      get => this._SkinFamilies;
      set => this._SkinFamilies = value;
    }

    public int SkinsOffset
    {
      get => this._SkinsOffset;
      set => this._SkinsOffset = value;
    }

    public CountOffsetPair BodyGroups
    {
      get => this._BodyGroups;
      set => this._BodyGroups = value;
    }

    public CountOffsetPair Attachments
    {
      get => this._Attachments;
      set => this._Attachments = value;
    }

    public CountOffsetPair SoundGroups
    {
      get => this._SoundGroups;
      set => this._SoundGroups = value;
    }

    public int SoundsOffset
    {
      get => this._SoundsOffset;
      set => this._SoundsOffset = value;
    }

    public int TransitionsCount
    {
      get => this._TransitionsCount;
      set => this._TransitionsCount = value;
    }

    public int TransitionFlagsOffset
    {
      get => this._TransitionFlagsOffset;
      set => this._TransitionFlagsOffset = value;
    }

    public int TransitionsOffset
    {
      get => this._TransitionsOffset;
      set => this._TransitionsOffset = value;
    }

    public LevelOfDetailFlags LevelOfDetailFlags
    {
      get => this._LevelOfDetailFlags;
      set => this._LevelOfDetailFlags = value;
    }

    public virtual byte[] GetBytes()
    {
      Messages.ThrowException("Mdl.Header.GetBytes()", Messages.METHOD_OVERRIDE);
      return new byte[0];
    }

    public new string ToString() => this._Name;
  }
}
