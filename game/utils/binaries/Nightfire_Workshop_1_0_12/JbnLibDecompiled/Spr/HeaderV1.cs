// Decompiled with JetBrains decompiler
// Type: JbnLib.Spr.HeaderV1
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using System;
using System.Text;

namespace JbnLib.Spr
{
  public class HeaderV1
  {
    protected string _Identifier;
    protected int _Version;
    protected SpriteType _Type;
    protected float _BoundingRadius;
    protected int _Width;
    protected int _Height;
    protected int _FrameCount;
    protected float _BeamLength;
    protected SyncType _SyncType;

    public static int Length => 36;

    public string Identifier
    {
      get => this._Identifier;
      set => this._Identifier = value;
    }

    public int Version
    {
      get => this._Version;
      set => this._Version = value;
    }

    public SpriteType Type
    {
      get => this._Type;
      set => this._Type = value;
    }

    public float BoundingRadius
    {
      get => this._BoundingRadius;
      set => this._BoundingRadius = value;
    }

    public int Width
    {
      get => this._Width;
      set => this._Width = value;
    }

    public int Height
    {
      get => this._Height;
      set => this._Height = value;
    }

    public int FrameCount
    {
      get => this._FrameCount;
      set => this._FrameCount = value;
    }

    public float BeamLength
    {
      get => this._BeamLength;
      set => this._BeamLength = value;
    }

    public SyncType SyncType
    {
      get => this._SyncType;
      set => this._SyncType = value;
    }

    public HeaderV1()
    {
    }

    public HeaderV1(byte[] raw)
    {
      this._Identifier = Encoding.ASCII.GetString(raw, 0, 4);
      this._Version = BitConverter.ToInt32(raw, 4);
      this._Type = (SpriteType) BitConverter.ToInt32(raw, 8);
      this._BoundingRadius = BitConverter.ToSingle(raw, 12);
      this._Width = BitConverter.ToInt32(raw, 16);
      this._Height = BitConverter.ToInt32(raw, 20);
      this._FrameCount = BitConverter.ToInt32(raw, 24);
      this._BeamLength = BitConverter.ToSingle(raw, 28);
      this._SyncType = (SyncType) BitConverter.ToInt32(raw, 32);
    }

    public byte[] GetBytes()
    {
      byte[] bytes = new byte[HeaderV1.Length];
      Encoding.ASCII.GetBytes(this._Identifier).CopyTo((Array) bytes, 0);
      BitConverter.GetBytes(this._Version).CopyTo((Array) bytes, 4);
      BitConverter.GetBytes((int) this._Type).CopyTo((Array) bytes, 8);
      BitConverter.GetBytes(this._BoundingRadius).CopyTo((Array) bytes, 12);
      BitConverter.GetBytes(this._Width).CopyTo((Array) bytes, 16);
      BitConverter.GetBytes(this._Height).CopyTo((Array) bytes, 20);
      BitConverter.GetBytes(this._FrameCount).CopyTo((Array) bytes, 24);
      BitConverter.GetBytes(this._BeamLength).CopyTo((Array) bytes, 28);
      BitConverter.GetBytes((int) this._SyncType).CopyTo((Array) bytes, 32);
      return bytes;
    }
  }
}
