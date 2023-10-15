// Decompiled with JetBrains decompiler
// Type: JbnLib.Smd.BlendV11
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

namespace JbnLib.Smd
{
  public class BlendV11
  {
    private sbyte _Bone;
    private float _Scale;

    public sbyte Bone
    {
      get => this._Bone;
      set => this._Bone = value;
    }

    public float Scale
    {
      get => this._Scale;
      set => this._Scale = value;
    }

    public BlendV11()
    {
      this._Bone = (sbyte) -1;
      this._Scale = 0.0f;
    }

    public BlendV11(sbyte bone, float scale)
    {
      this._Bone = bone;
      this._Scale = scale;
    }

    public new string ToString() => this._Bone.ToString().PadLeft(3) + " " + string.Format("{0:f4}", (object) this._Scale).PadLeft(8);
  }
}
