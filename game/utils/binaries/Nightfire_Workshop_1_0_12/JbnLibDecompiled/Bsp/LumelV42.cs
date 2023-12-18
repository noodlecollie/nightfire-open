// Decompiled with JetBrains decompiler
// Type: JbnLib.Bsp.LumelV42
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using System.Drawing;

namespace JbnLib.Bsp
{
  public class LumelV42
  {
    private Color _Color;

    public static int Length => 3;

    public Color Color
    {
      get => this._Color;
      set => this._Color = value;
    }

    public LumelV42(byte[] raw) => this._Color = Color.FromArgb((int) raw[0], (int) raw[1], (int) raw[2]);

    public byte[] GetBytes() => new byte[3]
    {
      this._Color.R,
      this._Color.G,
      this._Color.B
    };

    public new string ToString() => string.Format("{0,3} {1,3} {2,3}", (object) this._Color.R, (object) this._Color.B, (object) this._Color.G);
  }
}
