// Decompiled with JetBrains decompiler
// Type: JbnLib.Fgd.Types.ClassSprite
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Shared;

namespace JbnLib.Fgd.Types
{
  public class ClassSprite
  {
    private string _Sprite;
    private string _Extra;

    public string Sprite
    {
      get => this._Sprite;
      set => this._Sprite = value;
    }

    public string Extra
    {
      get => this._Extra;
      set => this._Extra = value;
    }

    public ClassSprite()
    {
    }

    public ClassSprite(string sprite, string extra)
    {
      this._Sprite = sprite;
      this._Extra = extra;
    }

    public ClassSprite(string data)
    {
      this._Sprite = StaticMethods.PullQuoted(data);
      if (data.Contains("\""))
        this._Extra = data.Substring(data.LastIndexOf('"') + 1).Trim();
      else
        this._Extra = "";
    }

    public new string ToString() => this._Extra.Length > 0 ? "\"" + this._Sprite + "\" " + this._Extra : "\"" + this._Sprite + "\"";
  }
}
