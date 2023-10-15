// Decompiled with JetBrains decompiler
// Type: JbnLib.Shared.AmbientColor
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Lang;
using System;
using System.Collections.Generic;
using System.ComponentModel;

namespace JbnLib.Shared
{
  [TypeConverter(typeof (AmbientColorConverter))]
  public class AmbientColor : List<float>
  {
    public float R => this[0];

    public float G => this[1];

    public float B => this[2];

    public AmbientColor()
    {
      this.Add(0.0f);
      this.Add(0.0f);
      this.Add(0.0f);
    }

    public AmbientColor(float r, float g, float b)
    {
      this.Add(r);
      this.Add(g);
      this.Add(b);
    }

    public AmbientColor(float[] rgb)
    {
      if (rgb.Length != 3)
        Messages.ThrowException("Shared.AmbientColor.New(float[])", Messages.COLOR_RGB);
      this.Add(rgb[0]);
      this.Add(rgb[1]);
      this.Add(rgb[2]);
    }

    public AmbientColor(byte r, byte g, byte b)
    {
      this.Add(Convert.ToSingle((float) r / (float) byte.MaxValue));
      this.Add(Convert.ToSingle((float) g / (float) byte.MaxValue));
      this.Add(Convert.ToSingle((float) b / (float) byte.MaxValue));
    }

    public AmbientColor(byte[] rgb)
    {
      if (rgb.Length != 3)
        Messages.ThrowException("Shared.AmbientColor.New(byte[])", Messages.COLOR_RGB);
      this.Add(Convert.ToSingle((float) rgb[0] / (float) byte.MaxValue));
      this.Add(Convert.ToSingle((float) rgb[1] / (float) byte.MaxValue));
      this.Add(Convert.ToSingle((float) rgb[2] / (float) byte.MaxValue));
    }

    public AmbientColor(string rgb)
    {
      string[] strArray = rgb.Split(' ');
      if (strArray.Length != 3)
        Messages.ThrowException("Shared.AmbientColor.New(string)", Messages.COLOR_RGB);
      this.Add(Convert.ToSingle(strArray[0]));
      this.Add(Convert.ToSingle(strArray[1]));
      this.Add(Convert.ToSingle(strArray[2]));
    }

    public AmbientColor(int argb)
    {
      byte[] bytes = BitConverter.GetBytes(argb);
      this.Add(Convert.ToSingle((float) bytes[2] / (float) byte.MaxValue));
      this.Add(Convert.ToSingle((float) bytes[1] / (float) byte.MaxValue));
      this.Add(Convert.ToSingle((float) bytes[0] / (float) byte.MaxValue));
    }

    public static bool operator ==(AmbientColor a, AmbientColor b) => (double) a[0] == (double) b[0] && (double) a[1] == (double) b[1] && (double) a[2] == (double) b[2];

    public static bool operator !=(AmbientColor a, AmbientColor b) => (double) a[0] != (double) b[0] || (double) a[1] != (double) b[1] || (double) a[2] != (double) b[2];

    public bool IsBlack => this == new AmbientColor(0.0f, 0.0f, 0.0f);

    public new string ToString() => string.Format("{0:f6} {1:f6} {2:f6}", (object) this[0], (object) this[1], (object) this[2]);

    public byte[] GetRGB() => new byte[3]
    {
      Convert.ToByte(this[0] * (float) byte.MaxValue),
      Convert.ToByte(this[1] * (float) byte.MaxValue),
      Convert.ToByte(this[2] * (float) byte.MaxValue)
    };

    public int GetARGB() => BitConverter.ToInt32(new byte[4]
    {
      Convert.ToByte(this[2] * (float) byte.MaxValue),
      Convert.ToByte(this[1] * (float) byte.MaxValue),
      Convert.ToByte(this[0] * (float) byte.MaxValue),
      byte.MaxValue
    }, 0);

    public override bool Equals(object o)
    {
      AmbientColor ambientColor = (AmbientColor) o;
      return (double) ambientColor[0] == (double) this[0] && (double) ambientColor[1] == (double) this[1] && (double) ambientColor[2] == (double) this[2];
    }

    public override int GetHashCode() => this.GetARGB();
  }
}
