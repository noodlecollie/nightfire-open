// Decompiled with JetBrains decompiler
// Type: JbnLib.Mdl.BodyGroupV11
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Interfaces;
using System;
using System.Text;

namespace JbnLib.Mdl
{
  public class BodyGroupV11 : IStructure
  {
    private string _Name;
    private int _UnknownCount;
    private int _ModelCount;
    private int _BodyOffset;
    private int _ModelOffset;

    public static int Length => 80;

    public string Name
    {
      get => this._Name;
      set => this._Name = value;
    }

    public int UnknownCount
    {
      get => this._UnknownCount;
      set => this._UnknownCount = value;
    }

    public int ModelCount
    {
      get => this._ModelCount;
      set => this._ModelCount = value;
    }

    public int BodyOffset
    {
      get => this._BodyOffset;
      set => this._BodyOffset = value;
    }

    public int ModelOffset
    {
      get => this._ModelOffset;
      set => this._ModelOffset = value;
    }

    public BodyGroupV11(byte[] raw)
    {
      this._Name = Encoding.ASCII.GetString(raw, 0, 64).Trim(new char[1]);
      this._ModelCount = BitConverter.ToInt32(raw, 64);
      this._UnknownCount = BitConverter.ToInt32(raw, 68);
      this._BodyOffset = BitConverter.ToInt32(raw, 72);
      this._ModelOffset = BitConverter.ToInt32(raw, 76);
    }

    public BodyGroupV11(
      string name,
      int bodycount,
      int modelcount,
      int bodyoffset,
      int modeloffset)
    {
      this._Name = name;
      this._UnknownCount = bodycount;
      this._ModelCount = modelcount;
      this._BodyOffset = bodyoffset;
      this._ModelOffset = modeloffset;
    }

    public byte[] GetBytes()
    {
      byte[] bytes = new byte[BodyGroupV11.Length];
      Encoding.ASCII.GetBytes(this._Name).CopyTo((Array) bytes, 0);
      BitConverter.GetBytes(this._ModelCount).CopyTo((Array) bytes, 64);
      BitConverter.GetBytes(this._UnknownCount).CopyTo((Array) bytes, 68);
      BitConverter.GetBytes(this._BodyOffset).CopyTo((Array) bytes, 72);
      BitConverter.GetBytes(this._ModelOffset).CopyTo((Array) bytes, 76);
      return bytes;
    }

    public new string ToString() => this._Name;
  }
}
