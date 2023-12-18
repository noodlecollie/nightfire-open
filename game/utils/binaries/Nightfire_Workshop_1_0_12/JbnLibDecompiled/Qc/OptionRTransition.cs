// Decompiled with JetBrains decompiler
// Type: JbnLib.Qc.OptionRTransition
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

namespace JbnLib.Qc
{
  public class OptionRTransition
  {
    public const string Option = "rtransition";
    private int _EntryBone;
    private int _ExitBone;

    public int EntryBone
    {
      get => this._EntryBone;
      set => this._EntryBone = value;
    }

    public int ExitBone
    {
      get => this._ExitBone;
      set => this._ExitBone = value;
    }

    public OptionRTransition()
    {
    }

    public OptionRTransition(int entry, int exit)
    {
      this._EntryBone = entry;
      this._ExitBone = exit;
    }

    public new string ToString() => string.Format("{0} {1} {2}", (object) "rtransition", (object) this._EntryBone, (object) this._ExitBone);
  }
}
