// Decompiled with JetBrains decompiler
// Type: JbnLib.Shared.Quaternion
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
  public class Quaternion : IStructure, IComparer<Quaternion>
  {
    private double[] _vals = new double[4];

    public static int Length => 16;

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

    public float W
    {
      get => Convert.ToSingle(this._vals[3]);
      set => this._vals[3] = (double) value;
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

    public double dW
    {
      get => this._vals[3];
      set => this._vals[3] = value;
    }

    public double Magnitude
    {
      get
      {
        double num = this.SumComponentSqrs();
        return num != 1.0 ? Math.Sqrt(this.SumComponentSqrs()) : num;
      }
    }

    public bool IsNull() => this._vals[0] == 0.0 && this._vals[1] == 0.0 && this._vals[2] == 0.0 && this._vals[3] == 0.0;

    public Quaternion() => this._vals = new double[4];

    public Quaternion(Quaternion quat) => this._vals = new double[4]
    {
      quat.dX,
      quat.dY,
      quat.dZ,
      quat.dW
    };

    public Quaternion(Point3D angles)
    {
      double num1 = angles.dZ * 0.5;
      double num2 = Math.Sin(num1);
      double num3 = Math.Cos(num1);
      double num4 = angles.dY * 0.5;
      double num5 = Math.Sin(num4);
      double num6 = Math.Cos(num4);
      double num7 = angles.dX * 0.5;
      double num8 = Math.Sin(num7);
      double num9 = Math.Cos(num7);
      this._vals = new double[4]
      {
        num8 * num6 * num3 - num9 * num5 * num2,
        num9 * num5 * num3 + num8 * num6 * num2,
        num9 * num6 * num2 - num8 * num5 * num3,
        num9 * num6 * num3 + num8 * num5 * num2
      };
    }

    public Quaternion(byte[] raw)
    {
      this._vals[0] = (double) BitConverter.ToSingle(raw, 0);
      this._vals[1] = (double) BitConverter.ToSingle(raw, 4);
      this._vals[2] = (double) BitConverter.ToSingle(raw, 8);
      this._vals[3] = (double) BitConverter.ToSingle(raw, 12);
    }

    public Quaternion(float x, float y, float z, float w) => this._vals = new double[4]
    {
      (double) x,
      (double) y,
      (double) z,
      (double) w
    };

    public Quaternion(double x, double y, double z, double w) => this._vals = new double[4]
    {
      x,
      y,
      z,
      w
    };

    public Quaternion(float[] xyzw)
    {
      if (xyzw.Length != 4)
        Messages.ThrowException("Shared.Quaternion.New(float[])", Messages.POINT3D_XYZ);
      this._vals = new double[4]
      {
        (double) xyzw[0],
        (double) xyzw[1],
        (double) xyzw[2],
        (double) xyzw[3]
      };
    }

    public Quaternion(double[] xyzw)
    {
      if (xyzw.Length != 4)
        Messages.ThrowException("Shared.Quaternion.New(double[])", Messages.POINT3D_XYZ);
      this._vals = new double[4]
      {
        xyzw[0],
        xyzw[1],
        xyzw[2],
        xyzw[3]
      };
    }

    public static bool operator ==(Quaternion a, Quaternion b) => a.dX == b.dX && a.dY == b.dY && a.dZ == b.dZ && a.dW == b.dW;

    public static bool operator !=(Quaternion a, Quaternion b) => a.dX != b.dX || a.dY != b.dY || a.dZ != b.dZ || a.dW != b.dW;

    public static Quaternion operator +(Quaternion a, Quaternion b) => new Quaternion(a.dX + b.dX, a.dY + b.dY, a.dZ + b.dZ, a.dW + b.dW);

    public static Quaternion operator +(Quaternion a) => new Quaternion(a.dX, a.dY, a.dZ, a.dW);

    public static Quaternion operator -(Quaternion a, Quaternion b) => new Quaternion(a.dX - b.dX, a.dY - b.dY, a.dZ - b.dZ, a.dW - b.dW);

    public static Quaternion operator -(Quaternion a) => new Quaternion(-a.dX, -a.dY, -a.dZ, -a.dW);

    public static Quaternion operator *(Quaternion a, double b) => new Quaternion(a.dX * b, a.dY * b, a.dZ * b, a.dW * b);

    public static Quaternion operator *(double a, Quaternion b) => b * a;

    public static Quaternion operator /(Quaternion a, double b) => new Quaternion(a.dX / b, a.dY / b, a.dZ / b, a.dW / b);

    public static Quaternion operator /(double a, Quaternion b) => new Quaternion(a / b.dX, a / b.dY, a / b.dZ, a / b.dW);

    public double SumComponents() => this._vals[0] + this._vals[1] + this._vals[2] + this._vals[3];

    public Quaternion SqrComponents() => new Quaternion(this._vals[0] * this._vals[0], this._vals[1] * this._vals[1], this._vals[2] * this._vals[2], this._vals[3] * this._vals[3]);

    public double SumComponentSqrs() => this.SqrComponents().SumComponents();

    public Quaternion Normalize()
    {
      if (this.Magnitude == 0.0)
        throw new DivideByZeroException();
      double num = 1.0 / this.Magnitude;
      return new Quaternion(this._vals[0] * num, this._vals[1] * num, this._vals[2] * num, this._vals[3] * num);
    }

    public double Abs() => this.Magnitude;

    public override bool Equals(object o)
    {
      Quaternion quaternion = (Quaternion) o;
      return quaternion.dX == this._vals[0] && quaternion.dY == this._vals[1] && quaternion.dZ == this._vals[2] && quaternion.dW == this._vals[3];
    }

    public override int GetHashCode() => (this._vals[0] * this._vals[1] * this._vals[2] * this._vals[3]).GetHashCode();

    public byte[] GetBytes()
    {
      byte[] bytes = new byte[Quaternion.Length];
      BitConverter.GetBytes(Convert.ToSingle(this._vals[0])).CopyTo((Array) bytes, 0);
      BitConverter.GetBytes(Convert.ToSingle(this._vals[1])).CopyTo((Array) bytes, 4);
      BitConverter.GetBytes(Convert.ToSingle(this._vals[2])).CopyTo((Array) bytes, 8);
      BitConverter.GetBytes(Convert.ToSingle(this._vals[3])).CopyTo((Array) bytes, 12);
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
      if (double.IsNaN(this._vals[3]))
        this._vals[3] = 0.0;
      return string.Format("{0:f6} {1:f6} {2:f6} {3:f6}", (object) Convert.ToSingle(this._vals[0]), (object) Convert.ToSingle(this._vals[1]), (object) Convert.ToSingle(this._vals[2]), (object) Convert.ToSingle(this._vals[3]));
    }

    public string ToStringNoPad()
    {
      if (double.IsNaN(this._vals[0]))
        this._vals[0] = 0.0;
      if (double.IsNaN(this._vals[1]))
        this._vals[1] = 0.0;
      if (double.IsNaN(this._vals[2]))
        this._vals[2] = 0.0;
      if (double.IsNaN(this._vals[3]))
        this._vals[2] = 0.0;
      return string.Format("{0} {1} {2} {3}", (object) Convert.ToSingle(this._vals[0]), (object) Convert.ToSingle(this._vals[1]), (object) Convert.ToSingle(this._vals[2]), (object) Convert.ToSingle(this._vals[3]));
    }

    public int Compare(Quaternion a, Quaternion b)
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
          if (StaticMethods.CloseEnough(a.dZ, b.dZ, 6))
          {
            if (a.dW > b.dW)
              return 1;
            if (a.dW < b.dW)
              return -1;
          }
        }
      }
      return 0;
    }
  }
}
