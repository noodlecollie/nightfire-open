// Decompiled with JetBrains decompiler
// Type: JbnLib.Mdl.ModelV11
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Interfaces;
using System;
using System.Text;

namespace JbnLib.Mdl
{
  public class ModelV11 : IStructure
  {
    private string _Name;
    private int _Index;
    private ModelInfoV11[] _ModelInfoCollection = new ModelInfoV11[ModelV11.Skins];

    public static int Length => 48132;

    public static int Skins => 24;

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

    public ModelInfoV11[] ModelInfoCollection
    {
      get => this._ModelInfoCollection;
      set => this._ModelInfoCollection = value;
    }

    public ModelV11(byte[] raw)
    {
      this._Name = Encoding.ASCII.GetString(raw, 0, 32).Trim(new char[1]);
      this._Index = BitConverter.ToInt32(raw, 32);
      for (int index = 0; index < this._ModelInfoCollection.Length; ++index)
      {
        byte[] numArray = new byte[ModelInfoV11.Length];
        Array.Copy((Array) raw, index * ModelInfoV11.Length + 36, (Array) numArray, 0, ModelInfoV11.Length);
        this._ModelInfoCollection[index] = new ModelInfoV11(numArray);
      }
    }

    public ModelV11(string name, int index, ModelInfoV11[] modelinfo)
    {
      this._Name = name;
      this._Index = index;
      this._ModelInfoCollection = modelinfo;
    }

    public byte[] GetBytes()
    {
      byte[] bytes = new byte[ModelV11.Length];
      Encoding.ASCII.GetBytes(this._Name).CopyTo((Array) bytes, 0);
      BitConverter.GetBytes(this._Index).CopyTo((Array) bytes, 32);
      for (int index = 0; index < this._ModelInfoCollection.Length; ++index)
        this._ModelInfoCollection[index].GetBytes().CopyTo((Array) bytes, index * ModelInfoV11.Length + 36);
      return bytes;
    }

    public new string ToString() => this._Name;
  }
}
