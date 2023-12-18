// Decompiled with JetBrains decompiler
// Type: JbnLib.Bsp.NodeV42
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Shared;
using System;

namespace JbnLib.Bsp
{
  public class NodeV42
  {
    private int _PlaneIndex;
    private int[] _Children = new int[2];
    private Point3D _Min;
    private Point3D _Max;

    public static int Length => 36;

    public int PlaneIndex
    {
      get => this._PlaneIndex;
      set => this._PlaneIndex = value;
    }

    public int[] Children
    {
      get => this._Children;
      set => this._Children = value;
    }

    public Point3D Min
    {
      get => this._Min;
      set => this._Min = value;
    }

    public Point3D Max
    {
      get => this._Max;
      set => this._Max = value;
    }

    public NodeV42(byte[] raw)
    {
      this._PlaneIndex = BitConverter.ToInt32(raw, 0);
      this._Children[0] = BitConverter.ToInt32(raw, 4);
      this._Children[1] = BitConverter.ToInt32(raw, 8);
      this._Min = new Point3D(BitConverter.ToSingle(raw, 12), BitConverter.ToSingle(raw, 16), BitConverter.ToSingle(raw, 20));
      this._Max = new Point3D(BitConverter.ToSingle(raw, 24), BitConverter.ToSingle(raw, 28), BitConverter.ToSingle(raw, 32));
    }

    public NodeV42(int planeindex, int child0, int child1, Point3D min, Point3D max)
    {
      this._PlaneIndex = planeindex;
      this._Children[0] = child0;
      this._Children[1] = child1;
      this._Min = min;
      this._Max = max;
    }

    public byte[] GetBytes()
    {
      byte[] bytes = new byte[NodeV42.Length];
      BitConverter.GetBytes(this._PlaneIndex).CopyTo((Array) bytes, 0);
      BitConverter.GetBytes(this._Children[0]).CopyTo((Array) bytes, 4);
      BitConverter.GetBytes(this._Children[1]).CopyTo((Array) bytes, 8);
      this._Min.GetBytes().CopyTo((Array) bytes, 12);
      this._Max.GetBytes().CopyTo((Array) bytes, 24);
      return bytes;
    }
  }
}
