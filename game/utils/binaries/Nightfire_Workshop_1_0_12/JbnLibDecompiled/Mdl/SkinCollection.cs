// Decompiled with JetBrains decompiler
// Type: JbnLib.Mdl.SkinCollection
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Interfaces;
using System;

namespace JbnLib.Mdl
{
  public class SkinCollection : IStructure
  {
    private int _Length;
    private short[,] _Skins;
    private int _Families;
    private int _References;

    public int Length => this._Length;

    public short[,] Skins => this._Skins;

    public int Families => this._Families;

    public int References => this._References;

    public SkinCollection() => this._Skins = new short[0, 0];

    public SkinCollection(byte[] raw, int families, int references)
    {
      this._Families = families;
      this._References = references;
      this._Skins = new short[this._Families, this._References];
      int startIndex = 0;
      for (int index1 = 0; index1 < families; ++index1)
      {
        for (int index2 = 0; index2 < references; ++index2)
        {
          this._Skins[index1, index2] = BitConverter.ToInt16(raw, startIndex);
          startIndex += 2;
        }
      }
      this._Length = this._Families * this._References * 2;
    }

    public SkinCollection(short[,] skins, int families, int references)
    {
      this._Families = families;
      this._References = references;
      this._Skins = skins;
      this._Length = this._Families * this._References * 2;
    }

    public byte[] GetBytes()
    {
      byte[] bytes = new byte[this.Length];
      int index1 = 0;
      for (int index2 = 0; index2 < this._Families; ++index2)
      {
        for (int index3 = 0; index3 < this._References; ++index3)
        {
          BitConverter.GetBytes(this._Skins[index2, index3]).CopyTo((Array) bytes, index1);
          index1 += 2;
        }
      }
      return bytes;
    }

    public new string ToString() => this._Families.ToString() + ", " + (object) this._References;
  }
}
