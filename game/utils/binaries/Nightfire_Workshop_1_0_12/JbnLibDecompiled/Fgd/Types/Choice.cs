// Decompiled with JetBrains decompiler
// Type: JbnLib.Fgd.Types.Choice
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Fgd.Interfaces;
using JbnLib.Lang;
using JbnLib.Shared;

namespace JbnLib.Fgd.Types
{
  public class Choice : IChoice
  {
    private string _Text;
    private string _Value;

    public string Text
    {
      get => this._Text;
      set => this._Text = value;
    }

    public string Value
    {
      get => this._Value;
      set => this._Value = value;
    }

    public Choice()
    {
    }

    public Choice(string text, string value)
    {
      this._Text = text;
      this._Value = value;
    }

    public Choice(string line)
    {
      if (line.Trim().Length <= 0)
        return;
      string[] strArray = line.Split(':');
      if (strArray.Length == 2)
      {
        this._Value = strArray[0].Trim().Trim('"');
        this._Text = strArray[1].Trim().Trim('"');
      }
      else
        Messages.ThrowException("Fgd.Types.Choice.New(string)", Messages.TWO_SECTIONS);
    }

    public new string ToString() => StaticMethods.MagicQuotes(this._Value) + " : \"" + this._Text + "\"";
  }
}
