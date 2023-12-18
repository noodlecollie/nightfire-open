// Decompiled with JetBrains decompiler
// Type: JbnLib.Bsp.PlaneV42
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Shared;
using System;

namespace JbnLib.Bsp
{
  public class PlaneV42
  {
    private Point3D _Normal;
    private float _Distance;
    private int _Type;

    public static int Length => 20;

    public Point3D Normal
    {
      get => this._Normal;
      set => this._Normal = value;
    }

    public float Distance
    {
      get => this._Distance;
      set => this._Distance = value;
    }

    public int Type
    {
      get => this._Type;
      set => this._Type = value;
    }

    public PlaneV42(byte[] raw)
    {
      this._Normal = new Point3D(BitConverter.ToSingle(raw, 0), BitConverter.ToSingle(raw, 4), BitConverter.ToSingle(raw, 8));
      this._Distance = BitConverter.ToSingle(raw, 12);
      this._Type = BitConverter.ToInt32(raw, 16);
    }

    public PlaneV42(Point3D normal, float distance, int type)
    {
      this._Normal = normal;
      this._Distance = distance;
      this._Type = type;
    }

    public byte[] GetBytes()
    {
      byte[] bytes = new byte[PlaneV42.Length];
      this._Normal.GetBytes().CopyTo((Array) bytes, 0);
      BitConverter.GetBytes(this._Distance).CopyTo((Array) bytes, 12);
      BitConverter.GetBytes(this._Type).CopyTo((Array) bytes, 16);
      return bytes;
    }
  }
}
