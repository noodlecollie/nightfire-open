// Decompiled with JetBrains decompiler
// Type: JbnLib.Fgd.Types.Integer
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Fgd.Interfaces;
using JbnLib.Lang;
using JbnLib.Shared;
using System;

namespace JbnLib.Fgd.Types
{
  public class Integer : IInteger, INameText
  {
    private string _Name;
    private string _Text;
    private int _Value;

    public string Name
    {
      get => this._Name;
      set => this._Name = value;
    }

    public string Text
    {
      get => this._Text;
      set => this._Text = value;
    }

    public int Value
    {
      get => this._Value;
      set => this._Value = value;
    }

    public Integer()
    {
    }

    public Integer(string name, string text, int value)
    {
      this._Name = name;
      this._Text = text;
      this._Value = value;
    }

    public Integer(string line)
    {
      string[] strArray = StaticMethods.DeleteLineComment(line).Split(':');
      if (strArray[0].ToLower().Contains("(integer)"))
      {
        this._Name = strArray[0].ToLower().Replace("(integer)", "").Trim();
        this._Text = strArray[1].Trim().Trim('"');
        if (strArray.Length == 3)
          this._Value = Convert.ToInt32(strArray[2].Trim());
        else
          this._Value = 0;
      }
      else
        Messages.ThrowException("Fgd.Types.Integer.New(string)", Messages.NOT_INT);
    }

    public new string ToString() => this._Name + "(integer) : \"" + this._Text + "\" : " + (object) this._Value;
  }
}
