// Decompiled with JetBrains decompiler
// Type: JbnLib.Mdl.TypeFlag
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using System;

namespace JbnLib.Mdl
{
  [Flags]
  public enum TypeFlag
  {
    Vehicle = 512, // 0x00000200
    Water = 2048, // 0x00000800
    MultiplayerCharacter = 8192, // 0x00002000
    NonPlayableCharacter = 16384, // 0x00004000
  }
}
