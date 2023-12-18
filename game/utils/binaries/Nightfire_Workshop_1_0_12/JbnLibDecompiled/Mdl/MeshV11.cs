// Decompiled with JetBrains decompiler
// Type: JbnLib.Mdl.MeshV11
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Interfaces;
using System;

namespace JbnLib.Mdl
{
  public class MeshV11 : IStructure
  {
    private sbyte[] _BoneMappings = new sbyte[16];
    private ushort _TriangleIndex;
    private ushort _TriangleCount;

    public static int Length => 20;

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

    public MeshV11()
    {
    }

    public MeshV11(byte[] raw)
    {
      for (byte index = 0; (int) index < this._BoneMappings.Length; ++index)
        this._BoneMappings[(int) index] = (sbyte) raw[(int) index];
      this._TriangleIndex = BitConverter.ToUInt16(raw, 16);
      this._TriangleCount = BitConverter.ToUInt16(raw, 18);
    }

    public MeshV11(sbyte[] bonemappings, ushort triangleindex, ushort trianglecount)
    {
      this._BoneMappings = bonemappings;
      this._TriangleIndex = triangleindex;
      this._TriangleCount = trianglecount;
    }

    public byte[] GetBytes()
    {
      byte[] bytes = new byte[MeshV11.Length];
      for (byte index = 0; (int) index < this._BoneMappings.Length; ++index)
        BitConverter.GetBytes((short) this._BoneMappings[(int) index]).CopyTo((Array) bytes, (int) index);
      BitConverter.GetBytes(this._TriangleIndex).CopyTo((Array) bytes, 16);
      BitConverter.GetBytes(this._TriangleCount).CopyTo((Array) bytes, 18);
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
