// Decompiled with JetBrains decompiler
// Type: JbnLib.Bsp.ShaderV42
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using System;
using System.Text;

namespace JbnLib.Bsp
{
  public class ShaderV42
  {
    private string _Texture;

    public static int Length => 64;

    public string Texture
    {
      get => this._Texture;
      set => this._Texture = value;
    }

    public ShaderV42(byte[] raw) => this._Texture = Encoding.ASCII.GetString(raw).TrimEnd(new char[1]);

    public ShaderV42(string texture) => this._Texture = texture;

    public byte[] GetBytes()
    {
      byte[] bytes = new byte[ShaderV42.Length];
      Encoding.ASCII.GetBytes(this._Texture).CopyTo((Array) bytes, 0);
      return bytes;
    }

    public new string ToString() => this._Texture;
  }
}
