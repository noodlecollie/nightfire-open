// Decompiled with JetBrains decompiler
// Type: JbnLib.Fgd.Types.Color
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Lang;
using System;
using System.Globalization;

namespace JbnLib.Fgd.Types
{
  public class Color
  {
    public short Red;
    public short Green;
    public short Blue;
    public short Brightness;
    public bool HasValue;

    public Color()
    {
    }

    public Color(byte red, byte green, byte blue)
    {
      this.Red = (short) red;
      this.Green = (short) green;
      this.Blue = (short) blue;
      this.Brightness = (short) -1;
      this.HasValue = true;
    }

    public Color(byte red, byte green, byte blue, byte brightness)
    {
      this.Red = (short) red;
      this.Green = (short) green;
      this.Blue = (short) blue;
      this.Brightness = (short) brightness;
      this.HasValue = true;
    }

    public Color(string line)
    {
      string[] strArray = line.Replace("  ", " ").Split(' ');
      if (strArray.Length == 4)
      {
        try
        {
          this.Red = (short) Convert.ToByte(strArray[0]);
          this.Green = (short) Convert.ToByte(strArray[1]);
          this.Blue = (short) Convert.ToByte(strArray[2]);
          this.Brightness = (short) Convert.ToByte(strArray[3]);
          this.HasValue = true;
        }
        catch
        {
          Messages.ThrowException("Fgd.Types.Color.New(string)", Messages.CONVERT_BYTE);
        }
      }
      else if (strArray.Length == 3)
      {
        try
        {
          this.Red = (short) Convert.ToByte(strArray[0]);
          this.Green = (short) Convert.ToByte(strArray[1]);
          this.Blue = (short) Convert.ToByte(strArray[2]);
          this.Brightness = (short) -1;
          this.HasValue = true;
        }
        catch
        {
          Messages.ThrowException("Fgd.Types.Color.New(string)", Messages.CONVERT_BYTE);
        }
      }
      else if (strArray.Length == 1)
      {
        if (strArray[0].StartsWith("#"))
        {
          if (strArray[0].Length == 7)
          {
            this.Red = (short) byte.Parse(strArray[0].Substring(1, 2), NumberStyles.HexNumber);
            this.Green = (short) byte.Parse(strArray[0].Substring(3, 2), NumberStyles.HexNumber);
            this.Blue = (short) byte.Parse(strArray[0].Substring(5, 2), NumberStyles.HexNumber);
            this.Brightness = (short) -1;
            this.HasValue = true;
          }
          else
          {
            if (strArray[0].Length != 9)
              return;
            this.Red = (short) byte.Parse(strArray[0].Substring(1, 2), NumberStyles.HexNumber);
            this.Green = (short) byte.Parse(strArray[0].Substring(3, 2), NumberStyles.HexNumber);
            this.Blue = (short) byte.Parse(strArray[0].Substring(5, 2), NumberStyles.HexNumber);
            this.Brightness = (short) byte.Parse(strArray[0].Substring(7, 2), NumberStyles.HexNumber);
            this.HasValue = true;
          }
        }
        else
          Messages.ThrowException("Fgd.Types.Color.New(string)", Messages.HEX_COLORS);
      }
      else
        Messages.ThrowException("Fgd.Types.Color.New(string)", Messages.RGB_FORMAT);
    }

    public new string ToString() => this.Brightness == (short) -1 ? this.Red.ToString() + " " + (object) this.Green + " " + (object) this.Blue : this.Red.ToString() + " " + (object) this.Green + " " + (object) this.Blue + " " + (object) this.Brightness;
  }
}
