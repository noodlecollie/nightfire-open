// Decompiled with JetBrains decompiler
// Type: JbnLib.Log.Types.Say.SayPhoenix
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using System;

namespace JbnLib.Log.Types.Say
{
  public class SayPhoenix : JbnLib.Log.Types.Say.Say
  {
    public SayPhoenix(DateTime date, int id, string message)
    {
      this._Date = date;
      this._Channel = Team.Phoenix;
      this._ID = id;
      this._Message = message;
    }
  }
}
