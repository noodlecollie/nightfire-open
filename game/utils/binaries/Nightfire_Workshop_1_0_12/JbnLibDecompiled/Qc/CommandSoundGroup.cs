// Decompiled with JetBrains decompiler
// Type: JbnLib.Qc.CommandSoundGroup
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using System.Collections.Generic;

namespace JbnLib.Qc
{
  public class CommandSoundGroup
  {
    public const string Command = "$soundgroup";
    private string _Name;
    private List<string> _Sounds = new List<string>();

    public string Name
    {
      get => this._Name;
      set => this._Name = value;
    }

    public List<string> Sounds
    {
      get => this._Sounds;
      set => this._Sounds = value;
    }

    public new string ToString()
    {
      string str1 = "$soundgroup \"" + this._Name + "\"\r\n{\r\n";
      foreach (string sound in this._Sounds)
        str1 = str1 + "\t\"" + sound + "\"\r\n";
      string str2;
      return str2 = str1 + "}";
    }
  }
}
