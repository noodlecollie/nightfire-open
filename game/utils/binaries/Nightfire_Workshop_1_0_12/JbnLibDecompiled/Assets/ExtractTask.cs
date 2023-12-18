// Decompiled with JetBrains decompiler
// Type: JbnLib.Assets.ExtractTask
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

namespace JbnLib.Assets
{
  public enum ExtractTask : sbyte
  {
    NotSpecified = -1, // 0xFF
    VerifyingID = 0,
    ReadingTOC = 1,
    CreatingDirs = 2,
    ExtractingFiles = 3,
    Done = 4,
  }
}
