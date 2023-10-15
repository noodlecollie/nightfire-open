// Decompiled with JetBrains decompiler
// Type: JbnLib.Fgd.Types.Point3D
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Lang;
using System;

namespace JbnLib.Fgd.Types
{
  public struct Point3D
  {
    public int X;
    public int Y;
    public int Z;

    public Point3D(int x, int y, int z)
    {
      this.X = x;
      this.Y = y;
      this.Z = z;
    }

    public Point3D(string xyz)
    {
      string[] strArray = xyz.Split(' ');
      if (strArray.Length == 3)
      {
        try
        {
          this.X = Convert.ToInt32(strArray[0]);
          this.Y = Convert.ToInt32(strArray[1]);
          this.Z = Convert.ToInt32(strArray[2]);
        }
        catch
        {
          Messages.ThrowException("Fgd.Types.Point3D.New(string)", Messages.CONVERT_INT);
        }
      }
      else
        Messages.ThrowException("Fgd.Types.Point3D.New(string)", Messages.THREE_VALUES_SPACES);
      this.X = 0;
      this.Y = 0;
      this.Z = 0;
    }

    public new string ToString() => this.X.ToString() + " " + (object) this.Y + " " + (object) this.Z;
  }
}
