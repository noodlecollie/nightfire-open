// Decompiled with JetBrains decompiler
// Type: JbnLib.Shared.Vector2D
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using System;
using System.Collections.Generic;

namespace JbnLib.Shared
{
  public class Vector2D : List<Point>, IComparer<Vector2D>
  {
    public Point Initial
    {
      get => this[0];
      set => this[0] = value;
    }

    public Point Terminal
    {
      get => this[1];
      set => this[1] = value;
    }

    public Vector2D()
    {
      this.Add(new Point());
      this.Add(new Point());
    }

    public Vector2D(Point terminal)
    {
      this.Add(new Point());
      this.Add(terminal);
    }

    public Vector2D(Point initial, Point terminal)
    {
      this.Add(initial);
      this.Add(terminal);
    }

    public double Angle() => Math.Acos(new Vector2D(this.Initial.Normalize(), this.Terminal.Normalize()).DotProduct());

    public double Distance() => Math.Sqrt(((double) this[1][0] - (double) this[0][0]) * ((double) this[1][0] - (double) this[0][0]) + ((double) this[1][1] - (double) this[0][1]) * ((double) this[1][1] - (double) this[0][1]));

    public double DotProduct() => (double) this[0][0] * (double) this[1][0] + (double) this[0][1] * (double) this[1][1];

    public Point Difference() => this.Terminal - this.Initial;

    public int Compare(Vector2D a, Vector2D b)
    {
      double num1 = new Vector2D((this[1] - this[0]).Normalize(), (a[1] - this[0]).Normalize()).Angle();
      double num2 = new Vector2D((this[1] - this[0]).Normalize(), (b[1] - this[0]).Normalize()).Angle();
      if (num1 > num2)
        return 1;
      return num1 < num2 ? -1 : 0;
    }

    public bool IsNull() => this[0].IsNull() && this[1].IsNull();

    public override string ToString() => Math.Round((double) this[0][0], 6).ToString() + " " + (object) Math.Round((double) this[0][1], 6) + " - " + (object) Math.Round((double) this[1][0], 6) + " " + (object) Math.Round((double) this[1][1], 6);
  }
}
