// Decompiled with JetBrains decompiler
// Type: JbnLib.Mdl.ModelInfoV14
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Interfaces;
using JbnLib.Shared;
using System;
using System.Collections.Generic;

namespace JbnLib.Mdl
{
  public class ModelInfoV14 : IStructure
  {
    private int _SkinReference;
    private CountOffsetPair _Mesh;
    private List<MeshV14> _MeshCollection = new List<MeshV14>();

    public static int Length => 12;

    public int SkinReference
    {
      get => this._SkinReference;
      set => this._SkinReference = value;
    }

    public CountOffsetPair Mesh
    {
      get => this._Mesh;
      set => this._Mesh = value;
    }

    public List<MeshV14> MeshCollection
    {
      get => this._MeshCollection;
      set => this._MeshCollection = value;
    }

    public ModelInfoV14(byte[] raw)
    {
      this._SkinReference = BitConverter.ToInt32(raw, 0);
      this._Mesh = new CountOffsetPair(BitConverter.ToInt32(raw, 4), BitConverter.ToInt32(raw, 8));
    }

    public ModelInfoV14(int skin)
    {
      this._SkinReference = skin;
      this._Mesh = new CountOffsetPair();
    }

    public ModelInfoV14(int skin, CountOffsetPair mesh)
    {
      this._SkinReference = skin;
      this._Mesh = mesh;
    }

    public byte[] GetBytes()
    {
      byte[] bytes = new byte[ModelInfoV14.Length];
      BitConverter.GetBytes(this._SkinReference).CopyTo((Array) bytes, 0);
      this._Mesh.GetBytes().CopyTo((Array) bytes, 4);
      return bytes;
    }

    public new string ToString() => this._SkinReference.ToString() + ", " + (object) this._MeshCollection.Count;
  }
}
