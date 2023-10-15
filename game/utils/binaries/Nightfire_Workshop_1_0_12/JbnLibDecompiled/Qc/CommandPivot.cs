// Decompiled with JetBrains decompiler
// Type: JbnLib.Qc.CommandPivot
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

namespace JbnLib.Qc
{
  public class CommandPivot
  {
    public const string Command = "$pivot";
    private int _Index;
    private string _Bone;

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

    public new string ToString() => string.Format("{0} {1} \"{2}\"", (object) "$pivot", (object) this._Index, (object) this._Bone);
  }
}
