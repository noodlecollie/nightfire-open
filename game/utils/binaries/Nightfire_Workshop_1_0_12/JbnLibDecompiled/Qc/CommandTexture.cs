// Decompiled with JetBrains decompiler
// Type: JbnLib.Qc.CommandTexture
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

namespace JbnLib.Qc
{
  public class CommandTexture
  {
    public const string Command = "$texture";
    private string _Texture;
    private string _Material;

    public string Texture
    {
      get => this._Texture;
      set => this._Texture = value;
    }

    public string Material
    {
      get => this._Material;
      set => this._Material = value;
    }

    public CommandTexture()
    {
    }

    public CommandTexture(string texture, string material)
    {
      this._Texture = texture;
      this._Material = material;
    }

    public new string ToString() => string.Format("{0} \"{1}\" \"{2}\"", (object) "$texture", (object) this._Texture, (object) this._Material);
  }
}
