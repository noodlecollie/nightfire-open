// Decompiled with JetBrains decompiler
// Type: JbnLib.Bsp.BrushV42
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using System;

namespace JbnLib.Bsp
{
  public class BrushV42
  {
    private int _Flags;
    private int _BrushSideIndex;
    private int _BrushSideCount;

    public static int Length => 12;

    public int Flags
    {
      get => this._Flags;
      set => this._Flags = value;
    }

    public int BrushSideIndex
    {
      get => this._BrushSideIndex;
      set => this._BrushSideIndex = value;
    }

    public int BrushSideCount
    {
      get => this._BrushSideCount;
      set => this._BrushSideCount = value;
    }

    public BrushV42(byte[] raw)
    {
      this._Flags = BitConverter.ToInt32(raw, 0);
      this._BrushSideIndex = BitConverter.ToInt32(raw, 4);
      this._BrushSideCount = BitConverter.ToInt32(raw, 8);
    }

    public BrushV42(int flags, int brushsideindex, int brushsidecount)
    {
      this._Flags = flags;
      this._BrushSideIndex = brushsideindex;
      this._BrushSideCount = brushsidecount;
    }

    public byte[] GetBytes()
    {
      byte[] bytes = new byte[BrushV42.Length];
      BitConverter.GetBytes(this._Flags).CopyTo((Array) bytes, 0);
      BitConverter.GetBytes(this._BrushSideIndex).CopyTo((Array) bytes, 4);
      BitConverter.GetBytes(this._BrushSideCount).CopyTo((Array) bytes, 8);
      return bytes;
    }
  }
}
