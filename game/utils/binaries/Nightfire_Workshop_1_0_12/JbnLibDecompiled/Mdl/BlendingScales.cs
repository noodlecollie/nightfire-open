// Decompiled with JetBrains decompiler
// Type: JbnLib.Mdl.BlendingScales
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Interfaces;
using System;

namespace JbnLib.Mdl
{
  public class BlendingScales : IStructure
  {
    private float[] _Value = new float[4];

    public static int Length => 16;

    public float[] Value
    {
      get => this._Value;
      set => this._Value = value;
    }

    public BlendingScales(byte[] raw)
    {
      for (byte index = 0; (int) index < this._Value.Length; ++index)
        this._Value[(int) index] = BitConverter.ToSingle(raw, (int) index * 4);
    }

    public BlendingScales(float[] value) => this._Value = value;

    public BlendingScales()
    {
    }

    public byte[] GetBytes()
    {
      byte[] bytes = new byte[BlendingScales.Length];
      for (byte index = 0; (int) index < this._Value.Length; ++index)
        BitConverter.GetBytes(this._Value[(int) index]).CopyTo((Array) bytes, (int) index * 4);
      return bytes;
    }

    public new string ToString() => this._Value[0].ToString() + ", " + (object) this._Value[1] + ", " + (object) this._Value[2] + ", " + (object) this._Value[3];
  }
}
