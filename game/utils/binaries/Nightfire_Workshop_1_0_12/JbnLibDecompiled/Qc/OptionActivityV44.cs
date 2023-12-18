// Decompiled with JetBrains decompiler
// Type: JbnLib.Qc.OptionActivityV44
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

namespace JbnLib.Qc
{
  public class OptionActivityV44
  {
    private ActivityV44 _Activity;
    private float _ActivityWeight;

    public ActivityV44 Activity
    {
      get => this._Activity;
      set => this._Activity = value;
    }

    public float ActivityWeight
    {
      get => this._ActivityWeight;
      set => this._ActivityWeight = value;
    }

    public OptionActivityV44()
    {
    }

    public OptionActivityV44(ActivityV44 activity, float weight)
    {
      this._Activity = activity;
      this._ActivityWeight = weight;
    }

    public new string ToString() => string.Format("{0} {1}", (object) this._Activity, (object) this._ActivityWeight);
  }
}
