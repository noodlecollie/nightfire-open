// Decompiled with JetBrains decompiler
// Type: JbnLib.Mdl.MeshV14
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Interfaces;
using System;

namespace JbnLib.Mdl
{
  public class MeshV14 : IStructure
  {
    private sbyte[] _BoneMappings = new sbyte[24]
    {
      (sbyte) -1,
      (sbyte) -1,
      (sbyte) -1,
      (sbyte) -1,
      (sbyte) -1,
      (sbyte) -1,
      (sbyte) -1,
      (sbyte) -1,
      (sbyte) -1,
      (sbyte) -1,
      (sbyte) -1,
      (sbyte) -1,
      (sbyte) -1,
      (sbyte) -1,
      (sbyte) -1,
      (sbyte) -1,
      (sbyte) -1,
      (sbyte) -1,
      (sbyte) -1,
      (sbyte) -1,
      (sbyte) -1,
      (sbyte) -1,
      (sbyte) -1,
      (sbyte) -1
    };
    private ushort _TriangleIndex;
    private ushort _TriangleCount;
    private ushort _VertexIndexFrom;
    private ushort _VertexIndexTo;

    public static int Length => 32;

    public sbyte[] BoneMappings
    {
      get => this._BoneMappings;
      set => this._BoneMappings = value;
    }

    public ushort TriangleIndex
    {
      get => this._TriangleIndex;
      set => this._TriangleIndex = value;
    }

    public ushort TriangleCount
    {
      get => this._TriangleCount;
      set => this._TriangleCount = value;
    }

    public ushort VertexIndexFrom
    {
      get => this._VertexIndexFrom;
      set => this._VertexIndexFrom = value;
    }

    public ushort VertexIndexTo
    {
      get => this._VertexIndexTo;
      set => this._VertexIndexTo = value;
    }

    public MeshV14(byte[] raw)
    {
      for (byte index = 0; (int) index < this._BoneMappings.Length; ++index)
        this._BoneMappings[(int) index] = (sbyte) raw[(int) index];
      this._TriangleIndex = BitConverter.ToUInt16(raw, 24);
      this._TriangleCount = BitConverter.ToUInt16(raw, 26);
      this._VertexIndexFrom = BitConverter.ToUInt16(raw, 28);
      this._VertexIndexTo = BitConverter.ToUInt16(raw, 30);
    }

    public MeshV14()
    {
    }

    public MeshV14(sbyte[] bonemappings, ushort triangleindex, ushort trianglecount)
    {
      this._BoneMappings = bonemappings;
      this._TriangleIndex = triangleindex;
      this._TriangleCount = trianglecount;
      this._VertexIndexFrom = (ushort) 0;
      this._VertexIndexTo = (ushort) 0;
    }

    public MeshV14(
      sbyte[] bonemappings,
      ushort triangleindex,
      ushort trianglecount,
      ushort vertexindexfrom,
      ushort vertexindexto)
    {
      this._BoneMappings = bonemappings;
      this._TriangleIndex = triangleindex;
      this._TriangleCount = trianglecount;
      this._VertexIndexFrom = vertexindexfrom;
      this._VertexIndexTo = vertexindexto;
    }

    public byte[] GetBytes()
    {
      byte[] bytes = new byte[MeshV14.Length];
      for (byte index = 0; (int) index < this._BoneMappings.Length; ++index)
        BitConverter.GetBytes((short) this._BoneMappings[(int) index]).CopyTo((Array) bytes, (int) index);
      BitConverter.GetBytes(this._TriangleIndex).CopyTo((Array) bytes, 24);
      BitConverter.GetBytes(this._TriangleCount).CopyTo((Array) bytes, 26);
      BitConverter.GetBytes(this._VertexIndexFrom).CopyTo((Array) bytes, 28);
      BitConverter.GetBytes(this._VertexIndexTo).CopyTo((Array) bytes, 30);
      return bytes;
    }

    public new string ToString()
    {
      byte num = 0;
      for (byte index = 0; (int) index < this._BoneMappings.Length; ++index)
      {
        if (this._BoneMappings[(int) index] != (sbyte) -1)
          ++num;
      }
      return num.ToString() + ", " + (object) this._TriangleIndex + ", " + (object) this._TriangleCount;
    }
  }
}
