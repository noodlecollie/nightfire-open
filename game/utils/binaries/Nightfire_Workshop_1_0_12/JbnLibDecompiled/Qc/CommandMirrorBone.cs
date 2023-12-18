// Decompiled with JetBrains decompiler
// Type: JbnLib.Qc.CommandMirrorBone
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

namespace JbnLib.Qc
{
  public class CommandMirrorBone
  {
    public const string Command = "$mirrorbone";
    private string _Bone;

    public string Bone
    {
      get => this._Bone;
      set => this._Bone = value;
    }

    public CommandMirrorBone()
    {
    }

    public CommandMirrorBone(string bone) => this._Bone = bone;

    public new string ToString() => string.Format("{0} {1}", (object) "$mirrorbone", (object) this._Bone);
  }
}
