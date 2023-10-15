// Decompiled with JetBrains decompiler
// Type: JbnLib.Smd.VertexV11
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Mdl;
using JbnLib.Shared;

namespace JbnLib.Smd
{
  public class VertexV11
  {
    private BlendV11[] _Blends;
    private Point3D _Position;
    private Point3D _Normal;
    private TextureCoordinate _TextureCoordinate;

    public BlendV11[] Blends
    {
      get => this._Blends;
      set => this._Blends = value;
    }

    public Point3D Position
    {
      get => this._Position;
      set => this._Position = value;
    }

    public Point3D Normal
    {
      get => this._Normal;
      set => this._Normal = value;
    }

    public TextureCoordinate TextureCoordinate
    {
      get => this._TextureCoordinate;
      set => this._TextureCoordinate = value;
    }

    public VertexV11()
    {
      this._Blends = new BlendV11[4]
      {
        new BlendV11(),
        new BlendV11(),
        new BlendV11(),
        new BlendV11()
      };
      this._Position = new Point3D();
      this._Normal = new Point3D();
      this._TextureCoordinate = new TextureCoordinate();
    }

    public VertexV11(BlendV11[] blends, Point3D pos, Point3D norm, TextureCoordinate texcoord)
    {
      this._Blends = blends;
      this._Position = pos;
      this._Normal = norm;
      this._TextureCoordinate = texcoord;
    }

    public new string ToString()
    {
      string str = "$blended " + this._Blends.Length.ToString().PadLeft(3);
      for (byte index = 0; (int) index < this._Blends.Length; ++index)
        str = str + " " + this._Blends[(int) index].ToString();
      return str + " " + this._Position.ToString() + " " + this._Normal.ToString() + " " + this._TextureCoordinate.ToString();
    }
  }
}
