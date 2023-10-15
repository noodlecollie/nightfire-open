// Decompiled with JetBrains decompiler
// Type: JbnLib.Fgd.Types.Sound
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Fgd.Interfaces;
using JbnLib.Lang;

namespace JbnLib.Fgd.Types
{
  public class Sound : INameTextValue, INameText
  {
    private string _Name;
    private string _Text;
    private string _Value;

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

    public string Value
    {
      get => this._Value;
      set => this._Value = value;
    }

    public Sound()
    {
    }

    public Sound(string name, string text, string value)
    {
      this._Name = name;
      this._Text = text;
      this._Value = value;
    }

    public Sound(string line)
    {
      string[] strArray = line.Split(':');
      if (strArray[0].ToLower().Contains("(sound)"))
      {
        this._Name = strArray[0].ToLower().Replace("(sound)", "").Trim();
        this._Text = strArray[1].Trim().Trim('"');
        if (strArray.Length == 3)
          this._Value = strArray[2].Trim().Trim('"');
        else
          this._Value = "";
      }
      else
        Messages.ThrowException("Fgd.Types.Sound.New(string)", Messages.NOT_SOUND);
    }

    public new string ToString() => this._Name + "(sound) : \"" + this._Text + "\" : \"" + this._Value + "\"";
  }
}
