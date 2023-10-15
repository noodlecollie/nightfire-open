// Decompiled with JetBrains decompiler
// Type: JbnLib.Qc.CommandAnimation
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using System.IO;

namespace JbnLib.Qc
{
  public class CommandAnimation
  {
    public const string Command = "$animation";
    private string _Name;
    private string _File;
    private OptionFps _Fps;
    private OptionLoop _Loop;

    public string Name
    {
      get => this._Name;
      set => this._Name = value;
    }

    public string File
    {
      get => this._File;
      set
      {
        if (new FileInfo(value).Extension.Length == 0)
          this._File = value + ".smd";
        else
          this._File = value;
      }
    }

    public OptionFps Fps
    {
      get => this._Fps;
      set => this._Fps = value;
    }

    public OptionLoop Loop
    {
      get => this._Loop;
      set => this._Loop = value;
    }

    public new string ToString()
    {
      string str = "$animation \"" + this._Name + "\" \"" + this._File + "\"";
      if (this._Fps != null)
        str = str + " " + this._Fps.ToString();
      if (this._Loop != null)
        str = str + " " + this._Loop.ToString();
      return str;
    }
  }
}
