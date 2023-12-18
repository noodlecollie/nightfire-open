// Decompiled with JetBrains decompiler
// Type: JbnLib.Map.BrushV220
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using System.Collections.Generic;

namespace JbnLib.Map
{
  public class BrushV220 : List<SideV220>, IBrush
  {
    private int _Id = -1;

    public int Id
    {
      get => this._Id;
      set => this._Id = value;
    }

    public BrushV220()
    {
    }

    public BrushV220(int id) => this._Id = id;

    public new string ToString()
    {
      string str = "{ // Brush " + (object) this._Id + "\n";
      foreach (SideV220 sideV220 in (List<SideV220>) this)
        str += sideV220.ToString();
      return str + "}\n";
    }

    public BrushV510 ToV510()
    {
      BrushV510 v510 = new BrushV510();
      v510.Id = this._Id;
      foreach (SideV220 sideV220 in (List<SideV220>) this)
        v510.Add(sideV220.ToV510());
      return v510;
    }
  }
}
