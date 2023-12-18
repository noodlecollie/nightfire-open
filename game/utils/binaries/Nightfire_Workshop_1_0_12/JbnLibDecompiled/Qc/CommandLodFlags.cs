// Decompiled with JetBrains decompiler
// Type: JbnLib.Qc.CommandLodFlags
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

namespace JbnLib.Qc
{
  public class CommandLodFlags
  {
    public const string Command = "$lodflags";
    private int _Value;

    public int Value
    {
      get => this._Value;
      set => this._Value = value;
    }

    public CommandLodFlags() => this._Value = 0;

    public CommandLodFlags(int value) => this._Value = value;

    public new string ToString() => "$lodflags " + (object) this._Value;
  }
}
