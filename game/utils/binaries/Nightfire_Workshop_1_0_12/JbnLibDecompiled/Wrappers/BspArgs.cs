// Decompiled with JetBrains decompiler
// Type: JbnLib.Wrappers.BspArgs
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using System.ComponentModel;

namespace JbnLib.Wrappers
{
  public class BspArgs
  {
    private bool _OnlyEnts;
    private bool _LeakOnly;
    private bool _ShowBevels;
    private bool _NoLighting;
    private bool _NoFill;
    private bool _NoTJunc;
    private bool _NoClip;
    private bool _NoWater;
    private ushort _MaxNodeSize = 1024;
    private ushort _BLScale = 16;
    private ushort _ILScale = 16;

    [Category("BBsp")]
    [DefaultValue(false)]
    [Description("Do an entity update from *.map to *.bsp.")]
    public bool OnlyEnts
    {
      get => this._OnlyEnts;
      set => this._OnlyEnts = value;
    }

    [Category("BBsp")]
    [DefaultValue(false)]
    [Description("Run BSP only enough to check for leaks.")]
    public bool LeakOnly
    {
      get => this._LeakOnly;
      set => this._LeakOnly = value;
    }

    [Category("BBsp")]
    [DefaultValue(false)]
    [Description("Export a .map with the expand brush generated bevels.")]
    public bool ShowBevels
    {
      get => this._ShowBevels;
      set => this._ShowBevels = value;
    }

    [Category("BBsp")]
    [DefaultValue(false)]
    [Description("Do not build lighting bsp.")]
    public bool NoLighting
    {
      get => this._NoLighting;
      set => this._NoLighting = value;
    }

    [Category("BBsp")]
    [DefaultValue(false)]
    [Description("Don't fill outside.  This will prevent generation of leak errors.")]
    public bool NoFill
    {
      get => this._NoFill;
      set => this._NoFill = value;
    }

    [Category("BBsp")]
    [DefaultValue(false)]
    [Description("Don't break edges on t-junctions.")]
    public bool NoTJunc
    {
      get => this._NoTJunc;
      set => this._NoTJunc = value;
    }

    [Category("BBsp")]
    [DefaultValue(false)]
    [Description("Don't process the clipping hull.")]
    public bool NoClip
    {
      get => this._NoClip;
      set => this._NoClip = value;
    }

    [Category("BBsp")]
    [DefaultValue(false)]
    [Description("Do not create water models. It does not export the .smd files generated from func_waters to compile with studiomdl.exe")]
    public bool NoWater
    {
      get => this._NoWater;
      set => this._NoWater = value;
    }

    [Category("BBsp")]
    [DefaultValue(typeof (ushort), "1024")]
    [Description("Sets the maximum portal node size.  The minimum value is 64 and the maximum is 4096.")]
    public ushort MaxNodeSize
    {
      get => this._MaxNodeSize;
      set => this._MaxNodeSize = value;
    }

    [Category("BBsp")]
    [DefaultValue(typeof (ushort), "16")]
    [Description("Sets the base lightmap scale.  The minimum value is 16.")]
    public ushort BLScale
    {
      get => this._BLScale;
      set => this._BLScale = value;
    }

    [Category("BBsp")]
    [DefaultValue(typeof (ushort), "16")]
    [Description("Sets the incremental lightmap scale.  The minimum value is 16.")]
    public ushort ILScale
    {
      get => this._ILScale;
      set => this._ILScale = value;
    }

    public new string ToString()
    {
      string str = "";
      if (this._OnlyEnts)
        str += "-onlyents ";
      if (this._LeakOnly)
        str += "-leakonly ";
      if (this._ShowBevels)
        str += "-showbevels ";
      if (this._NoLighting)
        str += "-nolighting ";
      if (this._NoFill)
        str += "-nofill ";
      if (this._NoTJunc)
        str += "-notjunc ";
      if (this._NoClip)
        str += "-noclip ";
      if (this._NoWater)
        str += "-nowater ";
      if (this._MaxNodeSize != (ushort) 1024)
        str = str + "-maxnodesize " + (object) this._MaxNodeSize + " ";
      if (this._BLScale != (ushort) 16)
        str = str + "-blscale " + (object) this._BLScale + " ";
      if (this._ILScale != (ushort) 16)
        str = str + "-ilscale " + (object) this._ILScale + " ";
      return str;
    }
  }
}
