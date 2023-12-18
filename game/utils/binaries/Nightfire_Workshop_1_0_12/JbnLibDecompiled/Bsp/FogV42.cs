// Decompiled with JetBrains decompiler
// Type: JbnLib.Bsp.FogV42
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using System;
using System.Text;

namespace JbnLib.Bsp
{
  public class FogV42
  {
    private string _Material;

    public static int Length => 64;

    public string Material
    {
      get => this._Material;
      set => this._Material = value;
    }

    public FogV42(byte[] raw) => this._Material = Encoding.ASCII.GetString(raw).TrimEnd(new char[1]);

    public FogV42(string texture) => this._Material = texture;

    public byte[] GetBytes()
    {
      byte[] bytes = new byte[FogV42.Length];
      Encoding.ASCII.GetBytes(this._Material).CopyTo((Array) bytes, 0);
      return bytes;
    }

    public new string ToString() => this._Material;
  }
}
