// Decompiled with JetBrains decompiler
// Type: JbnLib.Mdl.TriangleMap
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Interfaces;
using System;

namespace JbnLib.Mdl
{
  public class TriangleMap : IStructure
  {
    private ushort _VertexIndex;

    public static int Length => 2;

    public ushort VertexIndex
    {
      get => this._VertexIndex;
      set => this._VertexIndex = value;
    }

    public TriangleMap(byte[] raw) => this._VertexIndex = BitConverter.ToUInt16(raw, 0);

    public TriangleMap(ushort vertexindex) => this._VertexIndex = vertexindex;

    public byte[] GetBytes() => BitConverter.GetBytes(this._VertexIndex);

    public new string ToString() => this._VertexIndex.ToString();
  }
}
