// Decompiled with JetBrains decompiler
// Type: JbnLib.Mdl.LevelOfDetailFlags
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using System;

namespace JbnLib.Mdl
{
  [Flags]
  public enum LevelOfDetailFlags
  {
    None = 0,
    TimesOne = 1,
    TimesTwo = 2,
    TimesFive = 4,
    PlusOne = 256, // 0x00000100
    PlusTwo = 512, // 0x00000200
    PlusFour = 1024, // 0x00000400
  }
}
