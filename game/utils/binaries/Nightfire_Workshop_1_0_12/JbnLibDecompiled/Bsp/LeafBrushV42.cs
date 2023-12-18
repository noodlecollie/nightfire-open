// Decompiled with JetBrains decompiler
// Type: JbnLib.Bsp.LeafBrushV42
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using System;

namespace JbnLib.Bsp
{
  public class LeafBrushV42
  {
    private int _BrushIndex;

    public static int Length => 4;

    public int BrushIndex
    {
      get => this._BrushIndex;
      set => this._BrushIndex = value;
    }

    public LeafBrushV42(byte[] raw) => this._BrushIndex = BitConverter.ToInt32(raw, 0);

    public LeafBrushV42(int brushindex) => this._BrushIndex = brushindex;

    public byte[] GetBytes() => BitConverter.GetBytes(this._BrushIndex);

    public new string ToString() => this._BrushIndex.ToString();
  }
}
