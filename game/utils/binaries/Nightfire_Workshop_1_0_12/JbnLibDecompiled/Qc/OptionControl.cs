// Decompiled with JetBrains decompiler
// Type: JbnLib.Qc.OptionControl
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Mdl;

namespace JbnLib.Qc
{
  public class OptionControl
  {
    private MotionFlags _Flags;

    public MotionFlags Flags
    {
      get => this._Flags;
      set => this._Flags = value;
    }

    public new string ToString() => this._Flags.ToString().Replace(",", "");
  }
}
