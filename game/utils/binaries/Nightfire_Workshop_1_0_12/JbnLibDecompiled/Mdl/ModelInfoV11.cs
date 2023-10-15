// Decompiled with JetBrains decompiler
// Type: JbnLib.Mdl.ModelInfoV11
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Interfaces;
using System;
using System.Collections.Generic;

namespace JbnLib.Mdl
{
  public class ModelInfoV11 : IStructure
  {
    private List<MeshV11> _MeshCollection = new List<MeshV11>();

    public static int Length => 2004;

    public static int Meshes => 100;

    public List<MeshV11> MeshCollection
    {
      get => this._MeshCollection;
      set => this._MeshCollection = value;
    }

    public ModelInfoV11(byte[] raw)
    {
      int int32 = BitConverter.ToInt32(raw, 0);
      for (int index = 0; index < int32; ++index)
      {
        byte[] numArray = new byte[MeshV11.Length];
        Array.Copy((Array) raw, index * MeshV11.Length + 4, (Array) numArray, 0, MeshV11.Length);
        this._MeshCollection.Add(new MeshV11(numArray));
      }
    }

    public ModelInfoV11(List<MeshV11> meshes) => this._MeshCollection = meshes;

    public byte[] GetBytes()
    {
      byte[] bytes = new byte[ModelInfoV11.Length];
      BitConverter.GetBytes(this._MeshCollection.Count).CopyTo((Array) bytes, 0);
      for (int index = 0; index < ModelInfoV11.Meshes; ++index)
      {
        if (index < this._MeshCollection.Count)
          this._MeshCollection[index].GetBytes().CopyTo((Array) bytes, index * MeshV11.Length + 4);
        else
          new MeshV11().GetBytes().CopyTo((Array) bytes, index * MeshV11.Length + 4);
      }
      return bytes;
    }

    public new string ToString() => this._MeshCollection.Count.ToString();
  }
}
