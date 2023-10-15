// Decompiled with JetBrains decompiler
// Type: JbnLib.Qc.OptionEvent
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

namespace JbnLib.Qc
{
  public class OptionEvent
  {
    public const string Option = "event";
    private int _EventValue;
    private int _Frame;
    private string _Options;

    public int EventValue
    {
      get => this._EventValue;
      set => this._EventValue = value;
    }

    public int Frame
    {
      get => this._Frame;
      set => this._Frame = value;
    }

    public string Options
    {
      get => this._Options;
      set => this._Options = value;
    }

    public OptionEvent()
    {
    }

    public OptionEvent(int value, int frame, string options)
    {
      this._EventValue = value;
      this._Frame = frame;
      this._Options = options;
    }

    public new string ToString() => string.Format("{0} {1} {2} \"{3}\"", (object) "event", (object) this._EventValue, (object) this._Frame, (object) this._Options);
  }
}
