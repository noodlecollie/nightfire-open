// Decompiled with JetBrains decompiler
// Type: JbnLib.Log.Types.Variable
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using System;
using System.ComponentModel;

namespace JbnLib.Log.Types
{
  [TypeConverter(typeof (VariableConverter))]
  [Description("Contains a name/value pair.")]
  public class Variable
  {
    private string _Name;
    private string _Value;

    [Description("The name of the player.")]
    public string Name
    {
      get => this._Name;
      set => this._Name = value;
    }

    [Description("The value the player achieved.")]
    public string Value
    {
      get => this._Value;
      set => this._Value = value;
    }

    public Variable(string name, string value)
    {
      this._Name = name;
      this._Value = value;
    }

    public Variable(string name, int value)
    {
      this._Name = name;
      this._Value = Convert.ToString(value);
    }

    public Variable(string name, double value)
    {
      this._Name = name;
      this._Value = Convert.ToString(value);
    }
  }
}
