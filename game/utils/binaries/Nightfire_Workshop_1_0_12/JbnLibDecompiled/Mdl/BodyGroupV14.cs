// Decompiled with JetBrains decompiler
// Type: JbnLib.Mdl.BodyGroupV14
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Interfaces;
using System;
using System.Text;

namespace JbnLib.Mdl
{
  public class BodyGroupV14 : IStructure
  {
    private string _Name;
    private int _ModelCount;
    private int _BodyCount;
    private int _ModelOffset;

    public static int Length => 76;

    public string Name
    {
      get => this._Name;
      set => this._Name = value;
    }

    public int ModelCount
    {
      get => this._ModelCount;
      set => this._ModelCount = value;
    }

    public int BodyCount
    {
      get => this._BodyCount;
      set => this._BodyCount = value;
    }

    public int ModelOffset
    {
      get => this._ModelOffset;
      set => this._ModelOffset = value;
    }

    public BodyGroupV14(byte[] raw)
    {
      this._Name = Encoding.ASCII.GetString(raw, 0, 64).Trim(new char[1]);
      this._ModelCount = BitConverter.ToInt32(raw, 64);
      this._BodyCount = BitConverter.ToInt32(raw, 68);
      this._ModelOffset = BitConverter.ToInt32(raw, 72);
    }

    public BodyGroupV14(string name, int modelcount, int modeloffset)
    {
      this._Name = name;
      this._ModelCount = modelcount;
      this._BodyCount = 0;
      this._ModelOffset = modeloffset;
    }

    public BodyGroupV14(string name, int modelcount, int bodycount, int modeloffset)
    {
      this._Name = name;
      this._ModelCount = modelcount;
      this._BodyCount = bodycount;
      this._ModelOffset = modeloffset;
    }

    public byte[] GetBytes()
    {
      byte[] bytes = new byte[BodyGroupV14.Length];
      Encoding.ASCII.GetBytes(this._Name).CopyTo((Array) bytes, 0);
      BitConverter.GetBytes(this._ModelCount).CopyTo((Array) bytes, 64);
      BitConverter.GetBytes(this._BodyCount).CopyTo((Array) bytes, 68);
      BitConverter.GetBytes(this._ModelOffset).CopyTo((Array) bytes, 72);
      return bytes;
    }

    public new string ToString() => this._Name;
  }
}
