// Decompiled with JetBrains decompiler
// Type: JbnLib.Mdl.ModelV14
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Interfaces;
using System;
using System.Collections.Generic;
using System.Text;

namespace JbnLib.Mdl
{
  public class ModelV14 : IStructure
  {
    private string _Name;
    private int _Index;
    private int[] _ModelInfoOffset = new int[24];
    private List<ModelInfoV14> _ModelInfoCollection = new List<ModelInfoV14>();

    public static int Length => 132;

    public string Name
    {
      get => this._Name;
      set => this._Name = value;
    }

    public int Index
    {
      get => this._Index;
      set => this._Index = value;
    }

    public int[] ModelInfoOffset
    {
      get => this._ModelInfoOffset;
      set => this._ModelInfoOffset = value;
    }

    public List<ModelInfoV14> ModelInfoCollection
    {
      get => this._ModelInfoCollection;
      set => this._ModelInfoCollection = value;
    }

    public ModelV14(byte[] raw)
    {
      this._Name = Encoding.ASCII.GetString(raw, 0, 32).Trim(new char[1]);
      this._Index = BitConverter.ToInt32(raw, 32);
      for (int index = 0; index < this._ModelInfoOffset.Length; ++index)
        this._ModelInfoOffset[index] = BitConverter.ToInt32(raw, index * 4 + 36);
    }

    public ModelV14(string name, int index)
    {
      this._Name = name;
      this._Index = index;
      this._ModelInfoOffset = new int[24];
    }

    public ModelV14(string name, int index, int[] modelinfooffset)
    {
      this._Name = name;
      this._Index = index;
      this._ModelInfoOffset = modelinfooffset;
    }

    public byte[] GetBytes()
    {
      byte[] bytes = new byte[ModelV14.Length];
      Encoding.ASCII.GetBytes(this._Name).CopyTo((Array) bytes, 0);
      BitConverter.GetBytes(this._Index).CopyTo((Array) bytes, 32);
      for (int index = 0; index < this._ModelInfoOffset.Length; ++index)
        BitConverter.GetBytes(this._ModelInfoOffset[index]).CopyTo((Array) bytes, index * 4 + 36);
      return bytes;
    }

    public new string ToString() => this._Name;
  }
}
