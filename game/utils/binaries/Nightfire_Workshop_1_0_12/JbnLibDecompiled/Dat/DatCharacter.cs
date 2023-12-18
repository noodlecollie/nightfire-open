// Decompiled with JetBrains decompiler
// Type: JbnLib.Dat.DatCharacter
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

namespace JbnLib.Dat
{
  public class DatCharacter
  {
    private ushort _Width;
    private ushort _Height;
    private ushort _X;
    private ushort _Y;

    public ushort Width
    {
      get => this._Width;
      set => this._Width = value;
    }

    public ushort Height
    {
      get => this._Height;
      set => this._Height = value;
    }

    public ushort X
    {
      get => this._X;
      set => this._X = value;
    }

    public ushort Y
    {
      get => this._Y;
      set => this._Y = value;
    }

    public DatCharacter()
    {
      this._Width = (ushort) 0;
      this._Height = (ushort) 0;
      this._X = (ushort) 0;
      this._Y = (ushort) 0;
    }

    public DatCharacter(ushort width, ushort height, ushort x, ushort y)
    {
      this._Width = width;
      this._Height = height;
      this._X = x;
      this._Y = y;
    }

    public new string ToString() => this._Width.ToString() + " " + (object) this._Height + " " + (object) this._X + " " + (object) this._Y;

    public bool Unused() => this._Width == (ushort) 0 && this._Height == (ushort) 0 && this._X == (ushort) 0 && this._Y == (ushort) 0;
  }
}
