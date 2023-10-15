// Decompiled with JetBrains decompiler
// Type: JbnLib.Qc.CommandRenameBone
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

namespace JbnLib.Qc
{
  public class CommandRenameBone
  {
    public const string Command = "$renamebone";
    private string _OldName;
    private string _NewName;

    public string OldName
    {
      get => this._OldName;
      set => this._OldName = value;
    }

    public string NewName
    {
      get => this._NewName;
      set => this._NewName = value;
    }

    public new string ToString() => string.Format("{0} \"{1}\" \"{2}\"", (object) "$renamebone", (object) this._OldName, (object) this._NewName);
  }
}
