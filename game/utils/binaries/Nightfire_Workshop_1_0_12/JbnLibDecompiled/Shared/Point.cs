// Decompiled with JetBrains decompiler
// Type: JbnLib.Shared.Point
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
  public class Point : List<float>, IStructure
  {
    public static int Length => 8;

    public float X
    {
      get => this[0];
      set => this[0] = value;
    }

    public float Y
    {
      get => this[1];
      set => this[1] = value;
    }

    public double Magnitude => Math.Sqrt(this.SumComponentSqrs());

    public bool IsNull() => (double) this[0] == 0.0 && (double) this[1] == 0.0;

    public Point()
    {
      this.Add(0.0f);
      this.Add(0.0f);
    }

    public Point(Point point)
    {
      this.Add(point.X);
      this.Add(point.Y);
    }

    public Point(byte[] raw)
    {
      this.Add(BitConverter.ToSingle(raw, 0));
      this.Add(BitConverter.ToSingle(raw, 4));
    }

    public Point(float x, float y)
    {
      this.Add(x);
      this.Add(y);
    }

    public Point(float[] xy)
    {
      if (xy.Length != 2)
        Messages.ThrowException("Shared.Point.New(float[])", Messages.POINT_XY);
      this.Add(xy[0]);
      this.Add(xy[1]);
    }

    public static bool operator ==(Point a, Point b) => (double) a[0] == (double) b[0] && (double) a[1] == (double) b[1];

    public static bool operator !=(Point a, Point b) => (double) a[0] != (double) b[0] || (double) a[1] != (double) b[1] || (double) a[2] != (double) b[2];

    public static Point operator +(Point a, Point b) => new Point(a[0] + b[0], a[1] + b[1]);

    public static Point operator +(Point a) => new Point(a.X, a.Y);

    public static Point operator -(Point a, Point b) => new Point(a[0] - b[0], a[1] - b[1]);

    public static Point operator -(Point a) => new Point(-a.X, -a.Y);

    public static Point operator *(Point a, double b) => new Point(Convert.ToSingle((double) a[0] * b), Convert.ToSingle((double) a[1] * b));

    public static Point operator *(double a, Point b) => b * a;

    public static Point operator /(Point a, double b) => new Point(Convert.ToSingle((double) a[0] / b), Convert.ToSingle((double) a[1] / b));

    public static Point operator /(double a, Point b) => new Point(Convert.ToSingle(a / (double) b[0]), Convert.ToSingle(a / (double) b[1]));

    public double SumComponents() => (double) this[0] + (double) this[1];

    public Point SqrComponents() => new Point(this[0] * this[0], this[1] * this[1]);

    public double SumComponentSqrs() => this.SqrComponents().SumComponents();

    public Point Normalize()
    {
      if (this.Magnitude == 0.0)
        throw new DivideByZeroException();
      double num = 1.0 / this.Magnitude;
      return new Point(Convert.ToSingle((double) this[0] * num), Convert.ToSingle((double) this[1] * num));
    }

    public double Abs() => this.Magnitude;

    public override bool Equals(object o)
    {
      Point point = (Point) o;
      return (double) point[0] == (double) this[0] && (double) point[1] == (double) this[1];
    }

    public override int GetHashCode() => Convert.ToInt32(this[0] * this[1]);

    public byte[] GetBytes()
    {
      byte[] bytes = new byte[Point.Length];
      BitConverter.GetBytes(this[0]).CopyTo((Array) bytes, 0);
      BitConverter.GetBytes(this[1]).CopyTo((Array) bytes, 4);
      return bytes;
    }

    public new string ToString()
    {
      if (float.IsNaN(this[0]))
        this[0] = 0.0f;
      if (float.IsNaN(this[1]))
        this[1] = 0.0f;
      return string.Format("{0:f6} {1:f6}", (object) this[0], (object) this[1]);
    }

    public string ToStringNoPad()
    {
      if (float.IsNaN(this[0]))
        this[0] = 0.0f;
      if (float.IsNaN(this[1]))
        this[1] = 0.0f;
      return string.Format("{0} {1}", (object) this[0], (object) this[1]);
    }
  }
}
