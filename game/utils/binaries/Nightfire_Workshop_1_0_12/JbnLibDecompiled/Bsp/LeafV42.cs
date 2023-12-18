// Decompiled with JetBrains decompiler
// Type: JbnLib.Bsp.LeafV42
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Shared;
using System;

namespace JbnLib.Bsp
{
  public class LeafV42
  {
    private int _Type;
    private int _VisibilityOffset;
    private Point3D _Min;
    private Point3D _Max;
    private int _LeafSurfaceIndex;
    private int _LeafSurfaceCount;
    private int _LeafBrushIndex;
    private int _LeafBrushCount;

    public static int Length => 48;

    public int Type
    {
      get => this._Type;
      set => this._Type = value;
    }

    public int VisibilityOffset
    {
      get => this._VisibilityOffset;
      set => this._VisibilityOffset = value;
    }

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

    public int LeafSurfaceIndex
    {
      get => this._LeafSurfaceIndex;
      set => this._LeafSurfaceIndex = value;
    }

    public int LeafSurfaceCount
    {
      get => this._LeafSurfaceCount;
      set => this._LeafSurfaceCount = value;
    }

    public int LeafBrushIndex
    {
      get => this._LeafBrushIndex;
      set => this._LeafBrushIndex = value;
    }

    public int LeafBrushCount
    {
      get => this._LeafBrushCount;
      set => this._LeafBrushCount = value;
    }

    public LeafV42(byte[] raw)
    {
      this._Type = BitConverter.ToInt32(raw, 0);
      this._VisibilityOffset = BitConverter.ToInt32(raw, 4);
      this._Min = new Point3D(BitConverter.ToSingle(raw, 8), BitConverter.ToSingle(raw, 12), BitConverter.ToSingle(raw, 16));
      this._Max = new Point3D(BitConverter.ToSingle(raw, 20), BitConverter.ToSingle(raw, 24), BitConverter.ToSingle(raw, 28));
      this._LeafSurfaceIndex = BitConverter.ToInt32(raw, 32);
      this._LeafSurfaceCount = BitConverter.ToInt32(raw, 36);
      this._LeafBrushIndex = BitConverter.ToInt32(raw, 40);
      this._LeafBrushCount = BitConverter.ToInt32(raw, 44);
    }

    public LeafV42(
      int type,
      int visibilityoffset,
      Point3D min,
      Point3D max,
      int leafsurfaceindex,
      int leafsurfacecount,
      int leafbrushindex,
      int leafbrushcount)
    {
      this._Type = type;
      this._VisibilityOffset = visibilityoffset;
      this._Min = min;
      this._Max = max;
      this._LeafSurfaceIndex = leafsurfaceindex;
      this._LeafSurfaceCount = leafsurfacecount;
      this._LeafBrushIndex = leafbrushindex;
      this._LeafBrushCount = leafbrushcount;
    }

    public byte[] GetBytes()
    {
      byte[] bytes = new byte[LeafV42.Length];
      BitConverter.GetBytes(this._Type).CopyTo((Array) bytes, 0);
      BitConverter.GetBytes(this._VisibilityOffset).CopyTo((Array) bytes, 4);
      this._Min.GetBytes().CopyTo((Array) bytes, 8);
      this._Max.GetBytes().CopyTo((Array) bytes, 20);
      BitConverter.GetBytes(this._LeafSurfaceIndex).CopyTo((Array) bytes, 32);
      BitConverter.GetBytes(this._LeafSurfaceCount).CopyTo((Array) bytes, 36);
      BitConverter.GetBytes(this._LeafBrushIndex).CopyTo((Array) bytes, 40);
      BitConverter.GetBytes(this._LeafBrushCount).CopyTo((Array) bytes, 44);
      return bytes;
    }
  }
}
