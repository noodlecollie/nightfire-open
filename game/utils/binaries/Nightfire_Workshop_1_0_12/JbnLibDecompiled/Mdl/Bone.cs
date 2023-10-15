// Decompiled with JetBrains decompiler
// Type: JbnLib.Mdl.Bone
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Interfaces;
using JbnLib.Shared;
using System;
using System.Text;

namespace JbnLib.Mdl
{
  [Serializable]
  public class Bone : IStructure
  {
    private string _Name;
    private int _Parent;
    private int _Flags;
    private int[] _Controllers = new int[6];
    private Point3D _Position;
    private Point3D _Rotation;
    private Point3D _ScalePosition;
    private Point3D _ScaleRotation;

    public static int Length => 112;

    public string Name
    {
      get => this._Name;
      set => this._Name = value;
    }

    public int Parent
    {
      get => this._Parent;
      set => this._Parent = value;
    }

    public int Flags
    {
      get => this._Flags;
      set => this._Flags = value;
    }

    public int[] Controllers
    {
      get => this._Controllers;
      set => this._Controllers = value;
    }

    public Point3D Position
    {
      get => this._Position;
      set => this._Position = value;
    }

    public Point3D Rotation
    {
      get => this._Rotation;
      set => this._Rotation = value;
    }

    public Point3D ScalePosition
    {
      get => this._ScalePosition;
      set => this._ScalePosition = value;
    }

    public Point3D ScaleRotation
    {
      get => this._ScaleRotation;
      set => this._ScaleRotation = value;
    }

    public Bone(byte[] raw)
    {
      this._Name = Encoding.ASCII.GetString(raw, 0, 32).Trim(new char[1]);
      this._Parent = BitConverter.ToInt32(raw, 32);
      this._Flags = BitConverter.ToInt32(raw, 36);
      for (int index = 0; index < this._Controllers.Length; ++index)
        this._Controllers[index] = BitConverter.ToInt32(raw, index * 4 + 40);
      this._Position = new Point3D(BitConverter.ToSingle(raw, 64), BitConverter.ToSingle(raw, 68), BitConverter.ToSingle(raw, 72));
      this._Rotation = new Point3D(BitConverter.ToSingle(raw, 76), BitConverter.ToSingle(raw, 80), BitConverter.ToSingle(raw, 84));
      this._ScalePosition = new Point3D(BitConverter.ToSingle(raw, 88), BitConverter.ToSingle(raw, 92), BitConverter.ToSingle(raw, 96));
      this._ScaleRotation = new Point3D(BitConverter.ToSingle(raw, 100), BitConverter.ToSingle(raw, 104), BitConverter.ToSingle(raw, 108));
    }

    public Bone(
      string name,
      int parent,
      int flags,
      int[] controllers,
      Point3D position,
      Point3D rotation,
      Point3D scaleposition,
      Point3D scalerotation)
    {
      this._Name = name;
      this._Parent = parent;
      this._Flags = flags;
      this._Controllers = controllers;
      this._Position = position;
      this._Rotation = rotation;
      this._ScalePosition = scaleposition;
      this._ScaleRotation = scalerotation;
    }

    public byte[] GetBytes()
    {
      byte[] bytes = new byte[Bone.Length];
      Encoding.ASCII.GetBytes(this._Name).CopyTo((Array) bytes, 0);
      BitConverter.GetBytes(this._Parent).CopyTo((Array) bytes, 32);
      BitConverter.GetBytes(this._Flags).CopyTo((Array) bytes, 36);
      for (int index = 0; index < this._Controllers.Length; ++index)
        BitConverter.GetBytes(this._Controllers[index]).CopyTo((Array) bytes, index * 4 + 40);
      this._Position.GetBytes().CopyTo((Array) bytes, 64);
      this._Rotation.GetBytes().CopyTo((Array) bytes, 76);
      this._ScalePosition.GetBytes().CopyTo((Array) bytes, 88);
      this._ScaleRotation.GetBytes().CopyTo((Array) bytes, 100);
      return bytes;
    }

    public new string ToString() => this._Name;
  }
}
