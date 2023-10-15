// Decompiled with JetBrains decompiler
// Type: JbnLib.Qc.CommandOrigin
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Shared;

namespace JbnLib.Qc
{
  public class CommandOrigin
  {
    public const string Command = "$origin";
    private Point3D _Value = new Point3D();

    public Point3D Value
    {
      get => this._Value;
      set => this._Value = value;
    }

    public CommandOrigin()
    {
    }

    public CommandOrigin(Point3D value) => this._Value = value;

    public new string ToString() => string.Format("{0} {1}", (object) "$origin", (object) this._Value.ToString());
  }
}
