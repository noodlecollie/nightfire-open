// Decompiled with JetBrains decompiler
// Type: JbnLib.Map.EntityV220
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using System.Collections.Generic;

namespace JbnLib.Map
{
  public class EntityV220 : Dictionary<string, string>
  {
    private int _Id = -1;
    private List<BrushV220> _BrushCollection = new List<BrushV220>();

    public int Id
    {
      get => this._Id;
      set => this._Id = value;
    }

    public List<BrushV220> BrushCollection
    {
      get => this._BrushCollection;
      set => this._BrushCollection = value;
    }

    public EntityV220()
    {
    }

    public EntityV220(int id) => this._Id = id;

    public new string ToString()
    {
      string str = "{ // Entity " + (object) this._Id + "\n";
      foreach (KeyValuePair<string, string> keyValuePair in (Dictionary<string, string>) this)
        str = str + "\"" + keyValuePair.Key + "\" \"" + keyValuePair.Value + "\"\n";
      foreach (BrushV220 brush in this._BrushCollection)
        str += brush.ToString();
      return str + "}\n";
    }

    public EntityV510 ToV510()
    {
      EntityV510 v510 = new EntityV510(this._Id);
      foreach (KeyValuePair<string, string> keyValuePair in (Dictionary<string, string>) this)
        v510.Add(keyValuePair.Key, keyValuePair.Value);
      foreach (BrushV220 brush in this._BrushCollection)
        v510.BrushCollection.Add(brush.ToV510());
      return v510;
    }
  }
}
