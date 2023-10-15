// Decompiled with JetBrains decompiler
// Type: JbnLib.Log.Types.DateID.EnteredGame
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using System;

namespace JbnLib.Log.Types.DateID
{
  public class EnteredGame : JbnLib.Log.Types.DateID.DateID
  {
    public EnteredGame(DateTime date, int id)
    {
      this._Date = date;
      this._ID = id;
    }
  }
}
