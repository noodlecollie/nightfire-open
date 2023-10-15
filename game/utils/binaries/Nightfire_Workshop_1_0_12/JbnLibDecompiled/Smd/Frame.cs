// Decompiled with JetBrains decompiler
// Type: JbnLib.Smd.Frame
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using System.Collections.Generic;

namespace JbnLib.Smd
{
  public class Frame
  {
    private int _FrameValue;
    private List<Time> _Bones;

    public int FrameValue
    {
      get => this._FrameValue;
      set => this._FrameValue = value;
    }

    public List<Time> Bones
    {
      get => this._Bones;
      set => this._Bones = value;
    }

    public Frame() => this._Bones = new List<Time>();

    public Frame(int framevalue, List<Time> bones)
    {
      this._FrameValue = framevalue;
      this._Bones = bones;
    }

    public new string ToString()
    {
      string str = "time " + (object) this._FrameValue;
      foreach (Time bone in this._Bones)
        str = str + "\r\n" + bone.ToString();
      return str;
    }
  }
}
