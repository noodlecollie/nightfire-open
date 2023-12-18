// Decompiled with JetBrains decompiler
// Type: JbnLib.Bsp.BrushSideV42
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using System;

namespace JbnLib.Bsp
{
  public class BrushSideV42
  {
    private int _PlaneIndex;
    private int _SurfaceIndex;

    public static int Length => 8;

    public int PlaneIndex
    {
      get => this._PlaneIndex;
      set => this._PlaneIndex = value;
    }

    public int SurfaceIndex
    {
      get => this._SurfaceIndex;
      set => this._SurfaceIndex = value;
    }

    public BrushSideV42(byte[] raw)
    {
      this._PlaneIndex = BitConverter.ToInt32(raw, 0);
      this._SurfaceIndex = BitConverter.ToInt32(raw, 4);
    }

    public BrushSideV42(int planeindex, int surfaceindex)
    {
      this._PlaneIndex = planeindex;
      this._SurfaceIndex = surfaceindex;
    }

    public byte[] GetBytes()
    {
      byte[] bytes = new byte[BrushSideV42.Length];
      BitConverter.GetBytes(this._PlaneIndex).CopyTo((Array) bytes, 0);
      BitConverter.GetBytes(this._SurfaceIndex).CopyTo((Array) bytes, 4);
      return bytes;
    }
  }
}
