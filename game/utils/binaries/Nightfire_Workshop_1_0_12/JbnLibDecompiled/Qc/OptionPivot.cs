// Decompiled with JetBrains decompiler
// Type: JbnLib.Qc.OptionPivot
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

namespace JbnLib.Qc
{
  public class OptionPivot
  {
    public const string Option = "pivot";
    private int _Index;
    private float _Start;
    private float _End;

    public int Index
    {
      get => this._Index;
      set => this._Index = value;
    }

    public float Start
    {
      get => this._Start;
      set => this._Start = value;
    }

    public float End
    {
      get => this._End;
      set => this._End = value;
    }

    public OptionPivot()
    {
    }

    public OptionPivot(int index, float start, float end)
    {
      this._Index = index;
      this._Start = start;
      this._End = end;
    }

    public new string ToString() => string.Format("{0} {1} {2} {3}", (object) "pivot", (object) this._Index, (object) this._Start, (object) this._End);
  }
}
