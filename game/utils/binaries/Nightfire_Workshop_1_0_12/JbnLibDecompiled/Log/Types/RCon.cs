// Decompiled with JetBrains decompiler
// Type: JbnLib.Log.Types.RCon
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using System;

namespace JbnLib.Log.Types
{
  public class RCon
  {
    private DateTime _Date;
    private string _Command;
    private string _IPPort;

    public DateTime Date => this._Date;

    public string Command => this._Command;

    public string IPPort => this._IPPort;

    public RCon(DateTime date, string command, string ipport)
    {
      this._Date = date;
      this._Command = command;
      this._IPPort = ipport;
    }
  }
}
