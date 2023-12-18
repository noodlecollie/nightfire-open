// Decompiled with JetBrains decompiler
// Type: JbnLib.Smd.Time
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Shared;

namespace JbnLib.Smd
{
  public class Time
  {
    private int _Node;
    private Point3D _Position;
    private Point3D _Rotation;

    public int Node
    {
      get => this._Node;
      set => this._Node = value;
    }

    public Point3D Position
    {
      get => this._Position;
      set => this._Position = value;
    }

    public Point3D Rotation
    {
      get => this._Rotation;
      set => this._Rotation = value;
    }

    public Time()
    {
      this._Node = 0;
      this._Position = new Point3D();
      this._Rotation = new Point3D();
    }

    public Time(int node, Point3D pos, Point3D rot)
    {
      this._Node = node;
      this._Position = pos;
      this._Rotation = rot;
    }

    public new string ToString() => this._Node.ToString().PadLeft(3, ' ') + " " + this._Position.ToString() + " " + this._Rotation.ToString();
  }
}
