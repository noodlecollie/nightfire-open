// Decompiled with JetBrains decompiler
// Type: JbnLib.Mdl.LodPosition
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Interfaces;
using JbnLib.Shared;
using System;

namespace JbnLib.Mdl
{
  public class LodPosition : IStructure
  {
    private int _Bone;
    private float _Scale;
    private Point3D _Position;

    public static int Length => 20;

    public int Bone
    {
      get => this._Bone;
      set => this._Bone = value;
    }

    public float Scale
    {
      get => this._Scale;
      set => this._Scale = value;
    }

    public Point3D Position
    {
      get => this._Position;
      set => this._Position = value;
    }

    public LodPosition(byte[] raw)
    {
      this._Bone = BitConverter.ToInt32(raw, 0);
      this._Scale = BitConverter.ToSingle(raw, 4);
      this._Position = new Point3D(BitConverter.ToSingle(raw, 8), BitConverter.ToSingle(raw, 12), BitConverter.ToSingle(raw, 16));
    }

    public LodPosition(sbyte bone, float scale, float x, float y, float z)
    {
      this._Bone = (int) bone;
      this._Scale = scale;
      this._Position = new Point3D(x, y, z);
    }

    public byte[] GetBytes()
    {
      byte[] bytes = new byte[LodPosition.Length];
      BitConverter.GetBytes(this._Bone).CopyTo((Array) bytes, 0);
      BitConverter.GetBytes(this._Scale).CopyTo((Array) bytes, 4);
      BitConverter.GetBytes(this._Position.X).CopyTo((Array) bytes, 8);
      BitConverter.GetBytes(this._Position.Y).CopyTo((Array) bytes, 12);
      BitConverter.GetBytes(this._Position.Z).CopyTo((Array) bytes, 16);
      return bytes;
    }

    public new string ToString() => this._Bone.ToString() + ", " + (object) this._Scale + ", " + this.Position.ToString();
  }
}
