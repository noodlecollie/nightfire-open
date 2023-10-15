// Decompiled with JetBrains decompiler
// Type: JbnLib.Mdl.HeaderV14
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Shared;
using System;
using System.Text;

namespace JbnLib.Mdl
{
  public class HeaderV14 : Header
  {
    private int _ModelCount;
    private int _VertexCount;
    private int _TriangleCount;
    private int _TriangleMapOffset;
    private int _VerticesOffset;
    private int _NormalOffset;
    private int _TextureCoordsOffset;
    private int _UselessOffset;
    private int _BlendingScalesOffset;
    private int _BlendingOffset;
    private int _BoneFixUpOffset;
    private int[] _ModelsOffset = new int[48];

    public new static int Length => 484;

    public int ModelCount
    {
      get => this._ModelCount;
      set => this._ModelCount = value;
    }

    public int VertexCount
    {
      get => this._VertexCount;
      set => this._VertexCount = value;
    }

    public int TriangleCount
    {
      get => this._TriangleCount;
      set => this._TriangleCount = value;
    }

    public int TriangleMapOffset
    {
      get => this._TriangleMapOffset;
      set => this._TriangleMapOffset = value;
    }

    public int VerticesOffset
    {
      get => this._VerticesOffset;
      set => this._VerticesOffset = value;
    }

    public int NormalOffset
    {
      get => this._NormalOffset;
      set => this._NormalOffset = value;
    }

    public int TextureCoordsOffset
    {
      get => this._TextureCoordsOffset;
      set => this._TextureCoordsOffset = value;
    }

    public int UselessOffset
    {
      get => this._UselessOffset;
      set => this._UselessOffset = value;
    }

    public int BlendingScalesOffset
    {
      get => this._BlendingScalesOffset;
      set => this._BlendingScalesOffset = value;
    }

    public int BlendingOffset
    {
      get => this._BlendingOffset;
      set => this._BlendingOffset = value;
    }

    public int BoneFixUpOffset
    {
      get => this._BoneFixUpOffset;
      set => this._BoneFixUpOffset = value;
    }

    public int[] ModelsOffset
    {
      get => this._ModelsOffset;
      set => this._ModelsOffset = value;
    }

    public HeaderV14()
    {
    }

    public HeaderV14(byte[] raw)
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
      this._ModelCount = BitConverter.ToInt32(raw, 248);
      this._VertexCount = BitConverter.ToInt32(raw, 252);
      this._TriangleCount = BitConverter.ToInt32(raw, 256);
      this._TriangleMapOffset = BitConverter.ToInt32(raw, 260);
      this._VerticesOffset = BitConverter.ToInt32(raw, 264);
      this._NormalOffset = BitConverter.ToInt32(raw, 268);
      this._TextureCoordsOffset = BitConverter.ToInt32(raw, 272);
      this._UselessOffset = BitConverter.ToInt32(raw, 276);
      this._BlendingScalesOffset = BitConverter.ToInt32(raw, 280);
      this._BlendingOffset = BitConverter.ToInt32(raw, 284);
      this._BoneFixUpOffset = BitConverter.ToInt32(raw, 288);
      for (int index = 0; index < this._ModelsOffset.Length; ++index)
        this._ModelsOffset[index] = BitConverter.ToInt32(raw, index * 4 + 292);
    }

    public override byte[] GetBytes()
    {
      byte[] bytes = new byte[HeaderV14.Length];
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
      BitConverter.GetBytes(this._ModelCount).CopyTo((Array) bytes, 248);
      BitConverter.GetBytes(this._VertexCount).CopyTo((Array) bytes, 252);
      BitConverter.GetBytes(this._TriangleCount).CopyTo((Array) bytes, 256);
      BitConverter.GetBytes(this._TriangleMapOffset).CopyTo((Array) bytes, 260);
      BitConverter.GetBytes(this._VerticesOffset).CopyTo((Array) bytes, 264);
      BitConverter.GetBytes(this._NormalOffset).CopyTo((Array) bytes, 268);
      BitConverter.GetBytes(this._TextureCoordsOffset).CopyTo((Array) bytes, 272);
      BitConverter.GetBytes(this._UselessOffset).CopyTo((Array) bytes, 276);
      BitConverter.GetBytes(this._BlendingScalesOffset).CopyTo((Array) bytes, 280);
      BitConverter.GetBytes(this._BlendingOffset).CopyTo((Array) bytes, 284);
      BitConverter.GetBytes(this._BoneFixUpOffset).CopyTo((Array) bytes, 288);
      for (int index = 0; index < this._ModelsOffset.Length; ++index)
        BitConverter.GetBytes(this._ModelsOffset[index]).CopyTo((Array) bytes, index * 4 + 292);
      return bytes;
    }
  }
}
