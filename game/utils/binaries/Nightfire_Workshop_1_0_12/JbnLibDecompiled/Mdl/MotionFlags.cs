// Decompiled with JetBrains decompiler
// Type: JbnLib.Mdl.MotionFlags
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using System;

namespace JbnLib.Mdl
{
  [Flags]
  public enum MotionFlags
  {
    Invalid = -1, // 0xFFFFFFFF
    None = 0,
    X = 1,
    Y = 2,
    Z = 4,
    XR = 8,
    YR = 16, // 0x00000010
    ZR = 32, // 0x00000020
    LX = 64, // 0x00000040
    LY = 128, // 0x00000080
    LZ = 256, // 0x00000100
    AX = 512, // 0x00000200
    AY = 1024, // 0x00000400
    AZ = 2048, // 0x00000800
    AXR = 4096, // 0x00001000
    AYR = 8192, // 0x00002000
    AZR = 16384, // 0x00004000
  }
}
