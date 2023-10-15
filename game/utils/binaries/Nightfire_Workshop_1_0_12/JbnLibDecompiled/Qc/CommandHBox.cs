// Decompiled with JetBrains decompiler
// Type: JbnLib.Qc.CommandHBox
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Shared;

namespace JbnLib.Qc
{
  public class CommandHBox
  {
    public const string Command = "$hbox";
    private int _Group;
    private string _Bone;
    private Point3D _Min = new Point3D();
    private Point3D _Max = new Point3D();

    public int Group
    {
      get => this._Group;
      set => this._Group = value;
    }

    public string Bone
    {
      get => this._Bone;
      set => this._Bone = value;
    }

    public Point3D Min
    {
      get => this._Min;
      set => this._Min = value;
    }

    public Point3D Max
    {
      get => this._Max;
      set => this._Max = value;
    }

    public CommandHBox()
    {
    }

    public CommandHBox(int group, string bone, Point3D min, Point3D max)
    {
      this._Group = group;
      this._Bone = bone;
      this._Min = min;
      this._Max = max;
    }

    public new string ToString() => string.Format("{0} {1} \"{2}\" {3} {4}", (object) "$hbox", (object) this._Group, (object) this._Bone, (object) this._Min.ToString(), (object) this.Max.ToString());
  }
}
