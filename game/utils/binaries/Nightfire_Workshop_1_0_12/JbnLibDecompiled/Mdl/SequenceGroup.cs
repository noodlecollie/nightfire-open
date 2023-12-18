// Decompiled with JetBrains decompiler
// Type: JbnLib.Mdl.SequenceGroup
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Interfaces;
using System;
using System.Text;

namespace JbnLib.Mdl
{
  public class SequenceGroup : IStructure
  {
    private string _Label;
    private string _FileName;
    private int _Cache;
    private int _Data;

    public static int Length => 104;

    public string Label
    {
      get => this._Label;
      set => this._Label = value;
    }

    public string FileName
    {
      get => this._FileName;
      set => this._FileName = value;
    }

    public int Cache
    {
      get => this._Cache;
      set => this._Cache = value;
    }

    public int Data
    {
      get => this._Data;
      set => this._Data = value;
    }

    public SequenceGroup(byte[] raw)
    {
      this._Label = Encoding.ASCII.GetString(raw, 0, 32).Trim(new char[1]);
      this._FileName = Encoding.ASCII.GetString(raw, 32, 64).Trim(new char[1]);
      this._Cache = BitConverter.ToInt32(raw, 96);
      this._Data = BitConverter.ToInt32(raw, 100);
    }

    public SequenceGroup(string label, string name, int cache, int data)
    {
      this._Label = label;
      this._FileName = name;
      this._Cache = cache;
      this._Data = data;
    }

    public byte[] GetBytes()
    {
      byte[] bytes = new byte[SequenceGroup.Length];
      Encoding.ASCII.GetBytes(this._Label).CopyTo((Array) bytes, 0);
      Encoding.ASCII.GetBytes(this._FileName).CopyTo((Array) bytes, 32);
      BitConverter.GetBytes(this._Cache).CopyTo((Array) bytes, 96);
      BitConverter.GetBytes(this._Data).CopyTo((Array) bytes, 100);
      return bytes;
    }

    public new string ToString() => this._Label;
  }
}
