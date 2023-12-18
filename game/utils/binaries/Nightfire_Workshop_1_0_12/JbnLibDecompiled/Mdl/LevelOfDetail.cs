// Decompiled with JetBrains decompiler
// Type: JbnLib.Mdl.LevelOfDetail
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Interfaces;
using System;

namespace JbnLib.Mdl
{
  public class LevelOfDetail : IStructure
  {
    private int _Levels;
    private int[] _Distance = new int[4];

    public static int Length => 20;

    public int Levels
    {
      get => this._Levels;
      set => this._Levels = value;
    }

    public int[] Distance
    {
      get => this._Distance;
      set => this._Distance = value;
    }

    public LevelOfDetail() => this._Levels = 0;

    public LevelOfDetail(byte[] raw)
    {
      this._Levels = BitConverter.ToInt32(raw, 0);
      for (int index = 0; index < this._Distance.Length; ++index)
        this._Distance[index] = BitConverter.ToInt32(raw, index * 4 + 4);
    }

    public LevelOfDetail(int levels, int[] distance)
    {
      this._Levels = levels;
      this._Distance = distance;
    }

    public byte[] GetBytes()
    {
      byte[] bytes = new byte[LevelOfDetail.Length];
      BitConverter.GetBytes(this._Levels).CopyTo((Array) bytes, 0);
      for (int index = 0; index < this._Distance.Length; ++index)
        BitConverter.GetBytes(this._Distance[index]).CopyTo((Array) bytes, index * 4 + 4);
      return bytes;
    }

    public new string ToString()
    {
      if (this._Levels <= 0)
        return "";
      string str = this._Distance[0].ToString();
      for (byte index = 1; (int) index < this._Levels; ++index)
        str = str + " " + (object) this._Distance[(int) index];
      return str;
    }
  }
}
