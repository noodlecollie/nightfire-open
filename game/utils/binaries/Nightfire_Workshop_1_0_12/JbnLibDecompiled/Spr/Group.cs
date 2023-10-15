// Decompiled with JetBrains decompiler
// Type: JbnLib.Spr.Group
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using System.Collections.Generic;

namespace JbnLib.Spr
{
  public class Group
  {
    private List<float> _Intervals = new List<float>();
    private List<Frame> _FrameCollection = new List<Frame>();

    public List<float> Intervals
    {
      get => this._Intervals;
      set => this._Intervals = value;
    }

    public List<Frame> FrameCollection
    {
      get => this._FrameCollection;
      set => this._FrameCollection = value;
    }
  }
}
