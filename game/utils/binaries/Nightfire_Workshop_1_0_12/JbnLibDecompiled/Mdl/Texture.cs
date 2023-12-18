// Decompiled with JetBrains decompiler
// Type: JbnLib.Mdl.Texture
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Interfaces;
using System;
using System.Text;

namespace JbnLib.Mdl
{
  public class Texture : IStructure
  {
    private string _MaterialName;
    private string _TextureName;
    private int _ReferenceCount;
    private int _Unknown;

    public static int Length => 136;

    public string MaterialName
    {
      get => this._MaterialName;
      set => this._MaterialName = value;
    }

    public string TextureName
    {
      get => this._TextureName;
      set => this._TextureName = value;
    }

    public int ReferenceCount
    {
      get => this._ReferenceCount;
      set => this._ReferenceCount = value;
    }

    public int Unknown
    {
      get => this._Unknown;
      set => this._Unknown = value;
    }

    public Texture(byte[] raw)
    {
      this._MaterialName = Encoding.ASCII.GetString(raw, 0, 64).Trim(new char[1]);
      this._TextureName = Encoding.ASCII.GetString(raw, 64, 64).Trim(new char[1]);
      this._ReferenceCount = BitConverter.ToInt32(raw, 128);
      this._Unknown = BitConverter.ToInt32(raw, 132);
    }

    public Texture(string materialname, string texturename, int referencecount, int unknown)
    {
      this._MaterialName = materialname;
      this._TextureName = texturename;
      this._ReferenceCount = referencecount;
      this._Unknown = unknown;
    }

    public byte[] GetBytes()
    {
      byte[] bytes = new byte[Texture.Length];
      Encoding.ASCII.GetBytes(this._MaterialName).CopyTo((Array) bytes, 0);
      Encoding.ASCII.GetBytes(this._TextureName).CopyTo((Array) bytes, 64);
      BitConverter.GetBytes(this._ReferenceCount).CopyTo((Array) bytes, 128);
      BitConverter.GetBytes(this._Unknown).CopyTo((Array) bytes, 132);
      return bytes;
    }

    public new string ToString() => this._TextureName;
  }
}
