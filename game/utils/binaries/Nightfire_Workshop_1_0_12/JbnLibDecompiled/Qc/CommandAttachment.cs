// Decompiled with JetBrains decompiler
// Type: JbnLib.Qc.CommandAttachment
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Shared;

namespace JbnLib.Qc
{
  public class CommandAttachment
  {
    public const string Command = "$attachment";
    private string _Name;
    private string _Bone;
    private Point3D _Position = new Point3D();

    public string Name
    {
      get => this._Name;
      set => this._Name = value;
    }

    public string Bone
    {
      get => this._Bone;
      set => this._Bone = value;
    }

    public Point3D Position
    {
      get => this._Position;
      set => this._Position = value;
    }

    public CommandAttachment()
    {
    }

    public CommandAttachment(string name, string bone, Point3D position)
    {
      this._Name = name;
      this._Bone = bone;
      this._Position = position;
    }

    public new string ToString() => string.Format("{0} \"{1}\" \"{2}\" {3}", (object) "$attachment", (object) this._Name, (object) this._Bone, (object) this._Position.ToString());
  }
}
