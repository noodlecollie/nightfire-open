// Decompiled with JetBrains decompiler
// Type: JbnLib.Log.Types.VariableChanged
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using System;

namespace JbnLib.Log.Types
{
  public class VariableChanged
  {
    private DateTime _Date;
    private string _Name;
    private string _OldValue;
    private string _NewValue;

    public DateTime Date => this._Date;

    public string Name => this._Name;

    public string OldValue => this._OldValue;

    public string NewValue => this._NewValue;

    public VariableChanged(DateTime date, string name, string oldvalue, string newvalue)
    {
      this._Date = date;
      this._Name = name;
      this._OldValue = oldvalue;
      this._NewValue = newvalue;
    }
  }
}
