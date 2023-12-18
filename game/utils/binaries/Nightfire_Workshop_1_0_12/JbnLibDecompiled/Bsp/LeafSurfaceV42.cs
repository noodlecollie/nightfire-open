// Decompiled with JetBrains decompiler
// Type: JbnLib.Bsp.LeafSurfaceV42
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using System;

namespace JbnLib.Bsp
{
  public class LeafSurfaceV42
  {
    private int _SurfaceIndex;

    public static int Length => 4;

    public int SurfaceIndex
    {
      get => this._SurfaceIndex;
      set => this._SurfaceIndex = value;
    }

    public LeafSurfaceV42(byte[] raw) => this._SurfaceIndex = BitConverter.ToInt32(raw, 0);

    public LeafSurfaceV42(int surfaceindex) => this._SurfaceIndex = surfaceindex;

    public byte[] GetBytes() => BitConverter.GetBytes(this._SurfaceIndex);

    public new string ToString() => this._SurfaceIndex.ToString();
  }
}
