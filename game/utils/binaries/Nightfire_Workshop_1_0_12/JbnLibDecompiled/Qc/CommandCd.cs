﻿// Decompiled with JetBrains decompiler
// Type: JbnLib.Qc.CommandCd
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

namespace JbnLib.Qc
{
  public class CommandCd
  {
    public const string Command = "$cd";
    private string _Directory;

    public string Directory
    {
      get => this._Directory;
      set => this._Directory = value;
    }

    public CommandCd()
    {
    }

    public CommandCd(string directory) => this._Directory = directory;

    public new string ToString() => string.Format("{0} \"{1}\"", (object) "$cd", (object) this._Directory);
  }
}
