// Decompiled with JetBrains decompiler
// Type: JbnLib.Map.SideV220
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Shared;

namespace JbnLib.Map
{
  public class SideV220 : ISide
  {
    internal Plane3D _Plane = new Plane3D();
    internal string _Texture = "";
    internal TextureAxis _S = new TextureAxis();
    internal TextureAxis _T = new TextureAxis();
    internal float _Rotation;
    internal Point _Scale = new Point();

    public Plane3D Plane
    {
      get => this._Plane;
      set => this._Plane = value;
    }

    public string Texture
    {
      get => this._Texture;
      set => this._Texture = value;
    }

    public TextureAxis S
    {
      get => this._S;
      set => this._S = value;
    }

    public TextureAxis T
    {
      get => this._T;
      set => this._T = value;
    }

    public float Rotation
    {
      get => this._Rotation;
      set => this._Rotation = value;
    }

    public Point Scale
    {
      get => this._Scale;
      set => this._Scale = value;
    }

    public new string ToString() => string.Format("{0} {1} [ {2} ] [ {3} ] {4} {5}\n", (object) this._Plane.ToString(), (object) this._Texture, (object) this._S.ToString(), (object) this._T.ToString(), (object) this._Rotation, (object) this._Scale.ToStringNoPad());

    public SideV510 ToV510() => this.ToV510(false);

    public SideV510 ToV510(bool convertspecialtextures)
    {
      SideV510 v510 = new SideV510();
      v510.Lightmap = new Lightmap();
      v510.Material = WorldMaterial.wld_lightmap;
      v510.Plane = this._Plane;
      v510.Rotation = this._Rotation;
      v510.Scale = this._Scale;
      v510.Texture = this._Texture;
      v510.S = this._S;
      v510.T = this._T;
      return v510;
    }

    public SideV44 ToV44(int id)
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
