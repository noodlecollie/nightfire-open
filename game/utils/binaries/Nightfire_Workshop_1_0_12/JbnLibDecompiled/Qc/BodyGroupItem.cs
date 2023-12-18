// Decompiled with JetBrains decompiler
// Type: JbnLib.Qc.BodyGroupItem
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

namespace JbnLib.Qc
{
  public class BodyGroupItem
  {
    private string _Name;
    private string _File;

    public string Name
    {
      get => this._Name;
      set => this._Name = value;
    }

    public string File
    {
      get => this._File;
      set => this._File = value;
    }

    public BodyGroupItem()
    {
    }

    public BodyGroupItem(string name) => this._Name = name;

    public BodyGroupItem(string name, string file)
    {
      this._Name = name;
      this._File = file;
    }

    public new string ToString()
    {
      switch (this._Name)
      {
        case "studio":
          return "studio \"" + this._File + "\"";
        case "blank":
          return "blank";
        default:
          return string.Format("\"{0}\" \"{1}\"", (object) this._Name, (object) this._File);
      }
    }
  }
}
