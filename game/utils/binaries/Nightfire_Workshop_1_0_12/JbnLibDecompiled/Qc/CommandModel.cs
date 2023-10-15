// Decompiled with JetBrains decompiler
// Type: JbnLib.Qc.CommandModel
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using System.IO;

namespace JbnLib.Qc
{
  public class CommandModel
  {
    public const string Command = "$model";
    private string _Name;
    private string _File;

    public string Name
    {
      get => this._Name;
      set => this._Name = value;
    }

    public string File
    {
      get => this._File;
      set
      {
        if (new FileInfo(value).Extension.Length == 0)
          this._File = value + ".smd";
        else
          this._File = value;
      }
    }

    public CommandModel()
    {
    }

    public CommandModel(string name, string file)
    {
      this._Name = name;
      this._File = file;
    }

    public new string ToString() => string.Format("{0} \"{1}\" \"{2}\"", (object) "$model", (object) this._Name, (object) this._File);
  }
}
