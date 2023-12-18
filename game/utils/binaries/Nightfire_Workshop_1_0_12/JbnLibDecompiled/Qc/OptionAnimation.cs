// Decompiled with JetBrains decompiler
// Type: JbnLib.Qc.OptionAnimation
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

namespace JbnLib.Qc
{
  public class OptionAnimation
  {
    public const string Option = "animation";
    private string _File;

    public string File
    {
      get => this._File;
      set => this._File = value;
    }

    public OptionAnimation()
    {
    }

    public OptionAnimation(string file) => this._File = file;

    public new string ToString() => string.Format("{0} \"{1}\"", (object) "animation", (object) this._File);
  }
}
