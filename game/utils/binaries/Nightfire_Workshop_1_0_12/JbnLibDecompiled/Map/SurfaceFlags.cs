// Decompiled with JetBrains decompiler
// Type: JbnLib.Map.SurfaceFlags
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using System;

namespace JbnLib.Map
{
  [Flags]
  public enum SurfaceFlags
  {
    KEEP = 8,
    NODRAW = 32, // 0x00000020
    NOIMPACTS = 131072, // 0x00020000
    NODECALS = 262144, // 0x00040000
  }
}
