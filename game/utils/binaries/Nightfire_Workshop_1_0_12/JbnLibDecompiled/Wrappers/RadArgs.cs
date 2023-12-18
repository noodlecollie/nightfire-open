// Decompiled with JetBrains decompiler
// Type: JbnLib.Wrappers.RadArgs
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Shared;
using System.ComponentModel;
using System.Drawing.Design;
using System.Windows.Forms.Design;

namespace JbnLib.Wrappers
{
  public class RadArgs
  {
    private bool _Extra;
    private bool _Circus;
    private bool _NoTexScale;
    private bool _NoHunt;
    private bool _NoSlide;
    private bool _NoLerp;
    private bool _NoSkyFix;
    private bool _Dump;
    private bool _Incremental;
    private bool _Sparse;
    private bool _NoMatrix;
    private bool _NoSubdivide;
    private ushort _Bounce = 1;
    private float _MaxLight = 256f;
    private float _Smooth = 50f;
    private float _Chop = 64f;
    private float _TexChop = 32f;
    private float _Coring = 1f;
    private float _DLight = 25f;
    private float _Sky = 1f;
    private float _Fade = 1f;
    private float _DScale = 2f;
    private float _BScale = 1f;
    private float _Scale = 1f;
    private float _Gamma = 2.2f;
    private AmbientColor _Ambient = new AmbientColor();
    private RadArgs.FallOffEnum _FallOff = RadArgs.FallOffEnum.InverseSquare;
    private string _Lights = "";

    [Category("BRad")]
    [DefaultValue(false)]
    [Description("Improve lighting quality by doing 9 point oversampling.")]
    public bool Extra
    {
      get => this._Extra;
      set => this._Extra = value;
    }

    [Category("BRad")]
    [DefaultValue(false)]
    [Description("Enable 'circus' mode for locating unlit lightmaps.")]
    public bool Circus
    {
      get => this._Circus;
      set => this._Circus = value;
    }

    [Category("BRad")]
    [DefaultValue(false)]
    [Description("Do not scale radiosity patches with texture scale.")]
    public bool NoTexScale
    {
      get => this._NoTexScale;
      set => this._NoTexScale = value;
    }

    [Category("BRad")]
    [DefaultValue(false)]
    [Description("Skip lightmap texel hunt for world.")]
    public bool NoHunt
    {
      get => this._NoHunt;
      set => this._NoHunt = value;
    }

    [Category("BRad")]
    [DefaultValue(false)]
    [Description("Skip sliding lightmap texels into its polygon bounds.")]
    public bool NoSlide
    {
      get => this._NoSlide;
      set => this._NoSlide = value;
    }

    [Category("BRad")]
    [DefaultValue(false)]
    [Description("Disable radiosity interpolation, nearest point instead.")]
    public bool NoLerp
    {
      get => this._NoLerp;
      set => this._NoLerp = value;
    }

    [Category("BRad")]
    [DefaultValue(false)]
    [Description("Disable light_environment being global.")]
    public bool NoSkyFix
    {
      get => this._NoSkyFix;
      set => this._NoSkyFix = value;
    }

    [Category("BRad")]
    [DefaultValue(false)]
    [Description("Dumps light patches to a file for hlrad debugging info.")]
    public bool Dump
    {
      get => this._Dump;
      set => this._Dump = value;
    }

    [Category("BRad")]
    [DefaultValue(false)]
    [Description("Use or create an incremental transfer list file.")]
    public bool Incremental
    {
      get => this._Incremental;
      set => this._Incremental = value;
    }

    [Category("BRad")]
    [DefaultValue(false)]
    [Description("Enable low memory vismatrix algorithm.")]
    public bool Sparse
    {
      get => this._Sparse;
      set => this._Sparse = value;
    }

    [Category("BRad")]
    [DefaultValue(false)]
    [Description("Disable usage of vismatrix entirely.")]
    public bool NoMatrix
    {
      get => this._NoMatrix;
      set => this._NoMatrix = value;
    }

    [Category("BRad")]
    [DefaultValue(false)]
    [Description("Disables patch subdividing.")]
    public bool NoSubdivide
    {
      get => this._NoSubdivide;
      set => this._NoSubdivide = value;
    }

    [Category("BRad")]
    [DefaultValue(typeof (ushort), "1")]
    [Description("Set number of radiosity bounces.  This number should be between 1 and 1000.")]
    public ushort Bounce
    {
      get => this._Bounce;
      set => this._Bounce = value;
    }

    [Category("BRad")]
    [DefaultValue(typeof (float), "256.000")]
    [Description("Set maximum light intensity value.  Must be positive.")]
    public float MaxLight
    {
      get => this._MaxLight;
      set => this._MaxLight = value;
    }

    [Category("BRad")]
    [DefaultValue(typeof (float), "50.000")]
    [Description("Set smoothing threshold for blending in degrees.")]
    public float Smooth
    {
      get => this._Smooth;
      set => this._Smooth = value;
    }

    [Category("BRad")]
    [DefaultValue(typeof (float), "64.000")]
    [Description("Set radiosity patch size for normal textures.  A higher value decreases performance while a lower value increases quality.  Chop values below 32 are not recommended.")]
    public float Chop
    {
      get => this._Chop;
      set => this._Chop = value;
    }

    [Category("BRad")]
    [DefaultValue(typeof (float), "32.000")]
    [Description("Set radiosity patch size for texture light faces.  This should be half the chop value.  TexChop values below 16 are not recommended.")]
    public float TexChop
    {
      get => this._TexChop;
      set => this._TexChop = value;
    }

    [Category("BRad")]
    [DefaultValue(typeof (float), "1.000")]
    [Description("Set lighting threshold before blackness.")]
    public float Coring
    {
      get => this._Coring;
      set => this._Coring = value;
    }

    [Category("BRad")]
    [DefaultValue(typeof (float), "25.000")]
    [Description("Similar to -maxlight but re-normalizes instead.  Sets the direct lighting threshold.")]
    public float DLight
    {
      get => this._DLight;
      set => this._DLight = value;
    }

    [Category("BRad")]
    [DefaultValue(typeof (float), "1.000")]
    [Description("Set ambient sunlight contribution in the shade outside.  Effects how much sky lighting is put into shadows.")]
    public float Sky
    {
      get => this._Sky;
      set => this._Sky = value;
    }

    [Category("BRad")]
    [DefaultValue(typeof (float), "1.000")]
    [Description("Set global fade.  Larger values means shorter lights.")]
    public float Fade
    {
      get => this._Fade;
      set => this._Fade = value;
    }

    [Category("BRad")]
    [DefaultValue(typeof (float), "2.000")]
    [Description("Scales the direct lighting layer.")]
    public float DScale
    {
      get => this._DScale;
      set => this._DScale = value;
    }

    [Category("BRad")]
    [DefaultValue(typeof (float), "1.000")]
    [Description("Scales bounce value to fit lighting grid.")]
    public float BScale
    {
      get => this._BScale;
      set => this._BScale = value;
    }

    [Category("BRad")]
    [DefaultValue(typeof (float), "1.000")]
    [Description("Set global light scaling value.")]
    public float Scale
    {
      get => this._Scale;
      set => this._Scale = value;
    }

    [Category("BRad")]
    [DefaultValue(typeof (float), "2.200")]
    [Description("Set global gamma value.")]
    public float Gamma
    {
      get => this._Gamma;
      set => this._Gamma = value;
    }

    [Editor(typeof (AmbientColorEditor), typeof (UITypeEditor))]
    [Category("BRad")]
    [DefaultValue(typeof (AmbientColor), "0 0 0")]
    [Description("Sets the color of ambient lighting.  It is given in RGB format with each channel having a value of 0 to 1.")]
    public AmbientColor Ambient
    {
      get => this._Ambient;
      set => this._Ambient = value;
    }

    [Category("BRad")]
    [DefaultValue(RadArgs.FallOffEnum.InverseSquare)]
    [Description("Set global falloff mode.")]
    public RadArgs.FallOffEnum FallOff
    {
      get => this._FallOff;
      set => this._FallOff = value;
    }

    [Editor(typeof (FileNameEditor), typeof (UITypeEditor))]
    [Category("BRad")]
    [DefaultValue("")]
    [Description("Manually specify a lights .rad file to use.")]
    public string Lights
    {
      get => this._Lights;
      set => this._Lights = value;
    }

    public new string ToString()
    {
      string str = "";
      if (this._Extra)
        str += "-extra ";
      if (this._Circus)
        str += "-circus ";
      if (this._NoTexScale)
        str += "-notexscale ";
      if (this._NoHunt)
        str += "-nohunt ";
      if (this._NoSlide)
        str += "-noslide ";
      if (this._NoLerp)
        str += "-nolerp ";
      if (this._NoSkyFix)
        str += "-noskyfix ";
      if (this._Dump)
        str += "-dump ";
      if (this._Incremental)
        str += "-incremental ";
      if (this._Sparse)
        str += "-sparse ";
      if (this._NoMatrix)
        str += "-nomatrix ";
      if (this._Bounce != (ushort) 1)
        str = str + "-bounce " + (object) this._Bounce + " ";
      if ((double) this._MaxLight != 256.0)
        str = str + "-maxlight " + (object) this._MaxLight + " ";
      if ((double) this._Smooth != 50.0)
        str = str + "-smooth " + (object) this._Smooth + " ";
      if ((double) this._Chop != 64.0)
        str = str + "-chop " + (object) this._Chop + " ";
      if ((double) this._TexChop != 32.0)
        str = str + "-texchop " + (object) this._TexChop + " ";
      if ((double) this._Coring != 1.0)
        str = str + "-coring " + (object) this._Coring + " ";
      if ((double) this._DLight != 25.0)
        str = str + "-dlight " + (object) this._DLight + " ";
      if ((double) this._Sky != 1.0)
        str = str + "-sky " + (object) this._Sky + " ";
      if ((double) this._Fade != 1.0)
        str = str + "-fade " + (object) this._Fade + " ";
      if ((double) this._DScale != 2.0)
        str = str + "-dscale " + (object) this._DScale + " ";
      if ((double) this._BScale != 1.0)
        str = str + "-bscale " + (object) this._BScale + " ";
      if ((double) this._Scale != 1.0)
        str = str + "-scale " + (object) this._Scale + " ";
      if ((double) this._Gamma != 2.2000000476837158)
        str = str + "-gamma " + (object) this._Gamma + " ";
      if (!this._Ambient.IsBlack)
        str = str + "-ambient " + this._Ambient.ToString() + " ";
      if (this._FallOff == RadArgs.FallOffEnum.InverseLinear)
        str = str + "-falloff " + (object) (int) this._FallOff + " ";
      if (this._Lights.Length > 0)
        str = str + "-lights \"" + this._Lights + "\" ";
      return str;
    }

    public enum FallOffEnum : byte
    {
      InverseLinear = 1,
      InverseSquare = 2,
    }
  }
}
