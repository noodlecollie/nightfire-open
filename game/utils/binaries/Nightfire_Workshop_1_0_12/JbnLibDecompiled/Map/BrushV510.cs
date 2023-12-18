// Decompiled with JetBrains decompiler
// Type: JbnLib.Map.BrushV510
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using System.Collections.Generic;

namespace JbnLib.Map
{
  public class BrushV510 : List<SideV510>, IBrush
  {
    private int _Id = -1;

    public int Id
    {
      get => this._Id;
      set => this._Id = value;
    }

    public BrushV510()
    {
    }

    public BrushV510(int id) => this._Id = id;

    public new string ToString()
    {
      string str = "{ // Brush " + (object) this._Id + "\n";
      foreach (SideV510 sideV510 in (List<SideV510>) this)
        str += sideV510.ToString();
      return str + "}\n";
    }

    public string ToStringNoComments()
    {
      string str = "{\n";
      foreach (SideV510 sideV510 in (List<SideV510>) this)
        str += sideV510.ToString();
      return str + "}\n";
    }

    public BrushV220 ToV220()
    {
      BrushV220 v220 = new BrushV220();
      v220.Id = this._Id;
      foreach (SideV510 sideV510 in (List<SideV510>) this)
        v220.Add(sideV510.ToV220());
      return v220;
    }
  }
}
