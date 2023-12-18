// Decompiled with JetBrains decompiler
// Type: JbnLib.Mdl.LodVertices
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Interfaces;
using System;
using System.Collections.Generic;

namespace JbnLib.Mdl
{
  public class LodVertices : IStructure
  {
    private short _LodVertexCount;
    private List<LodVertex> _LodVertexCollection = new List<LodVertex>();

    public static int Length => 2;

    public short LodVertexCount
    {
      get => this._LodVertexCount;
      set => this._LodVertexCount = value;
    }

    public List<LodVertex> LodVertexCollection
    {
      get => this._LodVertexCollection;
      set => this._LodVertexCollection = value;
    }

    public LodVertices(byte[] raw) => this._LodVertexCount = BitConverter.ToInt16(raw, 0);

    public LodVertices(short lodvertexcount) => this._LodVertexCount = lodvertexcount;

    public byte[] GetBytes()
    {
      byte[] bytes = new byte[LodVertices.Length + LodVertex.Length * this._LodVertexCollection.Count];
      BitConverter.GetBytes(this._LodVertexCount).CopyTo((Array) bytes, 0);
      for (int index = 0; index < this._LodVertexCollection.Count; ++index)
        this._LodVertexCollection[index].GetBytes().CopyTo((Array) bytes, index * LodVertex.Length + 2);
      return bytes;
    }

    public override string ToString() => this._LodVertexCount.ToString();
  }
}
