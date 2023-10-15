// Decompiled with JetBrains decompiler
// Type: JbnLib.Bsp.DrawIndexV42
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using System;

namespace JbnLib.Bsp
{
  public class DrawIndexV42
  {
    private int _VertexIndex;

    public static int Length => 4;

    public int VertexIndex
    {
      get => this._VertexIndex;
      set => this._VertexIndex = value;
    }

    public DrawIndexV42(byte[] raw) => this._VertexIndex = BitConverter.ToInt32(raw, 0);

    public DrawIndexV42(int vertexindex) => this._VertexIndex = vertexindex;

    public byte[] GetBytes() => BitConverter.GetBytes(this._VertexIndex);

    public new string ToString() => this._VertexIndex.ToString();
  }
}
