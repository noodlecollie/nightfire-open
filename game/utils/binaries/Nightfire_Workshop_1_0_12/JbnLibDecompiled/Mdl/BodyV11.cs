// Decompiled with JetBrains decompiler
// Type: JbnLib.Mdl.BodyV11
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Interfaces;
using JbnLib.Shared;
using System;
using System.Collections.Generic;
using System.Text;

namespace JbnLib.Mdl
{
  public class BodyV11 : IStructure
  {
    private string _Name;
    private int _Unknown0;
    private int _Unknown1;
    private int _LevelOfDetailCount;
    private int _LevelOfDetailOffset;
    private int _LodBlendCount;
    private int _Unknown2;
    private int _UnknownOffset0;
    private int _LodBoneCount;
    private int _LodBoneOffset;
    private byte[] _LodBones;
    private int _LodNormOffset;
    private int _Unknown3;
    private int _Unknown4;
    private int _LodBlendOffset;
    private byte[] _LodBlends;
    private int _LodPositionOffset;
    private List<LevelOfDetailV11> _LevelOfDetailCollection = new List<LevelOfDetailV11>();
    private List<Point3D> _LodNormCollection = new List<Point3D>();
    private LodPosition[][] _LodPositionCollection;

    public static int Length => 120;

    public string Name
    {
      get => this._Name;
      set => this._Name = value;
    }

    public int Unknown0
    {
      get => this._Unknown0;
      set => this._Unknown0 = value;
    }

    public int Unknown1
    {
      get => this._Unknown1;
      set => this._Unknown1 = value;
    }

    public int LevelOfDetailCount
    {
      get => this._LevelOfDetailCount;
      set => this._LevelOfDetailCount = value;
    }

    public int LevelOfDetailOffset
    {
      get => this._LevelOfDetailOffset;
      set => this._LevelOfDetailOffset = value;
    }

    public int LodBlendCount
    {
      get => this._LodBlendCount;
      set => this._LodBlendCount = value;
    }

    public int Unknown2
    {
      get => this._Unknown2;
      set => this._Unknown2 = value;
    }

    public int UnknownOffset0
    {
      get => this._UnknownOffset0;
      set => this._UnknownOffset0 = value;
    }

    public int LodBoneCount
    {
      get => this._LodBoneCount;
      set => this._LodBoneCount = value;
    }

    public int LodBoneOffset
    {
      get => this._LodBoneOffset;
      set => this._LodBoneOffset = value;
    }

    public byte[] LodBones
    {
      get => this._LodBones;
      set => this._LodBones = value;
    }

    public int LodNormOffset
    {
      get => this._LodNormOffset;
      set => this._LodNormOffset = value;
    }

    public int Unknown3
    {
      get => this._Unknown3;
      set => this._Unknown3 = value;
    }

    public int Unknown4
    {
      get => this._Unknown4;
      set => this._Unknown4 = value;
    }

    public int LodBlendOffset
    {
      get => this._LodBlendOffset;
      set => this._LodBlendOffset = value;
    }

    public byte[] LodBlends
    {
      get => this._LodBlends;
      set => this._LodBlends = value;
    }

    public int LodPositionOffset
    {
      get => this._LodPositionOffset;
      set => this._LodPositionOffset = value;
    }

    public List<LevelOfDetailV11> LevelOfDetailCollection
    {
      get => this._LevelOfDetailCollection;
      set => this._LevelOfDetailCollection = value;
    }

    public List<Point3D> LodNormCollection
    {
      get => this._LodNormCollection;
      set => this._LodNormCollection = value;
    }

    public LodPosition[][] LodPositionCollection
    {
      get => this._LodPositionCollection;
      set => this._LodPositionCollection = value;
    }

    public BodyV11(byte[] raw)
    {
      this._Name = Encoding.ASCII.GetString(raw, 0, 64).Trim(new char[1]);
      this._Unknown0 = BitConverter.ToInt32(raw, 64);
      this._Unknown1 = BitConverter.ToInt32(raw, 68);
      this._LevelOfDetailCount = BitConverter.ToInt32(raw, 72);
      this._LevelOfDetailOffset = BitConverter.ToInt32(raw, 76);
      this._LodBlendCount = BitConverter.ToInt32(raw, 80);
      this._Unknown2 = BitConverter.ToInt32(raw, 84);
      this._UnknownOffset0 = BitConverter.ToInt32(raw, 88);
      this._LodBoneCount = BitConverter.ToInt32(raw, 92);
      this._LodBoneOffset = BitConverter.ToInt32(raw, 96);
      this._LodNormOffset = BitConverter.ToInt32(raw, 100);
      this._Unknown3 = BitConverter.ToInt32(raw, 104);
      this._Unknown4 = BitConverter.ToInt32(raw, 108);
      this._LodBlendOffset = BitConverter.ToInt32(raw, 112);
      this._LodPositionOffset = BitConverter.ToInt32(raw, 116);
    }

    public BodyV11(
      string name,
      int unknown0,
      int unknown1,
      int lodcount,
      int lodoffset,
      int lodblendcount,
      int unknown2,
      int unknownoffset0,
      int lodbonecount,
      int lodboneoffset,
      int lodnormoffset,
      int unknown3,
      int unknown4,
      int lodblendoffset,
      int lodpositionoffset)
    {
      this._Name = name;
      this._Unknown0 = unknown0;
      this._Unknown1 = unknown1;
      this._LevelOfDetailCount = lodcount;
      this._LevelOfDetailOffset = lodoffset;
      this._LodBlendCount = lodblendcount;
      this._Unknown2 = unknown2;
      this._UnknownOffset0 = unknownoffset0;
      this._LodBoneCount = lodbonecount;
      this._LodBoneOffset = lodboneoffset;
      this._LodNormOffset = lodnormoffset;
      this._Unknown3 = unknown3;
      this._Unknown4 = unknown4;
      this._LodBlendOffset = lodblendoffset;
      this._LodPositionOffset = lodpositionoffset;
    }

    public byte[] GetBytes()
    {
      byte[] bytes = new byte[BodyV11.Length];
      Encoding.ASCII.GetBytes(this._Name).CopyTo((Array) bytes, 0);
      BitConverter.GetBytes(this._Unknown0).CopyTo((Array) bytes, 64);
      BitConverter.GetBytes(this._Unknown1).CopyTo((Array) bytes, 68);
      BitConverter.GetBytes(this._LevelOfDetailCount).CopyTo((Array) bytes, 72);
      BitConverter.GetBytes(this._LevelOfDetailOffset).CopyTo((Array) bytes, 76);
      BitConverter.GetBytes(this._LodBlendCount).CopyTo((Array) bytes, 80);
      BitConverter.GetBytes(this._Unknown2).CopyTo((Array) bytes, 84);
      BitConverter.GetBytes(this._UnknownOffset0).CopyTo((Array) bytes, 88);
      BitConverter.GetBytes(this._LodBoneCount).CopyTo((Array) bytes, 92);
      BitConverter.GetBytes(this._LodBoneOffset).CopyTo((Array) bytes, 96);
      BitConverter.GetBytes(this._LodNormOffset).CopyTo((Array) bytes, 100);
      BitConverter.GetBytes(this._Unknown3).CopyTo((Array) bytes, 104);
      BitConverter.GetBytes(this._Unknown4).CopyTo((Array) bytes, 108);
      BitConverter.GetBytes(this._LodBlendOffset).CopyTo((Array) bytes, 112);
      BitConverter.GetBytes(this._LodPositionOffset).CopyTo((Array) bytes, 116);
      return bytes;
    }

    public new string ToString() => this._Name;
  }
}
