// Decompiled with JetBrains decompiler
// Type: JbnLib.Mdl.Vertex
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Interfaces;
using JbnLib.Shared;
using System;

namespace JbnLib.Mdl
{
  public class Vertex : IStructure
  {
    private Point3D _Position;
    private float _Scale;

    public static int Length => 16;

    public Point3D Position
    {
      get => this._Position;
      set => this._Position = value;
    }

    public float Scale
    {
      get => this._Scale;
      set => this._Scale = value;
    }

    public Vertex(byte[] raw)
    {
      this._Position = new Point3D(BitConverter.ToSingle(raw, 0), BitConverter.ToSingle(raw, 4), BitConverter.ToSingle(raw, 8));
      this._Scale = BitConverter.ToSingle(raw, 12);
    }

    public Vertex(Point3D position, float scale)
    {
      this._Position = position;
      this._Scale = scale;
    }

    public byte[] GetBytes()
    {
      byte[] bytes = new byte[Vertex.Length];
      this._Position.GetBytes().CopyTo((Array) bytes, 0);
      BitConverter.GetBytes(this._Scale).CopyTo((Array) bytes, 12);
      return bytes;
    }

    public new string ToString() => this._Position.ToString() + ", " + (object) this._Scale;
  }
}
