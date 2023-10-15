// Decompiled with JetBrains decompiler
// Type: JbnLib.Mdl.TextureCoordinate
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Interfaces;
using System;

namespace JbnLib.Mdl
{
  public class TextureCoordinate : IStructure
  {
    private float _U;
    private float _V;

    public static int Length => 8;

    public float U
    {
      get => this._U;
      set => this._U = value;
    }

    public float V
    {
      get => this._V;
      set => this._V = value;
    }

    public TextureCoordinate()
    {
    }

    public TextureCoordinate(byte[] raw)
    {
      this._U = BitConverter.ToSingle(raw, 0);
      this._V = BitConverter.ToSingle(raw, 4);
    }

    public TextureCoordinate(float u, float v)
    {
      this._U = u;
      this._V = v;
    }

    public TextureCoordinate(float ux, float uy, float uz, float vx, float vy, float vz)
    {
      this._U = Convert.ToSingle((double) ux / Math.Sqrt((double) ux * (double) ux + (double) uy * (double) uy + (double) uz * (double) uz));
      this._V = Convert.ToSingle((double) vx / Math.Sqrt((double) vx * (double) vx + (double) vy * (double) vy + (double) vz * (double) vz));
    }

    public byte[] GetBytes()
    {
      byte[] bytes = new byte[TextureCoordinate.Length];
      BitConverter.GetBytes(this._U).CopyTo((Array) bytes, 0);
      BitConverter.GetBytes(this._V).CopyTo((Array) bytes, 4);
      return bytes;
    }

    public new string ToString() => Math.Round((double) this._U, 6).ToString() + " " + (object) Math.Round((double) this._V, 6);
  }
}
