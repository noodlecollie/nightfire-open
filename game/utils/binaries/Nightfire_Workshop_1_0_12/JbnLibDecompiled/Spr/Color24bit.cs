// Decompiled with JetBrains decompiler
// Type: JbnLib.Spr.Color24bit
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Lang;
using System.Drawing;

namespace JbnLib.Spr
{
  public struct Color24bit
  {
    public const int LENGTH = 3;
    public byte R;
    public byte G;
    public byte B;

    public Color24bit(byte r, byte g, byte b)
    {
      this.R = r;
      this.G = g;
      this.B = b;
    }

    public Color24bit(byte[] rgb)
    {
      if (rgb.Length != 3)
        Messages.ThrowException("Spr.Color24bit.New(byte[])", Messages.COLOR_RGB);
      this.R = rgb[0];
      this.G = rgb[1];
      this.B = rgb[2];
    }

    public Color GetColor() => Color.FromArgb((int) this.R, (int) this.G, (int) this.B);
  }
}
