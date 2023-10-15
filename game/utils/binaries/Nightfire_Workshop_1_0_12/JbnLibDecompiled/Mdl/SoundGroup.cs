// Decompiled with JetBrains decompiler
// Type: JbnLib.Mdl.SoundGroup
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Interfaces;
using System;
using System.Text;

namespace JbnLib.Mdl
{
  public class SoundGroup : IStructure
  {
    private string _Name;
    private int _Offset;
    private Sounds _Sounds;

    public static int Length => 36;

    public string Name
    {
      get => this._Name;
      set => this._Name = value;
    }

    public int Offset
    {
      get => this._Offset;
      set => this._Offset = value;
    }

    public Sounds Sounds
    {
      get => this._Sounds;
      set => this._Sounds = value;
    }

    public SoundGroup(byte[] raw)
    {
      this._Name = Encoding.ASCII.GetString(raw, 0, 32).Trim(new char[1]);
      this._Offset = BitConverter.ToInt32(raw, 32);
    }

    public SoundGroup(string name, int offset)
    {
      this._Name = name;
      this._Offset = offset;
    }

    public byte[] GetBytes()
    {
      byte[] bytes = new byte[SoundGroup.Length];
      Encoding.ASCII.GetBytes(this._Name).CopyTo((Array) bytes, 0);
      BitConverter.ToInt32(bytes, 32);
      return bytes;
    }

    public new string ToString() => this._Name;
  }
}
