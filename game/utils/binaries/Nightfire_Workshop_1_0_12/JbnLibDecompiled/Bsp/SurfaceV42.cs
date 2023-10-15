// Decompiled with JetBrains decompiler
// Type: JbnLib.Bsp.SurfaceV42
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Map;
using System;
using System.Collections.Generic;

namespace JbnLib.Bsp
{
  public class SurfaceV42
  {
    private int _PlaneIndex;
    private int _VertexIndex;
    private int _VertexCount;
    private int _DrawIndex;
    private int _DrawCount;
    private SurfaceFlags _Flags;
    private int _ShaderIndex;
    private int _FogIndex;
    private int _TextureMapShaderIndex;
    private int _TextureMapLightingIndex;
    private sbyte[] _Styles = new sbyte[4];
    private int _LightingOffset;
    private List<LumelV42> _LumelCollection = new List<LumelV42>();

    public static int Length => 48;

    public int PlaneIndex
    {
      get => this._PlaneIndex;
      set => this._PlaneIndex = value;
    }

    public int VertexIndex
    {
      get => this._VertexIndex;
      set => this._VertexIndex = value;
    }

    public int VertexCount
    {
      get => this._VertexCount;
      set => this._VertexCount = value;
    }

    public int DrawIndex
    {
      get => this._DrawIndex;
      set => this._DrawIndex = value;
    }

    public int DrawCount
    {
      get => this._DrawCount;
      set => this._DrawCount = value;
    }

    public SurfaceFlags Flags
    {
      get => this._Flags;
      set => this._Flags = value;
    }

    public int ShaderIndex
    {
      get => this._ShaderIndex;
      set => this._ShaderIndex = value;
    }

    public int FogIndex
    {
      get => this._FogIndex;
      set => this._FogIndex = value;
    }

    public int TextureMapShaderIndex
    {
      get => this._TextureMapShaderIndex;
      set => this._TextureMapShaderIndex = value;
    }

    public int TextureMapLightingIndex
    {
      get => this._TextureMapLightingIndex;
      set => this._TextureMapLightingIndex = value;
    }

    public sbyte[] Styles
    {
      get => this._Styles;
      set => this._Styles = value;
    }

    public int LightingOffset
    {
      get => this._LightingOffset;
      set => this._LightingOffset = value;
    }

    public List<LumelV42> LumelCollection
    {
      get => this._LumelCollection;
      set => this._LumelCollection = value;
    }

    public SurfaceV42(byte[] raw)
    {
      this._PlaneIndex = BitConverter.ToInt32(raw, 0);
      this._VertexIndex = BitConverter.ToInt32(raw, 4);
      this._VertexCount = BitConverter.ToInt32(raw, 8);
      this._DrawIndex = BitConverter.ToInt32(raw, 12);
      this._DrawCount = BitConverter.ToInt32(raw, 16);
      this._Flags = (SurfaceFlags) BitConverter.ToInt32(raw, 20);
      this._ShaderIndex = BitConverter.ToInt32(raw, 24);
      this._FogIndex = BitConverter.ToInt32(raw, 28);
      this._TextureMapShaderIndex = BitConverter.ToInt32(raw, 32);
      this._TextureMapLightingIndex = BitConverter.ToInt32(raw, 36);
      for (byte index = 0; index < (byte) 4; ++index)
        this._Styles[(int) index] = (sbyte) raw[(int) index + 40];
      this._LightingOffset = BitConverter.ToInt32(raw, 44);
    }

    public SurfaceV42(
      int planeindex,
      int vertexindex,
      int vertexcount,
      int drawindex,
      int drawcount,
      SurfaceFlags flags,
      int shaderindex,
      int fogindex,
      int texturemapshaderindex,
      int texturemaplightingindex,
      sbyte[] styles,
      int lightmapoffset)
    {
      this._PlaneIndex = planeindex;
      this._VertexIndex = vertexindex;
      this._VertexCount = vertexcount;
      this._DrawIndex = drawindex;
      this._DrawCount = drawcount;
      this._Flags = flags;
      this._ShaderIndex = shaderindex;
      this._FogIndex = fogindex;
      this._TextureMapShaderIndex = texturemapshaderindex;
      this._TextureMapLightingIndex = texturemaplightingindex;
      for (byte index = 0; index < (byte) 4; ++index)
        this._Styles[(int) index] = styles[(int) index];
      this._LightingOffset = lightmapoffset;
    }

    public byte[] GetBytes()
    {
      byte[] bytes = new byte[SurfaceV42.Length];
      BitConverter.GetBytes(this._PlaneIndex).CopyTo((Array) bytes, 0);
      BitConverter.GetBytes(this._VertexIndex).CopyTo((Array) bytes, 4);
      BitConverter.GetBytes(this._VertexCount).CopyTo((Array) bytes, 8);
      BitConverter.GetBytes(this._DrawIndex).CopyTo((Array) bytes, 12);
      BitConverter.GetBytes(this._DrawCount).CopyTo((Array) bytes, 16);
      BitConverter.GetBytes((int) this._Flags).CopyTo((Array) bytes, 20);
      BitConverter.GetBytes(this._ShaderIndex).CopyTo((Array) bytes, 24);
      BitConverter.GetBytes(this._FogIndex).CopyTo((Array) bytes, 28);
      BitConverter.GetBytes(this._TextureMapShaderIndex).CopyTo((Array) bytes, 32);
      BitConverter.GetBytes(this._TextureMapLightingIndex).CopyTo((Array) bytes, 36);
      for (byte index = 0; index < (byte) 4; ++index)
        bytes[(int) index + 40] = (byte) this._Styles[(int) index];
      BitConverter.GetBytes(this._LightingOffset).CopyTo((Array) bytes, 44);
      return bytes;
    }
  }
}
