// Decompiled with JetBrains decompiler
// Type: JbnLib.Fgd.Types.Size
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Lang;

namespace JbnLib.Fgd.Types
{
  public class Size
  {
    public Point3D Point1;
    public Point3D Point2;
    public bool HasValue;

    public Size()
    {
    }

    public Size(Point3D point1, Point3D point2)
    {
      this.Point1 = point1;
      this.Point2 = point2;
      this.HasValue = true;
    }

    public Size(string size)
    {
      string[] strArray = size.Split(',');
      if (strArray.Length == 2)
      {
        this.Point1 = new Point3D(strArray[0].Trim());
        this.Point2 = new Point3D(strArray[1].Trim());
        this.HasValue = true;
      }
      else if (strArray.Length == 1)
      {
        this.Point1 = new Point3D(0, 0, 0);
        this.Point2 = new Point3D(strArray[0].Trim());
        this.HasValue = true;
      }
      else
        Messages.ThrowException("Fgd.Types.Size.New(string)", Messages.COMPARE_ARRAY + Messages.EXPECTING + "1 or 2 " + Messages.GOT + (object) strArray.Length);
    }

    public new string ToString() => this.Point1.ToString() + ", " + this.Point2.ToString();
  }
}
