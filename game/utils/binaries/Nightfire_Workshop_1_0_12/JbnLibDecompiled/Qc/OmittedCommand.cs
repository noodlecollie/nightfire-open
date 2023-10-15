// Decompiled with JetBrains decompiler
// Type: JbnLib.Qc.OmittedCommand
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

namespace JbnLib.Qc
{
  public class OmittedCommand
  {
    private long _Line;
    private string _Command;

    public long Line => this._Line;

    public string Command => this._Command;

    public OmittedCommand(long line, string command)
    {
      this._Line = line;
      this._Command = command;
    }

    public new string ToString() => this._Line.ToString() + ", " + this._Command;
  }
}
