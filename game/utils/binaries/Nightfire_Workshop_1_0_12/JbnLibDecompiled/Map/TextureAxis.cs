// Decompiled with JetBrains decompiler
// Type: JbnLib.Map.TextureAxis
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Shared;
using System;
using System.Collections.Generic;

namespace JbnLib.Map
{
  public class TextureAxis : List<float>
  {
    public const int Length = 16;

    public float X
    {
      get => this[0];
      set => this[0] = value;
    }

    public float Y
    {
      get => this[1];
      set => this[1] = value;
    }

    public float Z
    {
      get => this[2];
      set => this[2] = value;
    }

    public float Shift
    {
      get => this[3];
      set => this[3] = value;
    }

    public TextureAxis()
    {
      this.Add(0.0f);
      this.Add(0.0f);
      this.Add(0.0f);
      this.Add(0.0f);
    }

    public TextureAxis(byte[] raw)
    {
      this.Add(BitConverter.ToSingle(raw, 0));
      this.Add(BitConverter.ToSingle(raw, 4));
      this.Add(BitConverter.ToSingle(raw, 8));
      this.Add(BitConverter.ToSingle(raw, 12));
    }

    public TextureAxis(float x, float y, float z, float shift)
    {
      this.Add(x);
      this.Add(y);
      this.Add(z);
      this.Add(shift);
    }

    public byte[] GetBytes()
    {
      byte[] bytes = new byte[16];
      BitConverter.GetBytes(this[0]).CopyTo((Array) bytes, 0);
      BitConverter.GetBytes(this[1]).CopyTo((Array) bytes, 4);
      BitConverter.GetBytes(this[2]).CopyTo((Array) bytes, 8);
      BitConverter.GetBytes(this[3]).CopyTo((Array) bytes, 12);
      return bytes;
    }

    public new string ToString() => string.Format("{0:f6} {1:f6} {2:f6} {3:f6}", (object) this[0], (object) this[1], (object) this[2], (object) this[3]);

    public Point3D GetPoint3D() => new Point3D(this[0], this[1], this[2]);
  }
}
