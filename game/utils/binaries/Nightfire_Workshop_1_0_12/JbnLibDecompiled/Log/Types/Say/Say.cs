// Decompiled with JetBrains decompiler
// Type: JbnLib.Log.Types.Say.Say
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using System;

namespace JbnLib.Log.Types.Say
{
  public abstract class Say
  {
    protected DateTime _Date;
    protected Team _Channel;
    protected int _ID;
    protected string _Message;

    public DateTime Date => this._Date;

    public Team Channel => this._Channel;

    public int ID => this._ID;

    public string Message => this._Message;
  }
}
