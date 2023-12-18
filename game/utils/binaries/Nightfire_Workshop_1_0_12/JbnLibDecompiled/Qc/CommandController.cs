// Decompiled with JetBrains decompiler
// Type: JbnLib.Qc.CommandController
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Mdl;

namespace JbnLib.Qc
{
  public class CommandController
  {
    public const string Command = "$controller";
    private int _Index;
    private string _Bone;
    private MotionFlags _Type;
    private float _Start;
    private float _End;

    public int Index
    {
      get => this._Index;
      set => this._Index = value;
    }

    public string Bone
    {
      get => this._Bone;
      set => this._Bone = value;
    }

    public MotionFlags Type
    {
      get => this._Type;
      set => this._Type = value;
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

    public CommandController()
    {
    }

    public CommandController(int index, string bone, MotionFlags type, float start, float end)
    {
      this._Index = index;
      this._Bone = bone;
      this._Type = type;
      this._Start = start;
      this._End = end;
    }

    public new string ToString() => string.Format("{0} {1} \"{2}\" {3} {4} {5}", (object) "$controller", (object) this._Index, (object) this._Bone, (object) this._Type, (object) this._Start, (object) this._End);
  }
}
