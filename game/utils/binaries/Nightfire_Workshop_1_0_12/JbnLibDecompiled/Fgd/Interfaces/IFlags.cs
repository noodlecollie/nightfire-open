// Decompiled with JetBrains decompiler
// Type: JbnLib.Fgd.Interfaces.IFlags
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Fgd.Types;
using System.Collections.Generic;

namespace JbnLib.Fgd.Interfaces
{
  public interface IFlags
  {
    string Name { get; set; }

    List<Flag> FlagCollection { get; set; }

    string ToString();
  }
}
