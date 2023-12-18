// Decompiled with JetBrains decompiler
// Type: JbnLib.Fgd.Types.Flags
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Fgd.Interfaces;
using System.Collections.Generic;

namespace JbnLib.Fgd.Types
{
  public class Flags : IFlags
  {
    private string _Name;
    private List<Flag> _FlagCollection = new List<Flag>();

    public string Name
    {
      get => this._Name;
      set => this._Name = value;
    }

    public List<Flag> FlagCollection
    {
      get => this._FlagCollection;
      set => this._FlagCollection = value;
    }

    public Flags()
    {
    }

    public Flags(string name, List<Flag> collection)
    {
      this._Name = name;
      this._FlagCollection = collection;
    }

    public Flags(string line)
    {
      this._Name = line.Substring(0, line.IndexOf('=')).Trim().ToLower().Replace("(flags)", "");
      string str = line.Substring(line.IndexOf('[') + 1, line.LastIndexOf(']') - line.IndexOf('[') - 1).Trim();
      char[] chArray = new char[1]{ '\n' };
      foreach (string line1 in str.Split(chArray))
      {
        if (line1.Contains(":"))
          this._FlagCollection.Add(new Flag(line1));
      }
    }

    public new string ToString()
    {
      string str = this._Name + "(flags) =\n[\n";
      foreach (Flag flag in this._FlagCollection)
        str = str + "\t" + flag.ToString() + "\n";
      return str + "]";
    }
  }
}
