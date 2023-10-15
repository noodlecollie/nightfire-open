// Decompiled with JetBrains decompiler
// Type: JbnLib.Mdl.BoneFixUp
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Interfaces;
using JbnLib.Lang;
using JbnLib.Shared;
using System;

namespace JbnLib.Mdl
{
  public class BoneFixUp : IStructure
  {
    private float _XScale;
    private float _XSkewY;
    private float _XSkewZ;
    private float _XShift;
    private float _YSkewX;
    private float _YScale;
    private float _YSkewZ;
    private float _YShift;
    private float _ZSkewX;
    private float _ZSkewY;
    private float _ZScale;
    private float _ZShift;

    public static int Length => 48;

    public float XScale
    {
      get => this._XScale;
      set => this._XScale = value;
    }

    public float XSkewY
    {
      get => this._XSkewY;
      set => this._XSkewY = value;
    }

    public float XSkewZ
    {
      get => this._XSkewZ;
      set => this._XSkewZ = value;
    }

    public float XShift
    {
      get => this._XShift;
      set => this._XShift = value;
    }

    public float YSkewX
    {
      get => this._YSkewX;
      set => this._YSkewX = value;
    }

    public float YScale
    {
      get => this._YScale;
      set => this._YScale = value;
    }

    public float YSkewZ
    {
      get => this._YSkewZ;
      set => this._YSkewZ = value;
    }

    public float YShift
    {
      get => this._YShift;
      set => this._YShift = value;
    }

    public float ZSkewX
    {
      get => this._ZSkewX;
      set => this._ZSkewX = value;
    }

    public float ZSkewY
    {
      get => this._ZSkewY;
      set => this._ZSkewY = value;
    }

    public float ZScale
    {
      get => this._ZScale;
      set => this._ZScale = value;
    }

    public float ZShift
    {
      get => this._ZShift;
      set => this._ZShift = value;
    }

    public BoneFixUp(byte[] raw)
    {
      this._XScale = BitConverter.ToSingle(raw, 0);
      this._XSkewY = BitConverter.ToSingle(raw, 4);
      this._XSkewZ = BitConverter.ToSingle(raw, 8);
      this._XShift = BitConverter.ToSingle(raw, 12);
      this._YSkewX = BitConverter.ToSingle(raw, 16);
      this._YScale = BitConverter.ToSingle(raw, 20);
      this._YSkewZ = BitConverter.ToSingle(raw, 24);
      this._YShift = BitConverter.ToSingle(raw, 28);
      this._ZSkewX = BitConverter.ToSingle(raw, 32);
      this._ZSkewY = BitConverter.ToSingle(raw, 36);
      this._ZScale = BitConverter.ToSingle(raw, 40);
      this._ZShift = BitConverter.ToSingle(raw, 44);
    }

    public BoneFixUp(
      float xscale,
      float xskewy,
      float xskewz,
      float xpos,
      float yskewx,
      float yscale,
      float yskewz,
      float ypos,
      float zskewx,
      float zskewy,
      float zscale,
      float zpos)
    {
      this._XScale = xscale;
      this._XSkewY = xskewy;
      this._XSkewZ = xskewz;
      this._XShift = xpos;
      this._YSkewX = yskewx;
      this._YScale = yscale;
      this._YSkewZ = yskewz;
      this._YShift = ypos;
      this._ZSkewX = zskewx;
      this._ZSkewY = zskewy;
      this._ZScale = zscale;
      this._ZShift = zpos;
    }

    public BoneFixUp(float[,] table)
    {
      this._XScale = table[0, 0];
      this._XSkewY = table[0, 1];
      this._XSkewZ = table[0, 2];
      this._XShift = table[0, 3];
      this._YSkewX = table[1, 0];
      this._YScale = table[1, 1];
      this._YSkewZ = table[1, 2];
      this._YShift = table[1, 3];
      this._ZSkewX = table[2, 0];
      this._ZSkewY = table[2, 1];
      this._ZScale = table[2, 2];
      this._ZShift = table[2, 3];
    }

    public BoneFixUp(Matrix transform)
    {
      if (transform.Rows != 3 || transform.Cols != 4)
        Messages.ThrowException("Mdl.BoneFixUp(Matrix)", "Input matrix must be 3x4 in size.");
      this._XScale = (float) transform[0, 0];
      this._XSkewY = (float) transform[0, 1];
      this._XSkewZ = (float) transform[0, 2];
      this._XShift = (float) transform[0, 3];
      this._YSkewX = (float) transform[1, 0];
      this._YScale = (float) transform[1, 1];
      this._YSkewZ = (float) transform[1, 2];
      this._YShift = (float) transform[1, 3];
      this._ZSkewX = (float) transform[2, 0];
      this._ZSkewY = (float) transform[2, 1];
      this._ZScale = (float) transform[2, 2];
      this._ZShift = (float) transform[2, 3];
    }

    public byte[] GetBytes()
    {
      byte[] bytes = new byte[BoneFixUp.Length];
      BitConverter.GetBytes(this._XScale).CopyTo((Array) bytes, 0);
      BitConverter.GetBytes(this._XSkewY).CopyTo((Array) bytes, 4);
      BitConverter.GetBytes(this._XSkewZ).CopyTo((Array) bytes, 8);
      BitConverter.GetBytes(this._XShift).CopyTo((Array) bytes, 12);
      BitConverter.GetBytes(this._YSkewX).CopyTo((Array) bytes, 16);
      BitConverter.GetBytes(this._YScale).CopyTo((Array) bytes, 20);
      BitConverter.GetBytes(this._YSkewZ).CopyTo((Array) bytes, 24);
      BitConverter.GetBytes(this._YShift).CopyTo((Array) bytes, 28);
      BitConverter.GetBytes(this._ZSkewX).CopyTo((Array) bytes, 32);
      BitConverter.GetBytes(this._ZSkewY).CopyTo((Array) bytes, 36);
      BitConverter.GetBytes(this._ZScale).CopyTo((Array) bytes, 40);
      BitConverter.GetBytes(this._ZShift).CopyTo((Array) bytes, 44);
      return bytes;
    }

    public Matrix ToMatrix() => new Matrix(new double[3, 4]
    {
      {
        (double) this._XScale,
        (double) this._XSkewY,
        (double) this._XSkewZ,
        (double) this._XShift
      },
      {
        (double) this._YSkewX,
        (double) this._YScale,
        (double) this._YSkewZ,
        (double) this._YShift
      },
      {
        (double) this._ZSkewX,
        (double) this._ZSkewY,
        (double) this._ZScale,
        (double) this._ZShift
      }
    });

    public new string ToString() => "(" + (object) this._XShift + ", " + (object) this._YShift + ", " + (object) this._ZShift + ") (" + (object) this._XScale + ", " + (object) this._XSkewY + ", " + (object) this._XSkewZ + ") (" + (object) this._YSkewX + ", " + (object) this._YScale + ", " + (object) this._YSkewZ + ") (" + (object) this._ZSkewX + ", " + (object) this._ZSkewY + ", " + (object) this._ZScale + ")";
  }
}
