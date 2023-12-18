// Decompiled with JetBrains decompiler
// Type: JbnLib.Shared.Point3D
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Interfaces;
using JbnLib.Lang;
using System;
using System.Collections.Generic;

namespace JbnLib.Shared
{
  [Serializable]
  public class Point3D : IStructure, IComparer<Point3D>
  {
    private double[] _vals = new double[3];

    public static int Length => 12;

    public float this[int i]
    {
      get => Convert.ToSingle(this._vals[i]);
      set => this._vals[i] = (double) value;
    }

    public float X
    {
      get => Convert.ToSingle(this._vals[0]);
      set => this._vals[0] = (double) value;
    }

    public float Y
    {
      get => Convert.ToSingle(this._vals[1]);
      set => this._vals[1] = (double) value;
    }

    public float Z
    {
      get => Convert.ToSingle(this._vals[2]);
      set => this._vals[2] = (double) value;
    }

    public double dX
    {
      get => this._vals[0];
      set => this._vals[0] = value;
    }

    public double dY
    {
      get => this._vals[1];
      set => this._vals[1] = value;
    }

    public double dZ
    {
      get => this._vals[2];
      set => this._vals[2] = value;
    }

    public double Magnitude
    {
      get
      {
        double num = this.SumComponentSqrs();
        return num != 1.0 ? Math.Sqrt(this.SumComponentSqrs()) : num;
      }
    }

    public bool IsNull() => this._vals[0] == 0.0 && this._vals[1] == 0.0 && this._vals[2] == 0.0;

    public Point3D()
    {
      this._vals[0] = 0.0;
      this._vals[1] = 0.0;
      this._vals[2] = 0.0;
    }

    public Point3D(Point3D point)
    {
      this._vals[0] = point.dX;
      this._vals[1] = point.dY;
      this._vals[2] = point.dZ;
    }

    public Point3D(byte[] raw)
    {
      this._vals[0] = (double) BitConverter.ToSingle(raw, 0);
      this._vals[1] = (double) BitConverter.ToSingle(raw, 4);
      this._vals[2] = (double) BitConverter.ToSingle(raw, 8);
    }

    public Point3D(float x, float y, float z)
    {
      this._vals[0] = (double) x;
      this._vals[1] = (double) y;
      this._vals[2] = (double) z;
    }

    public Point3D(double x, double y, double z)
    {
      this._vals[0] = x;
      this._vals[1] = y;
      this._vals[2] = z;
    }

    public Point3D(float[] xyz)
    {
      if (xyz.Length != 3)
        Messages.ThrowException("Shared.Point3D.New(float[])", Messages.POINT3D_XYZ);
      this._vals[0] = (double) xyz[0];
      this._vals[1] = (double) xyz[1];
      this._vals[2] = (double) xyz[2];
    }

    public Point3D(double[] xyz)
    {
      if (xyz.Length != 3)
        Messages.ThrowException("Shared.Point3D.New(double[])", Messages.POINT3D_XYZ);
      this._vals[0] = xyz[0];
      this._vals[1] = xyz[1];
      this._vals[2] = xyz[2];
    }

    public static bool operator ==(Point3D a, Point3D b) => a.dX == b.dX && a.dY == b.dY && a.dZ == b.dZ;

    public static bool operator !=(Point3D a, Point3D b) => a.dX != b.dX || a.dY != b.dY || a.dZ != b.dZ;

    public static Point3D operator +(Point3D a, Point3D b) => new Point3D(a.dX + b.dX, a.dY + b.dY, a.dZ + b.dZ);

    public static Point3D operator +(Point3D a) => new Point3D(a.dX, a.dY, a.dZ);

    public static Point3D operator -(Point3D a, Point3D b) => new Point3D(a.dX - b.dX, a.dY - b.dY, a.dZ - b.dZ);

    public static Point3D operator -(Point3D a) => new Point3D(-a.dX, -a.dY, -a.dZ);

    public static Point3D operator *(Point3D a, double b) => new Point3D(a.dX * b, a.dY * b, a.dZ * b);

    public static Point3D operator *(double a, Point3D b) => b * a;

    public static Point3D operator /(Point3D a, double b) => new Point3D(a.dX / b, a.dY / b, a.dZ / b);

    public static Point3D operator /(double a, Point3D b) => new Point3D(a / b.dX, a / b.dY, a / b.dZ);

    public double SumComponents() => this._vals[0] + this._vals[1] + this._vals[2];

    public Point3D SqrComponents() => new Point3D(this._vals[0] * this._vals[0], this._vals[1] * this._vals[1], this._vals[2] * this._vals[2]);

    public double SumComponentSqrs() => this.SqrComponents().SumComponents();

    public Point3D Normalize()
    {
      if (this.Magnitude == 0.0)
        throw new DivideByZeroException();
      double num = 1.0 / this.Magnitude;
      return new Point3D(this._vals[0] * num, this._vals[1] * num, this._vals[2] * num);
    }

    public Point3D Yaw(double degrees) => new Point3D(this._vals[2] * Math.Sin(degrees) + this._vals[0] * Math.Cos(degrees), this._vals[1], this._vals[2] * Math.Cos(degrees) - this._vals[0] * Math.Sin(degrees));

    public Point3D Pitch(double degrees) => new Point3D(this._vals[0], this._vals[1] * Math.Cos(degrees) - this._vals[2] * Math.Sin(degrees), this._vals[1] * Math.Sin(degrees) + this._vals[2] * Math.Cos(degrees));

    public Point3D Roll(double degrees) => new Point3D(this._vals[0] * Math.Cos(degrees) - this._vals[1] * Math.Sin(degrees), this._vals[0] * Math.Sin(degrees) + this._vals[1] * Math.Cos(degrees), this._vals[2]);

    public double Abs() => this.Magnitude;

    public override bool Equals(object o)
    {
      Point3D point3D = (Point3D) o;
      return point3D.dX == this._vals[0] && point3D.dY == this._vals[1] && point3D.dZ == this._vals[2];
    }

    public override int GetHashCode() => (this._vals[0] * this._vals[1] * this._vals[2]).GetHashCode();

    public byte[] GetBytes()
    {
      byte[] bytes = new byte[Point3D.Length];
      BitConverter.GetBytes(Convert.ToSingle(this._vals[0])).CopyTo((Array) bytes, 0);
      BitConverter.GetBytes(Convert.ToSingle(this._vals[1])).CopyTo((Array) bytes, 4);
      BitConverter.GetBytes(Convert.ToSingle(this._vals[2])).CopyTo((Array) bytes, 8);
      return bytes;
    }

    public new string ToString()
    {
      if (double.IsNaN(this._vals[0]))
        this._vals[0] = 0.0;
      if (double.IsNaN(this._vals[1]))
        this._vals[1] = 0.0;
      if (double.IsNaN(this._vals[2]))
        this._vals[2] = 0.0;
      return string.Format("{0:f6} {1:f6} {2:f6}", (object) Convert.ToSingle(this._vals[0]), (object) Convert.ToSingle(this._vals[1]), (object) Convert.ToSingle(this._vals[2]));
    }

    public string ToStringNoPad()
    {
      if (double.IsNaN(this._vals[0]))
        this._vals[0] = 0.0;
      if (double.IsNaN(this._vals[1]))
        this._vals[1] = 0.0;
      if (double.IsNaN(this._vals[2]))
        this._vals[2] = 0.0;
      return string.Format("{0} {1} {2}", (object) Convert.ToSingle(this._vals[0]), (object) Convert.ToSingle(this._vals[1]), (object) Convert.ToSingle(this._vals[2]));
    }

    public int Compare(Point3D a, Point3D b)
    {
      if (a.dX > b.dX)
        return 1;
      if (a.dX < b.dX)
        return -1;
      if (StaticMethods.CloseEnough(a.dX, b.dX, 6))
      {
        if (a.dY > b.dY)
          return 1;
        if (a.dY < b.dY)
          return -1;
        if (StaticMethods.CloseEnough(a.dY, b.dY, 6))
        {
          if (a.dZ > b.dZ)
            return 1;
          if (a.dZ < b.dZ)
            return -1;
        }
      }
      return 0;
    }
  }
}
