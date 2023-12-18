// Decompiled with JetBrains decompiler
// Type: JbnLib.Map.SideV510
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Shared;

namespace JbnLib.Map
{
  public class SideV510 : SideV220
  {
    private WorldMaterial _Material = WorldMaterial.wld_lightmap;
    private Lightmap _Lightmap = new Lightmap();
    private SurfaceFlags _SurfaceFlags;

    public WorldMaterial Material
    {
      get => this._Material;
      set => this._Material = value;
    }

    public Lightmap Lightmap
    {
      get => this._Lightmap;
      set => this._Lightmap = value;
    }

    public SurfaceFlags SurfaceFlags
    {
      get => this._SurfaceFlags;
      set => this._SurfaceFlags = value;
    }

    public new string ToString() => string.Format("{0} {1} [ {2} ] [ {3} ] {4} {5} {6} {7} [ {8} ]\n", (object) this._Plane.ToString(), (object) this._Texture, (object) this._S.ToString(), (object) this._T.ToString(), (object) this._Rotation, (object) this._Scale.ToStringNoPad(), (object) (int) this._SurfaceFlags, (object) this._Material.ToString(), (object) this._Lightmap.ToString());

    public SideV220 ToV220()
    {
      SideV220 v220 = (SideV220) this;
      v220.Texture = v220.Texture.Substring(v220.Texture.IndexOf('/') + 1);
      return v220;
    }

    public new SideV44 ToV44(int id)
    {
      SideV44 v44 = new SideV44();
      v44.Id = id;
      v44.LightmapScale = 16;
      v44.Plane = this._Plane;
      v44.Rotation = this._Rotation;
      v44.Scale = this._Scale;
      v44.SmoothingGroups = 0;
      v44.Texture = this._Texture;
      v44.S = this._S;
      v44.T = this._T;
      return v44;
    }
  }
}
