// Decompiled with JetBrains decompiler
// Type: JbnLib.Mdl.LevelOfDetailV11
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Interfaces;
using System;
using System.Collections.Generic;

namespace JbnLib.Mdl
{
  public class LevelOfDetailV11 : IStructure
  {
    private int _UnknownCount0;
    private int _LodVerticesOffset;
    private int _LodBoneIndex;
    private int _LodBoneCount;
    private int _Unknown1;
    private List<LodVertices> _LodVerticesCollection = new List<LodVertices>();

    public static int Length => 20;

    public int UnknownCount0
    {
      get => this._UnknownCount0;
      set => this._UnknownCount0 = value;
    }

    public int LodVerticesOffset
    {
      get => this._LodVerticesOffset;
      set => this._LodVerticesOffset = value;
    }

    public int LodBoneIndex
    {
      get => this._LodBoneIndex;
      set => this._LodBoneIndex = value;
    }

    public int LodBoneCount
    {
      get => this._LodBoneCount;
      set => this._LodBoneCount = value;
    }

    public int Unknown1
    {
      get => this._Unknown1;
      set => this._Unknown1 = value;
    }

    public List<LodVertices> LodVerticesCollection
    {
      get => this._LodVerticesCollection;
      set => this._LodVerticesCollection = value;
    }

    public LevelOfDetailV11(byte[] raw)
    {
      this._UnknownCount0 = BitConverter.ToInt32(raw, 0);
      this._LodVerticesOffset = BitConverter.ToInt32(raw, 4);
      this._LodBoneIndex = BitConverter.ToInt32(raw, 8);
      this._LodBoneCount = BitConverter.ToInt32(raw, 12);
      this._Unknown1 = BitConverter.ToInt32(raw, 16);
    }

    public LevelOfDetailV11(
      int unknowncount0,
      int lodverticesoffset,
      int lodboneindex,
      int lodbonecount,
      int unknown1)
    {
      this._UnknownCount0 = unknowncount0;
      this._LodVerticesOffset = lodverticesoffset;
      this._LodBoneIndex = lodboneindex;
      this._LodBoneCount = lodbonecount;
      this._Unknown1 = unknown1;
    }

    public byte[] GetBytes()
    {
      byte[] bytes = new byte[LevelOfDetailV11.Length];
      BitConverter.GetBytes(this._UnknownCount0).CopyTo((Array) bytes, 0);
      BitConverter.GetBytes(this._LodVerticesOffset).CopyTo((Array) bytes, 4);
      BitConverter.GetBytes(this._LodBoneIndex).CopyTo((Array) bytes, 8);
      BitConverter.GetBytes(this._LodBoneCount).CopyTo((Array) bytes, 12);
      BitConverter.GetBytes(this._Unknown1).CopyTo((Array) bytes, 16);
      return bytes;
    }
  }
}
