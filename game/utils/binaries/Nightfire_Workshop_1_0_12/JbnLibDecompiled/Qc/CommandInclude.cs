// Decompiled with JetBrains decompiler
// Type: JbnLib.Qc.CommandInclude
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

namespace JbnLib.Qc
{
  public class CommandInclude
  {
    public const string Command = "$include";
    private string _File;

    public string File
    {
      get => this._File;
      set => this._File = value;
    }

    public CommandInclude()
    {
    }

    public CommandInclude(string file) => this._File = file;

    public new string ToString() => "$include \"" + this._File + "\"";
  }
}
