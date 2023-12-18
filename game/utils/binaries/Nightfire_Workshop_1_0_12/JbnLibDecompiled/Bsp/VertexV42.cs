// Decompiled with JetBrains decompiler
// Type: JbnLib.Bsp.VertexV42
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Shared;
using System;

namespace JbnLib.Bsp
{
  public class VertexV42
  {
    private Point3D _Position;

    public static int Length => 12;

    public Point3D Position
    {
      get => this._Position;
      set => this._Position = value;
    }

    public VertexV42(byte[] raw) => this._Position = new Point3D(BitConverter.ToSingle(raw, 0), BitConverter.ToSingle(raw, 4), BitConverter.ToSingle(raw, 8));

    public VertexV42(Point3D position) => this._Position = position;

    public byte[] GetBytes() => this._Position.GetBytes();

    public new string ToString() => this._Position.ToString();
  }
}
