// Decompiled with JetBrains decompiler
// Type: JbnLib.Dat.DatCellSize
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

namespace JbnLib.Dat
{
  public class DatCellSize
  {
    public const string Command = "cellsize";
    private ushort _Width;
    private ushort _Height;

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

    public DatCellSize()
    {
      this._Width = (ushort) 0;
      this._Height = (ushort) 0;
    }

    public DatCellSize(ushort width, ushort height)
    {
      this._Width = width;
      this._Height = height;
    }

    public new string ToString() => "cellsize " + (object) this._Width + " " + (object) this._Height;
  }
}
