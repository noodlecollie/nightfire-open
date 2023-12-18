// Decompiled with JetBrains decompiler
// Type: JbnLib.Smd.VertexV10
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Mdl;
using JbnLib.Shared;

namespace JbnLib.Smd
{
  public class VertexV10
  {
    private sbyte _Bone;
    private Point3D _Position;
    private Point3D _Normal;
    private TextureCoordinate _TextureCoordinate;

    public sbyte Bone
    {
      get => this._Bone;
      set => this._Bone = value;
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

    public VertexV10()
    {
      this._Bone = (sbyte) 0;
      this._Position = new Point3D();
      this._Normal = new Point3D();
      this._TextureCoordinate = new TextureCoordinate();
    }

    public VertexV10(sbyte bone, Point3D pos, Point3D norm, TextureCoordinate texcoord)
    {
      this._Bone = bone;
      this._Position = pos;
      this._Normal = norm;
      this._TextureCoordinate = texcoord;
    }

    public new string ToString() => this._Bone.ToString().PadLeft(3) + " " + this._Position.ToString() + " " + this._Normal.ToString() + " " + this._TextureCoordinate.ToString();
  }
}
