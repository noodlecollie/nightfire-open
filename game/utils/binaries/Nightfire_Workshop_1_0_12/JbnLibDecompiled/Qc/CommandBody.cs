// Decompiled with JetBrains decompiler
// Type: JbnLib.Qc.CommandBody
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

namespace JbnLib.Qc
{
  public class CommandBody
  {
    public const string Command = "$body";
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
      set => this._File = value;
    }

    public CommandBody()
    {
    }

    public CommandBody(string name, string file)
    {
      this._Name = name;
      this._File = file;
    }

    public new string ToString() => string.Format("{0} \"{1}\" \"{2}\"", (object) "$body", (object) this._Name, (object) this._File);
  }
}
