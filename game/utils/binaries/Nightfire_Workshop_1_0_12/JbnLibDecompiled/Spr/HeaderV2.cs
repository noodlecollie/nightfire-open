// Decompiled with JetBrains decompiler
// Type: JbnLib.Spr.HeaderV2
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using System;
using System.Text;

namespace JbnLib.Spr
{
  public class HeaderV2 : HeaderV1
  {
    private SpriteTexture _Texture;

    public new static int Length => 40;

    public SpriteTexture Texture
    {
      get => this._Texture;
      set => this._Texture = value;
    }

    public HeaderV2()
    {
    }

    public HeaderV2(byte[] raw)
    {
      this._Identifier = Encoding.ASCII.GetString(raw, 0, 4);
      this._Version = BitConverter.ToInt32(raw, 4);
      this._Type = (SpriteType) BitConverter.ToInt32(raw, 8);
      this._Texture = (SpriteTexture) BitConverter.ToInt32(raw, 12);
      this._BoundingRadius = BitConverter.ToSingle(raw, 16);
      this._Width = BitConverter.ToInt32(raw, 20);
      this._Height = BitConverter.ToInt32(raw, 24);
      this._FrameCount = BitConverter.ToInt32(raw, 28);
      this._BeamLength = BitConverter.ToSingle(raw, 32);
      this._SyncType = (SyncType) BitConverter.ToInt32(raw, 36);
    }

    public new byte[] GetBytes()
    {
      byte[] bytes = new byte[HeaderV2.Length];
      Encoding.ASCII.GetBytes(this._Identifier).CopyTo((Array) bytes, 0);
      BitConverter.GetBytes(this._Version).CopyTo((Array) bytes, 4);
      BitConverter.GetBytes((int) this._Type).CopyTo((Array) bytes, 8);
      BitConverter.GetBytes((int) this._Texture).CopyTo((Array) bytes, 12);
      BitConverter.GetBytes(this._BoundingRadius).CopyTo((Array) bytes, 16);
      BitConverter.GetBytes(this._Width).CopyTo((Array) bytes, 20);
      BitConverter.GetBytes(this._Height).CopyTo((Array) bytes, 24);
      BitConverter.GetBytes(this._FrameCount).CopyTo((Array) bytes, 28);
      BitConverter.GetBytes(this._BeamLength).CopyTo((Array) bytes, 32);
      BitConverter.GetBytes((int) this._SyncType).CopyTo((Array) bytes, 36);
      return bytes;
    }
  }
}
