// Decompiled with JetBrains decompiler
// Type: JbnLib.Shared.CountOffsetPair
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Interfaces;
using System;

namespace JbnLib.Shared
{
  [Serializable]
  public class CountOffsetPair : IStructure
  {
    private int _Count;
    private int _Offset;

    public static int Length => 8;

    public int Count
    {
      get => this._Count;
      set => this._Count = value;
    }

    public int Offset
    {
      get => this._Offset;
      set => this._Offset = value;
    }

    public CountOffsetPair()
    {
      this._Count = -1;
      this._Offset = -1;
    }

    public CountOffsetPair(byte[] raw)
    {
      this._Count = BitConverter.ToInt32(raw, 0);
      this._Offset = BitConverter.ToInt32(raw, 4);
    }

    public CountOffsetPair(int count, int offset)
    {
      this._Count = count;
      this._Offset = offset;
    }

    public byte[] GetBytes()
    {
      byte[] bytes = new byte[CountOffsetPair.Length];
      BitConverter.GetBytes(this._Count).CopyTo((Array) bytes, 0);
      BitConverter.GetBytes(this._Offset).CopyTo((Array) bytes, 4);
      return bytes;
    }

    public new string ToString() => this._Count.ToString() + " @ " + (object) this._Offset;
  }
}
