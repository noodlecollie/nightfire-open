// Decompiled with JetBrains decompiler
// Type: JbnLib.Mdl.HitBox
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Interfaces;
using JbnLib.Shared;
using System;

namespace JbnLib.Mdl
{
  public class HitBox : IStructure
  {
    private int _Bone;
    private int _Group;
    private Point3D _Min;
    private Point3D _Max;

    public static int Length => 32;

    public int Bone
    {
      get => this._Bone;
      set => this._Bone = value;
    }

    public int Group
    {
      get => this._Group;
      set => this._Group = value;
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

    public HitBox(byte[] raw)
    {
      this._Bone = BitConverter.ToInt32(raw, 0);
      this._Group = BitConverter.ToInt32(raw, 4);
      this._Min = new Point3D(BitConverter.ToSingle(raw, 8), BitConverter.ToSingle(raw, 12), BitConverter.ToSingle(raw, 16));
      this._Max = new Point3D(BitConverter.ToSingle(raw, 20), BitConverter.ToSingle(raw, 24), BitConverter.ToSingle(raw, 28));
    }

    public HitBox(int bone, int group, Point3D min, Point3D max)
    {
      this._Bone = bone;
      this._Group = group;
      this._Min = min;
      this._Max = max;
    }

    public byte[] GetBytes()
    {
      byte[] bytes = new byte[HitBox.Length];
      BitConverter.GetBytes(this._Bone).CopyTo((Array) bytes, 0);
      BitConverter.GetBytes(this._Group).CopyTo((Array) bytes, 4);
      this._Min.GetBytes().CopyTo((Array) bytes, 8);
      this._Max.GetBytes().CopyTo((Array) bytes, 20);
      return bytes;
    }

    public new string ToString() => this._Bone.ToString() + ", " + (object) this._Group;
  }
}
