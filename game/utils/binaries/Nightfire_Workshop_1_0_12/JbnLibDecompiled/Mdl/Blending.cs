// Decompiled with JetBrains decompiler
// Type: JbnLib.Mdl.Blending
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Interfaces;

namespace JbnLib.Mdl
{
  public class Blending : IStructure
  {
    private sbyte[] _Value = new sbyte[4];

    public static int Length => 4;

    public sbyte[] Value
    {
      get => this._Value;
      set => this._Value = value;
    }

    public Blending(byte[] raw)
    {
      this._Value[0] = (sbyte) raw[0];
      this._Value[1] = (sbyte) raw[1];
      this._Value[2] = (sbyte) raw[2];
      this._Value[3] = (sbyte) raw[3];
    }

    public Blending(sbyte[] value) => this._Value = value;

    public Blending()
    {
    }

    public byte[] GetBytes()
    {
      byte[] bytes = new byte[Blending.Length];
      bytes[0] = (byte) this._Value[0];
      bytes[1] = (byte) this._Value[1];
      bytes[2] = (byte) this._Value[2];
      bytes[3] = (byte) this._Value[3];
      return bytes;
    }

    public new string ToString() => this._Value[0].ToString() + ", " + (object) this._Value[1] + ", " + (object) this._Value[2] + ", " + (object) this._Value[3];
  }
}
