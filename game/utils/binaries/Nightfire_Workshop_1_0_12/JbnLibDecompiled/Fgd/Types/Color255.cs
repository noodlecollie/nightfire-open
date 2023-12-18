// Decompiled with JetBrains decompiler
// Type: JbnLib.Fgd.Types.Color255
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Fgd.Interfaces;
using JbnLib.Lang;

namespace JbnLib.Fgd.Types
{
  public class Color255 : INameText
  {
    private string _Name;
    private string _Text;
    private Color _Value;

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

    public Color Value
    {
      get => this._Value;
      set => this._Value = value;
    }

    public Color255()
    {
    }

    public Color255(string name, string text, Color value)
    {
      this._Name = name;
      this._Text = text;
      this._Value = value;
    }

    public Color255(string line)
    {
      string[] strArray = line.Split(':');
      if (strArray[0].ToLower().Contains("(color255)"))
      {
        this._Name = strArray[0].ToLower().Replace("(color255)", "").Trim();
        this._Text = strArray[1].Trim().Trim('"');
        if (strArray.Length == 3)
          this._Value = new Color(strArray[2].Trim().Trim('"'));
        else
          this._Value = new Color();
      }
      else
        Messages.ThrowException("Fgd.Types.Color255.New(string)", Messages.NOT_COLOR255);
    }

    public new string ToString() => this._Name + "(color255) : \"" + this._Text + "\" : \"" + this._Value.ToString() + "\"";
  }
}
