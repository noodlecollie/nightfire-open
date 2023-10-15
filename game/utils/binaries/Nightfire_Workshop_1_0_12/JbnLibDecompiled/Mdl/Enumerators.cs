// Decompiled with JetBrains decompiler
// Type: JbnLib.Mdl.Enumerators
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Lang;
using JbnLib.Qc;
using System;

namespace JbnLib.Mdl
{
  public static class Enumerators
  {
    public static MotionFlags ToMotionFlags(string value)
    {
      switch (value.ToLower())
      {
        case "":
          return MotionFlags.None;
        case "ax":
          return MotionFlags.AX;
        case "axr":
          return MotionFlags.AXR;
        case "ay":
          return MotionFlags.AY;
        case "ayr":
          return MotionFlags.AYR;
        case "az":
          return MotionFlags.AZ;
        case "azr":
          return MotionFlags.AZR;
        case "lx":
          return MotionFlags.LX;
        case "ly":
          return MotionFlags.LY;
        case "lz":
          return MotionFlags.LZ;
        case "x":
          return MotionFlags.X;
        case "xr":
          return MotionFlags.XR;
        case "y":
          return MotionFlags.Y;
        case "yr":
          return MotionFlags.YR;
        case "z":
          return MotionFlags.Z;
        case "zr":
          return MotionFlags.ZR;
        default:
          return MotionFlags.Invalid;
      }
    }

    public static int LodFlagsToInt(LevelOfDetailFlags value)
    {
      byte num1 = 0;
      if ((value & LevelOfDetailFlags.TimesOne) == LevelOfDetailFlags.TimesOne)
        num1 = (byte) 1;
      if ((value & LevelOfDetailFlags.TimesTwo) == LevelOfDetailFlags.TimesTwo)
        num1 = (byte) 2;
      int num2 = 0;
      if ((value & LevelOfDetailFlags.PlusOne) == LevelOfDetailFlags.PlusOne)
        num2 += 1 * (int) num1;
      if ((value & LevelOfDetailFlags.PlusTwo) == LevelOfDetailFlags.PlusTwo)
        num2 += 2 * (int) num1;
      if ((value & LevelOfDetailFlags.PlusFour) == LevelOfDetailFlags.PlusFour)
        num2 += 4 * (int) num1;
      if ((value & LevelOfDetailFlags.TimesFive) == LevelOfDetailFlags.TimesFive)
        num2 *= 5;
      return num2;
    }

    public static object ConvertActivity(Type from, Type to, object activity)
    {
      try
      {
        switch (to.Name)
        {
          case "ActivityV10":
            ActivityV10 activityV10 = (ActivityV10) activity;
            return Enum.Parse(from, activityV10.ToString());
          case "ActivityV11":
            ActivityV11 activityV11 = (ActivityV11) activity;
            return Enum.Parse(from, activityV11.ToString());
          case "ActivityV14":
            ActivityV14 activityV14 = (ActivityV14) activity;
            return Enum.Parse(from, activityV14.ToString());
          case "ActivityV44":
            ActivityV44 activityV44 = (ActivityV44) activity;
            return Enum.Parse(from, activityV44.ToString());
          default:
            Messages.ThrowException("Mdl.Enumerators.ConvertActivity(Type, Type, object)", Messages.UPDATE_TO_SUPPORT + to.Name);
            return (object) ActivityV10.ACT_INVALID;
        }
      }
      catch
      {
        switch (to.Name)
        {
          case "ActivityV10":
            return (object) ActivityV10.ACT_INVALID;
          case "ActivityV11":
            return (object) ActivityV11.ACT_INVALID;
          case "ActivityV14":
            return (object) ActivityV14.ACT_INVALID;
          case "ActivityV44":
            return (object) ActivityV44.ACT_INVALID;
          default:
            Messages.ThrowException("Mdl.Enumerators.ConvertActivity(Type, Type, object)", Messages.UPDATE_TO_SUPPORT + to.Name);
            return (object) ActivityV10.ACT_INVALID;
        }
      }
    }
  }
}
