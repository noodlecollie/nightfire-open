// Decompiled with JetBrains decompiler
// Type: JbnLib.Mdl.BoneController
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Interfaces;
using System;

namespace JbnLib.Mdl
{
  public class BoneController : IStructure
  {
    private int _Bone;
    private MotionFlags _Type;
    private float _Start;
    private float _End;
    private int _Rest;
    private int _Index;

    public static int Length => 24;

    public int Bone
    {
      get => this._Bone;
      set => this._Bone = value;
    }

    public MotionFlags Type
    {
      get => this._Type;
      set => this._Type = value;
    }

    public float Start
    {
      get => this._Start;
      set => this._Start = value;
    }

    public float End
    {
      get => this._End;
      set => this._End = value;
    }

    public int Rest
    {
      get => this._Rest;
      set => this._Rest = value;
    }

    public int Index
    {
      get => this._Index;
      set => this._Index = value;
    }

    public BoneController(byte[] raw)
    {
      this._Bone = BitConverter.ToInt32(raw, 0);
      this._Type = (MotionFlags) BitConverter.ToInt32(raw, 4);
      this._Start = BitConverter.ToSingle(raw, 8);
      this._End = BitConverter.ToSingle(raw, 12);
      this._Rest = BitConverter.ToInt32(raw, 16);
      this._Index = BitConverter.ToInt32(raw, 20);
    }

    public BoneController(
      int bone,
      MotionFlags type,
      float start,
      float end,
      int rest,
      int index)
    {
      this._Bone = bone;
      this._Type = type;
      this._Start = start;
      this._End = end;
      this._Rest = rest;
      this._Index = index;
    }

    public byte[] GetBytes()
    {
      byte[] bytes = new byte[BoneController.Length];
      BitConverter.GetBytes(this._Bone).CopyTo((Array) bytes, 0);
      BitConverter.GetBytes((int) this._Type).CopyTo((Array) bytes, 4);
      BitConverter.GetBytes(this._Start).CopyTo((Array) bytes, 8);
      BitConverter.GetBytes(this._End).CopyTo((Array) bytes, 12);
      BitConverter.GetBytes(this._Rest).CopyTo((Array) bytes, 16);
      BitConverter.GetBytes(this._Index).CopyTo((Array) bytes, 20);
      return bytes;
    }

    public new string ToString() => this._Bone.ToString();
  }
}
