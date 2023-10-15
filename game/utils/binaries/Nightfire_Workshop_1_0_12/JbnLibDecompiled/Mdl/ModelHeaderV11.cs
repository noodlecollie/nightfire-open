// Decompiled with JetBrains decompiler
// Type: JbnLib.Mdl.ModelHeaderV11
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Interfaces;
using System;

namespace JbnLib.Mdl
{
  public class ModelHeaderV11 : IStructure
  {
    private int _ModelCount;
    private int _VertexCount;
    private int _TriangleCount;
    private int _VerticesOffset;
    private int _Unknown0;
    private int _Unknown1;
    private int _Unknown2;
    private int _Unknown3;
    private int _BlendingOffset;
    private int _TriangleMapOffset;
    private int _NormalOffset;
    private int _TextureCoordsOffset;
    private int _BlendingScalesOffset;
    private int _BoneFixUpOffset;
    private int[] _ModelsOffset = new int[48];

    public static int Length => 248;

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

    public int VerticesOffset
    {
      get => this._VerticesOffset;
      set => this._VerticesOffset = value;
    }

    public int Unknown0
    {
      get => this._Unknown0;
      set => this._Unknown0 = value;
    }

    public int Unknown1
    {
      get => this._Unknown1;
      set => this._Unknown1 = value;
    }

    public int Unknown2
    {
      get => this._Unknown2;
      set => this._Unknown2 = value;
    }

    public int Unknown3
    {
      get => this._Unknown3;
      set => this._Unknown3 = value;
    }

    public int BlendingOffset
    {
      get => this._BlendingOffset;
      set => this._BlendingOffset = value;
    }

    public int TriangleMapOffset
    {
      get => this._TriangleMapOffset;
      set => this._TriangleMapOffset = value;
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

    public int BlendingScalesOffset
    {
      get => this._BlendingScalesOffset;
      set => this._BlendingScalesOffset = value;
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

    public ModelHeaderV11()
    {
    }

    public ModelHeaderV11(byte[] raw)
    {
      this._ModelCount = BitConverter.ToInt32(raw, 0);
      this._VertexCount = BitConverter.ToInt32(raw, 4);
      this._TriangleCount = BitConverter.ToInt32(raw, 8);
      this._VerticesOffset = BitConverter.ToInt32(raw, 12);
      this._Unknown0 = BitConverter.ToInt32(raw, 16);
      this._Unknown1 = BitConverter.ToInt32(raw, 20);
      this._Unknown2 = BitConverter.ToInt32(raw, 24);
      this._Unknown3 = BitConverter.ToInt32(raw, 28);
      this._BlendingOffset = BitConverter.ToInt32(raw, 32);
      this._TriangleMapOffset = BitConverter.ToInt32(raw, 36);
      this._NormalOffset = BitConverter.ToInt32(raw, 40);
      this._TextureCoordsOffset = BitConverter.ToInt32(raw, 44);
      this._BlendingScalesOffset = BitConverter.ToInt32(raw, 48);
      this._BoneFixUpOffset = BitConverter.ToInt32(raw, 52);
      for (int index = 0; index < this._ModelsOffset.Length; ++index)
        this._ModelsOffset[index] = BitConverter.ToInt32(raw, index * 4 + 56);
    }

    public byte[] GetBytes()
    {
      byte[] bytes = new byte[ModelHeaderV11.Length];
      BitConverter.GetBytes(this._ModelCount).CopyTo((Array) bytes, 0);
      BitConverter.GetBytes(this._VertexCount).CopyTo((Array) bytes, 4);
      BitConverter.GetBytes(this._TriangleCount).CopyTo((Array) bytes, 8);
      BitConverter.GetBytes(this._VerticesOffset).CopyTo((Array) bytes, 12);
      BitConverter.GetBytes(this._Unknown0).CopyTo((Array) bytes, 16);
      BitConverter.GetBytes(this._Unknown1).CopyTo((Array) bytes, 20);
      BitConverter.GetBytes(this._Unknown2).CopyTo((Array) bytes, 24);
      BitConverter.GetBytes(this._Unknown3).CopyTo((Array) bytes, 28);
      BitConverter.GetBytes(this._BlendingOffset).CopyTo((Array) bytes, 32);
      BitConverter.GetBytes(this._TriangleMapOffset).CopyTo((Array) bytes, 36);
      BitConverter.GetBytes(this._NormalOffset).CopyTo((Array) bytes, 40);
      BitConverter.GetBytes(this._TextureCoordsOffset).CopyTo((Array) bytes, 44);
      BitConverter.GetBytes(this._BlendingScalesOffset).CopyTo((Array) bytes, 48);
      BitConverter.GetBytes(this._BoneFixUpOffset).CopyTo((Array) bytes, 52);
      for (int index = 0; index < this._ModelsOffset.Length; ++index)
        BitConverter.GetBytes(this._ModelsOffset[index]).CopyTo((Array) bytes, index * 4 + 56);
      return bytes;
    }

    public new string ToString() => this._ModelCount.ToString() + ", " + (object) this._VertexCount + ", " + (object) this._TriangleCount;
  }
}
