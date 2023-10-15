// Decompiled with JetBrains decompiler
// Type: JbnLib.Wrappers.StudioMdlArgs
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using System.ComponentModel;

namespace JbnLib.Wrappers
{
  public class StudioMdlArgs
  {
    private string _Texture = "";
    private bool _TagReversed;
    private bool _TagBadNormals;
    private bool _FlipAllTriangles;
    private int _NormalBlendAngle;
    private bool _DumpHBoxes;
    private bool _IgnoreWarnings;
    private int _MaxSequenceGroupSize = -1;

    [Category("StudioMdl")]
    [DefaultValue("")]
    [Description("This texture is used whenever a texture is not specified.  It should be something like 'mytexture.png'.")]
    public string Texture
    {
      get => this._Texture;
      set => this._Texture = value;
    }

    [Category("StudioMdl")]
    [DefaultValue(false)]
    [Description("Set to true to tag reversed.")]
    public bool TagReversed
    {
      get => this._TagReversed;
      set => this._TagReversed = value;
    }

    [Category("StudioMdl")]
    [DefaultValue(false)]
    [Description("Set to true to tag bad normals.")]
    public bool TagBadNormals
    {
      get => this._TagBadNormals;
      set => this._TagBadNormals = value;
    }

    [Category("StudioMdl")]
    [DefaultValue(false)]
    [Description("Set to true to turn the model inside out.")]
    public bool FlipAllTriangles
    {
      get => this._FlipAllTriangles;
      set => this._FlipAllTriangles = value;
    }

    [Category("StudioMdl")]
    [DefaultValue(typeof (int), "0")]
    [Description("Set to zero to ignore, otherwise set the blend angle.")]
    public int NormalBlendAngle
    {
      get => this._NormalBlendAngle;
      set => this._NormalBlendAngle = value;
    }

    [Category("StudioMdl")]
    [DefaultValue(false)]
    [Description("Set to true to dump hitboxes.")]
    public bool DumpHBoxes
    {
      get => this._DumpHBoxes;
      set => this._DumpHBoxes = value;
    }

    [Category("StudioMdl")]
    [DefaultValue(false)]
    [Description("Set to true to not display any warnings.")]
    public bool IgnoreWarnings
    {
      get => this._IgnoreWarnings;
      set => this._IgnoreWarnings = value;
    }

    [Category("StudioMdl")]
    [DefaultValue(typeof (int), "-1")]
    [Description("Maximum sequences per group.  If it is equal to -1, it will not be sent to the compiler.")]
    public int MaxSequenceGroupSize
    {
      get => this._MaxSequenceGroupSize;
      set => this._MaxSequenceGroupSize = value;
    }

    public new string ToString()
    {
      string str = "";
      if (this._Texture.Length != 0)
        str = str + "-t \"" + this._Texture + "\" ";
      if (this._TagReversed)
        str += "-r ";
      if (this._TagBadNormals)
        str += "-n ";
      if (this._FlipAllTriangles)
        str += "-f ";
      if (this._NormalBlendAngle != 0)
        str = str + "-a " + (object) this._NormalBlendAngle + " ";
      if (this._DumpHBoxes)
        str += "-h ";
      if (this._IgnoreWarnings)
        str += "-i ";
      if (this._MaxSequenceGroupSize != -1)
        str = str + "-g " + (object) this._MaxSequenceGroupSize + " ";
      return str;
    }
  }
}
