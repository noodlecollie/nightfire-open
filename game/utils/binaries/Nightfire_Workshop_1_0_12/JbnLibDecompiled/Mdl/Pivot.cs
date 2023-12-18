// Decompiled with JetBrains decompiler
// Type: JbnLib.Mdl.Pivot
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Interfaces;
using JbnLib.Shared;
using System;

namespace JbnLib.Mdl
{
  [Serializable]
  public class Pivot : IStructure
  {
    private Point3D _Position;
    private int _Start;
    private int _End;

    public static int Length => 20;

    public Point3D Position
    {
      get => this._Position;
      set => this._Position = value;
    }

    public int Start
    {
      get => this._Start;
      set => this._Start = value;
    }

    public int End
    {
      get => this._End;
      set => this._End = value;
    }

    public Pivot(byte[] raw)
    {
      this._Position = new Point3D(BitConverter.ToSingle(raw, 0), BitConverter.ToSingle(raw, 4), BitConverter.ToSingle(raw, 8));
      this._Start = BitConverter.ToInt32(raw, 12);
      this._End = BitConverter.ToInt32(raw, 16);
    }

    public Pivot(Point3D position, int start, int end)
    {
      this._Position = position;
      this._Start = start;
      this._End = end;
    }

    public byte[] GetBytes()
    {
      byte[] bytes = new byte[Pivot.Length];
      this._Position.GetBytes().CopyTo((Array) bytes, 0);
      BitConverter.GetBytes(this._Start).CopyTo((Array) bytes, 12);
      BitConverter.GetBytes(this._End).CopyTo((Array) bytes, 16);
      return bytes;
    }

    public new string ToString() => this._Position.ToString() + ", " + (object) this._Start + "-" + (object) this._End;
  }
}
