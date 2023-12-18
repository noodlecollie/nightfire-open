// Decompiled with JetBrains decompiler
// Type: JbnLib.Log.Types.DateData.DateData
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using System;

namespace JbnLib.Log.Types.DateData
{
  public class DateData
  {
    protected DateTime _DateTime;
    protected string _Data;

    public DateTime DateTime
    {
      get => this._DateTime;
      set => this._DateTime = value;
    }

    public string Data
    {
      get => this._Data;
      set => this._Data = value;
    }

    public DateData(DateTime datetime, string data)
    {
      this._DateTime = datetime;
      this._Data = data;
    }

    public DateData()
    {
    }
  }
}
