// Decompiled with JetBrains decompiler
// Type: JbnLib.Mdl.HeaderV11
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Shared;
using System;
using System.Text;

namespace JbnLib.Mdl
{
  public class HeaderV11 : Header
  {
    private int _ModelHeaderOffset;

    public new static int Length => 252;

    public int ModelHeaderOffset
    {
      get => this._ModelHeaderOffset;
      set => this._ModelHeaderOffset = value;
    }

    public HeaderV11()
    {
    }

    public HeaderV11(byte[] raw)
    {
      this._Identifier = Encoding.ASCII.GetString(raw, 0, 4);
      this._Version = BitConverter.ToInt32(raw, 4);
      this._Name = Encoding.ASCII.GetString(raw, 8, 64).Trim(new char[1]);
      this._Length = BitConverter.ToInt32(raw, 72);
      this._EyePosition = new EyePosition(BitConverter.ToSingle(raw, 76), BitConverter.ToSingle(raw, 80), BitConverter.ToSingle(raw, 84));
      this._BoundingBoxMin = new Point3D(BitConverter.ToSingle(raw, 88), BitConverter.ToSingle(raw, 92), BitConverter.ToSingle(raw, 96));
      this._BoundingBoxMax = new Point3D(BitConverter.ToSingle(raw, 100), BitConverter.ToSingle(raw, 104), BitConverter.ToSingle(raw, 108));
      this._ClippingBoxMin = new Point3D(BitConverter.ToSingle(raw, 112), BitConverter.ToSingle(raw, 116), BitConverter.ToSingle(raw, 120));
      this._ClippingBoxMax = new Point3D(BitConverter.ToSingle(raw, 124), BitConverter.ToSingle(raw, 128), BitConverter.ToSingle(raw, 132));
      this._TypeFlag = (TypeFlag) BitConverter.ToInt32(raw, 136);
      this._Bones = new CountOffsetPair(BitConverter.ToInt32(raw, 140), BitConverter.ToInt32(raw, 144));
      this._BoneControllers = new CountOffsetPair(BitConverter.ToInt32(raw, 148), BitConverter.ToInt32(raw, 152));
      this._HitBoxes = new CountOffsetPair(BitConverter.ToInt32(raw, 156), BitConverter.ToInt32(raw, 160));
      this._Sequences = new CountOffsetPair(BitConverter.ToInt32(raw, 164), BitConverter.ToInt32(raw, 168));
      this._SequenceGroups = new CountOffsetPair(BitConverter.ToInt32(raw, 172), BitConverter.ToInt32(raw, 176));
      this._Textures = new CountOffsetPair(BitConverter.ToInt32(raw, 180), BitConverter.ToInt32(raw, 184));
      this._TextureInfo = BitConverter.ToInt32(raw, 188);
      this._SkinReferences = BitConverter.ToInt32(raw, 192);
      this._SkinFamilies = BitConverter.ToInt32(raw, 196);
      this._SkinsOffset = BitConverter.ToInt32(raw, 200);
      this._BodyGroups = new CountOffsetPair(BitConverter.ToInt32(raw, 204), BitConverter.ToInt32(raw, 208));
      this._Attachments = new CountOffsetPair(BitConverter.ToInt32(raw, 212), BitConverter.ToInt32(raw, 216));
      this._SoundGroups = new CountOffsetPair(BitConverter.ToInt32(raw, 220), BitConverter.ToInt32(raw, 224));
      this._SoundsOffset = BitConverter.ToInt32(raw, 228);
      this._TransitionsCount = BitConverter.ToInt32(raw, 232);
      this._TransitionFlagsOffset = BitConverter.ToInt32(raw, 236);
      this._TransitionsOffset = BitConverter.ToInt32(raw, 240);
      this._LevelOfDetailFlags = (LevelOfDetailFlags) BitConverter.ToInt32(raw, 244);
      this._ModelHeaderOffset = BitConverter.ToInt32(raw, 248);
    }

    public override byte[] GetBytes()
    {
      byte[] bytes = new byte[HeaderV11.Length];
      Encoding.ASCII.GetBytes(this._Identifier).CopyTo((Array) bytes, 0);
      BitConverter.GetBytes(this._Version).CopyTo((Array) bytes, 4);
      Encoding.ASCII.GetBytes(this._Name).CopyTo((Array) bytes, 8);
      BitConverter.GetBytes(this._Length).CopyTo((Array) bytes, 72);
      this._EyePosition.GetBytes().CopyTo((Array) bytes, 76);
      this._BoundingBoxMin.GetBytes().CopyTo((Array) bytes, 88);
      this._BoundingBoxMax.GetBytes().CopyTo((Array) bytes, 100);
      this._ClippingBoxMin.GetBytes().CopyTo((Array) bytes, 112);
      this._ClippingBoxMax.GetBytes().CopyTo((Array) bytes, 124);
      BitConverter.GetBytes((int) this._TypeFlag).CopyTo((Array) bytes, 136);
      this._Bones.GetBytes().CopyTo((Array) bytes, 140);
      this._BoneControllers.GetBytes().CopyTo((Array) bytes, 148);
      this._HitBoxes.GetBytes().CopyTo((Array) bytes, 156);
      this._Sequences.GetBytes().CopyTo((Array) bytes, 164);
      this._SequenceGroups.GetBytes().CopyTo((Array) bytes, 172);
      this._Textures.GetBytes().CopyTo((Array) bytes, 180);
      BitConverter.GetBytes(this._TextureInfo).CopyTo((Array) bytes, 188);
      BitConverter.GetBytes(this._SkinReferences).CopyTo((Array) bytes, 192);
      BitConverter.GetBytes(this._SkinFamilies).CopyTo((Array) bytes, 196);
      BitConverter.GetBytes(this._SkinsOffset).CopyTo((Array) bytes, 200);
      this._BodyGroups.GetBytes().CopyTo((Array) bytes, 204);
      this._Attachments.GetBytes().CopyTo((Array) bytes, 212);
      this._SoundGroups.GetBytes().CopyTo((Array) bytes, 220);
      BitConverter.GetBytes(this._SoundsOffset).CopyTo((Array) bytes, 228);
      BitConverter.GetBytes(this._TransitionsCount).CopyTo((Array) bytes, 232);
      BitConverter.GetBytes(this._TransitionFlagsOffset).CopyTo((Array) bytes, 236);
      BitConverter.GetBytes(this._TransitionsOffset).CopyTo((Array) bytes, 240);
      BitConverter.GetBytes((int) this._LevelOfDetailFlags).CopyTo((Array) bytes, 244);
      BitConverter.GetBytes(this._ModelHeaderOffset).CopyTo((Array) bytes, 248);
      return bytes;
    }
  }
}
