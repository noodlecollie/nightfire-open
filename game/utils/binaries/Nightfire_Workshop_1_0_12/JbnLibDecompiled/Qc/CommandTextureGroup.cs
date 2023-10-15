// Decompiled with JetBrains decompiler
// Type: JbnLib.Qc.CommandTextureGroup
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using System.Collections.Generic;

namespace JbnLib.Qc
{
  public class CommandTextureGroup
  {
    public const string Command = "$texturegroup";
    private string _Name;
    private List<List<string>> _SkinCollection = new List<List<string>>();

    public string Name
    {
      get => this._Name;
      set => this._Name = value;
    }

    public List<List<string>> SkinCollection
    {
      get => this._SkinCollection;
      set => this._SkinCollection = value;
    }

    public new string ToString()
    {
      string str1 = "$texturegroup " + this._Name + "\r\n{\r\n";
      foreach (List<string> skin in this._SkinCollection)
      {
        str1 += "\t{ ";
        foreach (string str2 in skin)
          str1 = str1 + str2 + ", ";
        str1 = str1.TrimEnd(' ', ',') + " }\r\n";
      }
      return str1 + "}";
    }
  }
}
