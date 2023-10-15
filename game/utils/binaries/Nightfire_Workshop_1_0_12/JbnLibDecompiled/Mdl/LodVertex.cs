// Decompiled with JetBrains decompiler
// Type: JbnLib.Mdl.LodVertex
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Interfaces;
using System;

namespace JbnLib.Mdl
{
  public class LodVertex : IStructure
  {
    private short _LodBlendIndex;
    private short _LodNormIndex;
    private TextureCoordinate _UV;

    public static int Length => 12;

    public short LodBlendIndex
    {
      get => this._LodBlendIndex;
      set => this._LodBlendIndex = value;
    }

    public short LodNormIndex
    {
      get => this._LodNormIndex;
      set => this._LodNormIndex = value;
    }

    public TextureCoordinate UV
    {
      get => this._UV;
      set => this._UV = value;
    }

    public LodVertex(byte[] raw)
    {
      this._LodBlendIndex = BitConverter.ToInt16(raw, 0);
      this._LodNormIndex = BitConverter.ToInt16(raw, 2);
      this._UV = new TextureCoordinate(BitConverter.ToSingle(raw, 4), BitConverter.ToSingle(raw, 8));
    }

    public LodVertex(short lodblendindex, short lodnormindex, float u, float v)
    {
      this._LodBlendIndex = lodblendindex;
      this._LodNormIndex = lodnormindex;
      this._UV = new TextureCoordinate(u, v);
    }

    public byte[] GetBytes()
    {
      byte[] bytes = new byte[LodVertex.Length];
      BitConverter.GetBytes(this._LodBlendIndex).CopyTo((Array) bytes, 0);
      BitConverter.GetBytes(this._LodNormIndex).CopyTo((Array) bytes, 2);
      this._UV.GetBytes().CopyTo((Array) bytes, 4);
      return bytes;
    }

    public new string ToString() => this._LodBlendIndex.ToString() + ", " + (object) this._LodNormIndex + ", " + this._UV.ToString();
  }
}
