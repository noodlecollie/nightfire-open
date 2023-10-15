// Decompiled with JetBrains decompiler
// Type: JbnLib.Qc.OptionFrame
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

namespace JbnLib.Qc
{
  public class OptionFrame
  {
    public const string Option = "frame";
    private int _Start;
    private int _End;

    public int Start
    {
      get => this._Start;
      set => this._Start = value;
    }

    public int End
    {
      get => this._End;
      set => this._End = value;
    }

    public new string ToString() => string.Format("{0} {1} {2}", (object) "frame", (object) this._Start, (object) this._End);
  }
}
