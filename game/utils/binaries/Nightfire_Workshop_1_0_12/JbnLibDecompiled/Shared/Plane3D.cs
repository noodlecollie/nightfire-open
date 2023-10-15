// Decompiled with JetBrains decompiler
// Type: JbnLib.Shared.Plane3D
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using System;
using System.Collections.Generic;

namespace JbnLib.Shared
{
  public class Plane3D : List<Point3D>
  {
    public Point3D A
    {
      get => this[0];
      set => this[0] = value;
    }

    public Point3D B
    {
      get => this[1];
      set => this[1] = value;
    }

    public Point3D C
    {
      get => this[2];
      set => this[2] = value;
    }

    public Point3D Normal
    {
      get
      {
        Point3D point3D = new Vector3D(new Point3D(this[0].dX - this[1].dX, this[0].dY - this[1].dY, this[0].dZ - this[1].dZ), new Point3D(this[0].dX - this[2].dX, this[0].dY - this[2].dY, this[0].dZ - this[2].dZ)).CrossProduct();
        return point3D / point3D.Magnitude;
      }
    }

    public double Distance
    {
      get
      {
        Point3D normal = this.Normal;
        return -(normal.dX * this[0].dX + normal.dY * this[0].dY + normal.dZ * this[0].dZ);
      }
    }

    public Plane3D()
    {
      this.Add(new Point3D());
      this.Add(new Point3D());
      this.Add(new Point3D());
    }

    public Plane3D(Point3D a, Point3D b, Point3D c)
    {
      this.Add(a);
      this.Add(b);
      this.Add(c);
    }

    public double DistanceFrom(Point3D point)
    {
      Point3D normal = this.Normal;
      return (normal.dX * point.dX + normal.dY * point.dY + normal.dZ * point.dZ + this.Distance) / Math.Sqrt(Math.Pow(normal.dX, 2.0) + Math.Pow(normal.dY, 2.0) + Math.Pow(normal.dZ, 2.0));
    }

    public new string ToString() => string.Format("( {0} ) ( {1} ) ( {2} )", (object) this[0].ToString(), (object) this[1].ToString(), (object) this[2].ToString());
  }
}
