// Decompiled with JetBrains decompiler
// Type: JbnLib.Smd.LinkV44
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

namespace JbnLib.Smd
{
  public class LinkV44
  {
    private sbyte _Bone;
    private float _Weight;

    public sbyte Bone
    {
      get => this._Bone;
      set => this._Bone = value;
    }

    public float Weight
    {
      get => this._Weight;
      set => this._Weight = value;
    }

    public LinkV44()
    {
      this._Bone = (sbyte) -1;
      this._Weight = 0.0f;
    }

    public LinkV44(sbyte bone, float weight)
    {
      this._Bone = bone;
      this._Weight = weight;
    }

    public new string ToString() => this._Bone.ToString() + " " + string.Format("{0:f6}", (object) this._Weight);
  }
}
