// Decompiled with JetBrains decompiler
// Type: JbnLib.Fgd.Types.Flag
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Fgd.Interfaces;
using JbnLib.Lang;
using System;

namespace JbnLib.Fgd.Types
{
  public class Flag : IFlag
  {
    private string _Text;
    private long _Value;
    private bool _Checked;

    public string Text
    {
      get => this._Text;
      set => this._Text = value;
    }

    public long Value
    {
      get => this._Value;
      set => this._Value = value;
    }

    public bool Checked
    {
      get => this._Checked;
      set => this._Checked = value;
    }

    public Flag()
    {
    }

    public Flag(string text, long value, bool @checked)
    {
      this._Text = text;
      this._Value = value;
      this._Checked = @checked;
    }

    public Flag(string line)
    {
      string[] strArray = line.Split(':');
      if (strArray.Length == 3)
      {
        this._Value = Convert.ToInt64(strArray[0].Trim());
        this._Text = strArray[1].Trim().Trim('"');
        if (strArray[2].Trim() == "1")
          this._Checked = true;
        else
          this._Checked = false;
      }
      else
        Messages.ThrowException("Fgd.Types.Flag.New(string)", Messages.THREE_SECTIONS);
    }

    public new string ToString()
    {
      string str = "0";
      if (this._Checked)
        str = "1";
      return this._Value.ToString() + " : \"" + this._Text + "\" : " + str;
    }
  }
}
