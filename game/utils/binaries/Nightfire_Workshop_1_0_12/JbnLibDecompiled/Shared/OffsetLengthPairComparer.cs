// Decompiled with JetBrains decompiler
// Type: JbnLib.Shared.OffsetLengthPairComparer
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using System.Collections.Generic;

namespace JbnLib.Shared
{
  public class OffsetLengthPairComparer : IComparer<OffsetLengthPair>
  {
    public int Compare(OffsetLengthPair a, OffsetLengthPair b)
    {
      if (a.Offset > b.Offset)
        return 1;
      return a.Offset < b.Offset ? -1 : 0;
    }
  }
}
