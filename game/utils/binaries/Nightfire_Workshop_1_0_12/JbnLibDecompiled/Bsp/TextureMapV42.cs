// Decompiled with JetBrains decompiler
// Type: JbnLib.Bsp.TextureMapV42
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Map;
using System;

namespace JbnLib.Bsp
{
  public class TextureMapV42
  {
    private TextureAxis _S;
    private TextureAxis _T;

    public static int Length => 32;

    public TextureAxis S
    {
      get => this._S;
      set => this._S = value;
    }

    public TextureAxis T
    {
      get => this._T;
      set => this._T = value;
    }

    public TextureMapV42(byte[] raw)
    {
      this._S = new TextureAxis(BitConverter.ToSingle(raw, 0), BitConverter.ToSingle(raw, 4), BitConverter.ToSingle(raw, 8), BitConverter.ToSingle(raw, 12));
      this._T = new TextureAxis(BitConverter.ToSingle(raw, 16), BitConverter.ToSingle(raw, 20), BitConverter.ToSingle(raw, 24), BitConverter.ToSingle(raw, 28));
    }

    public TextureMapV42(TextureAxis u, TextureAxis v)
    {
      this._S = u;
      this._T = v;
    }

    public byte[] GetBytes()
    {
      byte[] bytes = new byte[TextureMapV42.Length];
      this._S.GetBytes().CopyTo((Array) bytes, 0);
      this._T.GetBytes().CopyTo((Array) bytes, 16);
      return bytes;
    }

    public new string ToString() => string.Format("[ {0} ] [ {1} ]", (object) this._S.ToString(), (object) this._T.ToString());
  }
}
