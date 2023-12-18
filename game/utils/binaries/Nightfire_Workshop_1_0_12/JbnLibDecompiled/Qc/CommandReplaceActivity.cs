// Decompiled with JetBrains decompiler
// Type: JbnLib.Qc.CommandReplaceActivity
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

namespace JbnLib.Qc
{
  public class CommandReplaceActivity
  {
    public const string Command = "$replaceactivity";
    private string _SequenceName;
    private string _Activity;

    public string SequenceName
    {
      get => this._SequenceName;
      set => this._SequenceName = value;
    }

    public string Activity
    {
      get => this._Activity;
      set => this._Activity = value;
    }

    public CommandReplaceActivity()
    {
    }

    public CommandReplaceActivity(string sequencename, string activity)
    {
      this._SequenceName = sequencename;
      this._Activity = activity;
    }

    public new string ToString() => string.Format("{0} \"{1}\" {2}", (object) "$replaceactivity", (object) this._SequenceName, (object) this._Activity);
  }
}
