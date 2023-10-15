// Decompiled with JetBrains decompiler
// Type: JbnLib.Qc.CommandBodyGroup
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using System.Collections.Generic;

namespace JbnLib.Qc
{
  public class CommandBodyGroup
  {
    public const string Command = "$bodygroup";
    private string _Name;
    private List<BodyGroupItem> _BodyCollection = new List<BodyGroupItem>();

    public string Name
    {
      get => this._Name;
      set => this._Name = value;
    }

    public List<BodyGroupItem> BodyCollection
    {
      get => this._BodyCollection;
      set => this._BodyCollection = value;
    }

    public CommandBodyGroup()
    {
    }

    public CommandBodyGroup(string name) => this._Name = name;

    public new string ToString()
    {
      string str = "$bodygroup \"" + this._Name + "\"\r\n{\r\n";
      foreach (BodyGroupItem body in this._BodyCollection)
        str = str + "\t" + body.ToString() + "\r\n";
      return str + "}";
    }
  }
}
