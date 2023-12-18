// Decompiled with JetBrains decompiler
// Type: JbnLib.Map.SideV44
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Shared;

namespace JbnLib.Map
{
  public class SideV44 : SideV220
  {
    private int _Id;
    private int _LightmapScale = 16;
    private int _SmoothingGroups;

    public int Id
    {
      get => this._Id;
      set => this._Id = value;
    }

    public int LightmapScale
    {
      get => this._LightmapScale;
      set => this._LightmapScale = value;
    }

    public int SmoothingGroups
    {
      get => this._SmoothingGroups;
      set => this._SmoothingGroups = value;
    }

    public new string ToString() => "side\n{\n     \"id\" \"" + (object) this._Id + "\"\n     \"plane\" \"" + this._Plane.ToString() + "\"\n     \"material\" \"" + this._Texture + "\"\n     \"uaxis\" \"" + this._S.ToString() + " " + (object) this._Scale.X + "\"\n     \"vaxis\" \"" + this._T.ToString() + " " + (object) this._Scale.Y + "\"\n     \"rotation\" \"" + (object) this._Rotation + "\"\n     \"lightmapscale\" \"" + (object) this._LightmapScale + "\"\n     \"smoothing_groups\" \"" + (object) this._SmoothingGroups + "\"\n     dispinfo{}\n}\n";

    public SideV220 ToV220() => (SideV220) this;

    public new SideV510 ToV510()
    {
      SideV510 v510 = new SideV510();
      v510.Lightmap = new Lightmap(this._LightmapScale, 0);
      v510.Material = WorldMaterial.wld_lightmap;
      v510.Plane = this._Plane;
      v510.Rotation = this._Rotation;
      v510.Scale = this._Scale;
      v510.Texture = this._Texture;
      v510.S = this._S;
      v510.T = this._T;
      return v510;
    }
  }
}
