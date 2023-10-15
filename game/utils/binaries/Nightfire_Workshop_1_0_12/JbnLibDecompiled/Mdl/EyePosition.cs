// Decompiled with JetBrains decompiler
// Type: JbnLib.Mdl.EyePosition
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Lang;
using System;

namespace JbnLib.Mdl
{
  public class EyePosition
  {
    private float _Y;
    private float _X;
    private float _Z;

    public static int Length => 12;

    public float Y
    {
      get => this._Y;
      set => this._Y = value;
    }

    public float X
    {
      get => this._X;
      set => this._X = value;
    }

    public float Z
    {
      get => this._Z;
      set => this._Z = value;
    }

    public EyePosition()
    {
      this._Y = 0.0f;
      this._X = -0.0f;
      this._Z = 0.0f;
    }

    public EyePosition(byte[] raw)
    {
      this._Y = BitConverter.ToSingle(raw, 0);
      this._X = BitConverter.ToSingle(raw, 4);
      this._Z = BitConverter.ToSingle(raw, 8);
    }

    public EyePosition(float y, float x, float z)
    {
      this._Y = y;
      this._X = x;
      this._Z = z;
    }

    public EyePosition(float[] yxz)
    {
      if (yxz.Length != 3)
        Messages.ThrowException("Mdl.Eyeposition.New(float[])", Messages.EYEPOSITION_YXZ);
      this._Y = yxz[0];
      this._X = yxz[1];
      this._Z = yxz[2];
    }

    public byte[] GetBytes()
    {
      byte[] bytes = new byte[EyePosition.Length];
      BitConverter.GetBytes(this._Y).CopyTo((Array) bytes, 0);
      BitConverter.GetBytes(this._X).CopyTo((Array) bytes, 4);
      BitConverter.GetBytes(this._Z).CopyTo((Array) bytes, 8);
      return bytes;
    }

    public new string ToString() => string.Format("{0:f6} {1:f6} {2:f6}", (object) this._Y, (object) this._X, (object) this._Z);
  }
}
