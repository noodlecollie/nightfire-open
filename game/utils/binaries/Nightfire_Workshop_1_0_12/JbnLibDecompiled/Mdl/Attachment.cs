// Decompiled with JetBrains decompiler
// Type: JbnLib.Mdl.Attachment
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Interfaces;
using JbnLib.Shared;
using System;
using System.Text;

namespace JbnLib.Mdl
{
  public class Attachment : IStructure
  {
    private string _Name;
    private int _Type;
    private int _Bone;
    private Point3D _Position;
    private Point3D _Vector1;
    private Point3D _Vector2;
    private Point3D _Vector3;

    public static int Length => 88;

    public string Name
    {
      get => this._Name;
      set => this._Name = value;
    }

    public int Type
    {
      get => this._Type;
      set => this._Type = value;
    }

    public int Bone
    {
      get => this._Bone;
      set => this._Bone = value;
    }

    public Point3D Position
    {
      get => this._Position;
      set => this._Position = value;
    }

    public Point3D Vector1
    {
      get => this._Vector1;
      set => this._Vector1 = value;
    }

    public Point3D Vector2
    {
      get => this._Vector2;
      set => this._Vector2 = value;
    }

    public Point3D Vector3
    {
      get => this._Vector3;
      set => this._Vector3 = value;
    }

    public Attachment(byte[] raw)
    {
      this._Name = Encoding.ASCII.GetString(raw, 0, 32).Trim(new char[1]);
      this._Type = BitConverter.ToInt32(raw, 32);
      this._Bone = BitConverter.ToInt32(raw, 36);
      this._Position = new Point3D(BitConverter.ToSingle(raw, 40), BitConverter.ToSingle(raw, 44), BitConverter.ToSingle(raw, 48));
      this._Vector1 = new Point3D(BitConverter.ToSingle(raw, 52), BitConverter.ToSingle(raw, 56), BitConverter.ToSingle(raw, 60));
      this._Vector2 = new Point3D(BitConverter.ToSingle(raw, 64), BitConverter.ToSingle(raw, 68), BitConverter.ToSingle(raw, 72));
      this._Vector3 = new Point3D(BitConverter.ToSingle(raw, 76), BitConverter.ToSingle(raw, 80), BitConverter.ToSingle(raw, 84));
    }

    public Attachment(
      string name,
      int type,
      int bone,
      Point3D position,
      Point3D vector1,
      Point3D vector2,
      Point3D vector3)
    {
      this._Name = name;
      this._Type = type;
      this._Bone = bone;
      this._Position = position;
      this._Vector1 = vector1;
      this._Vector2 = vector2;
      this._Vector3 = vector3;
    }

    public byte[] GetBytes()
    {
      byte[] bytes = new byte[Attachment.Length];
      Encoding.ASCII.GetBytes(this._Name).CopyTo((Array) bytes, 0);
      BitConverter.GetBytes(this._Type).CopyTo((Array) bytes, 32);
      BitConverter.GetBytes(this._Bone).CopyTo((Array) bytes, 36);
      this._Position.GetBytes().CopyTo((Array) bytes, 40);
      this._Vector1.GetBytes().CopyTo((Array) bytes, 52);
      this._Vector2.GetBytes().CopyTo((Array) bytes, 64);
      this._Vector3.GetBytes().CopyTo((Array) bytes, 76);
      return bytes;
    }

    public new string ToString() => this._Name;
  }
}
