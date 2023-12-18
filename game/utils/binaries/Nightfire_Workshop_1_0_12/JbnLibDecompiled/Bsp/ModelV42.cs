// Decompiled with JetBrains decompiler
// Type: JbnLib.Bsp.ModelV42
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Shared;
using System;

namespace JbnLib.Bsp
{
  public class ModelV42
  {
    private Point3D _Min;
    private Point3D _Max;
    private float[] _Unknown = new float[4];
    private int _LeafIndex;
    private int _LeafCount;
    private int _SurfaceIndex;
    private int _SurfaceCount;

    public static int Length => 56;

    public Point3D Min
    {
      get => this._Min;
      set => this._Min = value;
    }

    public Point3D Max
    {
      get => this._Max;
      set => this._Max = value;
    }

    public float[] Unknown
    {
      get => this._Unknown;
      set => this._Unknown = value;
    }

    public int LeafIndex
    {
      get => this._LeafIndex;
      set => this._LeafIndex = value;
    }

    public int LeafCount
    {
      get => this._LeafCount;
      set => this._LeafCount = value;
    }

    public int SurfaceIndex
    {
      get => this._SurfaceIndex;
      set => this._SurfaceIndex = value;
    }

    public int SurfaceCount
    {
      get => this._SurfaceCount;
      set => this._SurfaceCount = value;
    }

    public ModelV42(byte[] raw)
    {
      this._Min = new Point3D(BitConverter.ToSingle(raw, 0), BitConverter.ToSingle(raw, 4), BitConverter.ToSingle(raw, 8));
      this._Max = new Point3D(BitConverter.ToSingle(raw, 12), BitConverter.ToSingle(raw, 16), BitConverter.ToSingle(raw, 20));
      for (byte index = 0; index < (byte) 4; ++index)
        this._Unknown[(int) index] = BitConverter.ToSingle(raw, (int) index * 4 + 24);
      this._LeafIndex = BitConverter.ToInt32(raw, 40);
      this._LeafCount = BitConverter.ToInt32(raw, 44);
      this._SurfaceIndex = BitConverter.ToInt32(raw, 48);
      this._SurfaceCount = BitConverter.ToInt32(raw, 52);
    }

    public ModelV42(
      Point3D min,
      Point3D max,
      float[] unknown,
      int leafindex,
      int leafcount,
      int surfaceindex,
      int surfacecount)
    {
      this._Min = min;
      this._Max = max;
      for (byte index = 0; index < (byte) 4; ++index)
        this._Unknown[(int) index] = unknown[(int) index];
      this._LeafIndex = leafindex;
      this._LeafCount = leafcount;
      this._SurfaceIndex = surfaceindex;
      this._SurfaceCount = surfacecount;
    }

    public byte[] GetBytes()
    {
      byte[] bytes = new byte[ModelV42.Length];
      this._Min.GetBytes().CopyTo((Array) bytes, 0);
      this._Max.GetBytes().CopyTo((Array) bytes, 12);
      for (int index = 0; index < 4; ++index)
        BitConverter.GetBytes(this._Unknown[index]).CopyTo((Array) bytes, index * 4 + 24);
      BitConverter.GetBytes(this._LeafIndex).CopyTo((Array) bytes, 40);
      BitConverter.GetBytes(this._LeafCount).CopyTo((Array) bytes, 44);
      BitConverter.GetBytes(this._SurfaceIndex).CopyTo((Array) bytes, 48);
      BitConverter.GetBytes(this._SurfaceCount).CopyTo((Array) bytes, 52);
      return bytes;
    }
  }
}
