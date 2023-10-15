// Decompiled with JetBrains decompiler
// Type: JbnLib.Shared.Vector3D
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using System;
using System.Collections.Generic;

namespace JbnLib.Shared
{
  public class Vector3D : IComparer<Point3D>
  {
    private Point3D[] _vec = new Point3D[2];

    public Point3D this[int i]
    {
      get => this._vec[i];
      set => this._vec[i] = value;
    }

    public Point3D Initial
    {
      get => this._vec[0];
      set => this._vec[0] = value;
    }

    public Point3D Terminal
    {
      get => this._vec[1];
      set => this._vec[1] = value;
    }

    public Vector3D()
    {
      this._vec[0] = new Point3D();
      this._vec[1] = new Point3D();
    }

    public Vector3D(Point3D terminal)
    {
      this._vec[0] = new Point3D();
      this._vec[1] = terminal;
    }

    public Vector3D(Point3D initial, Point3D terminal)
    {
      this._vec[0] = initial;
      this._vec[1] = terminal;
    }

    public double Angle() => Math.Acos(new Vector3D(this.Initial.Normalize(), this.Terminal.Normalize()).DotProduct());

    public double Distance() => Math.Sqrt((this._vec[1].dX - this._vec[0].dX) * (this._vec[1].dX - this._vec[0].dX) + (this._vec[1].dY - this._vec[0].dY) * (this._vec[1].dY - this._vec[0].dY) + (this._vec[1].dZ - this._vec[0].dZ) * (this._vec[1].dZ - this._vec[0].dZ));

    public Point3D CrossProduct() => new Point3D(this._vec[0].dY * this._vec[1].dZ - this._vec[0].dZ * this._vec[1].dY, this._vec[0].dZ * this._vec[1].dX - this._vec[0].dX * this._vec[1].dZ, this._vec[0].dX * this._vec[1].dY - this._vec[0].dY * this._vec[1].dX);

    public double DotProduct() => this._vec[0].dX * this._vec[1].dX + this._vec[0].dY * this._vec[1].dY + this._vec[0].dZ * this._vec[1].dZ;

    public Point3D Difference() => this.Terminal - this.Initial;

    public int Compare(Point3D a, Point3D b)
    {
      double num1 = new Vector3D((this._vec[1] - this._vec[0]).Normalize(), (a - this._vec[0]).Normalize()).Angle();
      double num2 = new Vector3D((this._vec[1] - this._vec[0]).Normalize(), (b - this._vec[0]).Normalize()).Angle();
      if (num1 > num2)
        return 1;
      return num1 < num2 ? -1 : 0;
    }

    public bool IsNull() => this._vec[0].IsNull() && this._vec[1].IsNull();

    public override string ToString() => Math.Round((double) this._vec[0][0], 6).ToString() + " " + (object) Math.Round((double) this._vec[0][1], 6) + " " + (object) Math.Round((double) this._vec[0][2], 6) + " - " + (object) Math.Round((double) this._vec[1][0], 6) + " " + (object) Math.Round((double) this._vec[1][1], 6) + " " + (object) Math.Round((double) this._vec[1][2], 6);
  }
}
