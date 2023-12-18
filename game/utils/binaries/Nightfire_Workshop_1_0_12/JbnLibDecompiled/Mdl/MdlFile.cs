// Decompiled with JetBrains decompiler
// Type: JbnLib.Mdl.MdlFile
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Lang;
using System.Collections.Generic;
using System.IO;
using System.Text;

namespace JbnLib.Mdl
{
  public class MdlFile : IStudioModel
  {
    public const string STRING_REF = "_ref";
    public const string STRING_ANI = "_ani";
    internal List<Sequence> _SequenceCollection;
    internal List<Bone> _BoneCollection;
    internal List<BoneController> _BoneControllerCollection;
    internal List<HitBox> _HitBoxCollection;
    internal List<Texture> _TextureCollection;
    internal List<TextureCoordinate> _TextureCoordinateCollection;
    internal SkinCollection _SkinCollection;
    internal List<Attachment> _AttachmentCollection;
    internal List<SoundGroup> _SoundGroupCollection;
    internal List<Vertex> _VertexCollection;
    internal List<BlendingScales> _BlendingScalesCollection;
    internal List<TriangleMap> _TriangleMapCollection;
    internal List<Blending> _BlendingCollection;
    internal List<BoneFixUp> _BoneFixUpCollection;
    internal List<SequenceGroup> _SequenceGroupCollection;
    internal List<Normal> _NormalCollection;
    internal List<LevelOfDetail> _LevelOfDetailCollection;

    public List<Sequence> SequenceCollection
    {
      get => this._SequenceCollection;
      set => this._SequenceCollection = value;
    }

    public List<Bone> BoneCollection
    {
      get => this._BoneCollection;
      set => this._BoneCollection = value;
    }

    public List<BoneController> BoneControllerCollection
    {
      get => this._BoneControllerCollection;
      set => this._BoneControllerCollection = value;
    }

    public List<HitBox> HitBoxCollection
    {
      get => this._HitBoxCollection;
      set => this._HitBoxCollection = value;
    }

    public List<Texture> TextureCollection
    {
      get => this._TextureCollection;
      set => this._TextureCollection = value;
    }

    public List<TextureCoordinate> TextureCoordinateCollection
    {
      get => this._TextureCoordinateCollection;
      set => this._TextureCoordinateCollection = value;
    }

    public SkinCollection SkinCollection
    {
      get => this._SkinCollection;
      set => this._SkinCollection = value;
    }

    public List<Attachment> AttachmentCollection
    {
      get => this._AttachmentCollection;
      set => this._AttachmentCollection = value;
    }

    public List<SoundGroup> SoundGroupCollection
    {
      get => this._SoundGroupCollection;
      set => this._SoundGroupCollection = value;
    }

    public List<Vertex> VertexCollection
    {
      get => this._VertexCollection;
      set => this._VertexCollection = value;
    }

    public List<BlendingScales> BlendingScalesCollection
    {
      get => this._BlendingScalesCollection;
      set => this._BlendingScalesCollection = value;
    }

    public List<TriangleMap> TriangleMapCollection
    {
      get => this._TriangleMapCollection;
      set => this._TriangleMapCollection = value;
    }

    public List<Blending> BlendingCollection
    {
      get => this._BlendingCollection;
      set => this._BlendingCollection = value;
    }

    public List<BoneFixUp> BoneFixUpCollection
    {
      get => this._BoneFixUpCollection;
      set => this._BoneFixUpCollection = value;
    }

    public List<SequenceGroup> SequenceGroupCollection
    {
      get => this._SequenceGroupCollection;
      set => this._SequenceGroupCollection = value;
    }

    public List<Normal> NormalCollection
    {
      get => this._NormalCollection;
      set => this._NormalCollection = value;
    }

    public List<LevelOfDetail> LevelOfDetailCollection
    {
      get => this._LevelOfDetailCollection;
      set => this._LevelOfDetailCollection = value;
    }

    public static int GetVersion(string mdl) => MdlFile.GetVersion(mdl, 4L);

    public static int GetVersion(string mdl, long offset)
    {
      if (File.Exists(mdl))
      {
        int version = -1;
        FileStream input = new FileStream(mdl, FileMode.Open);
        if (input.Length >= 8L)
        {
          BinaryReader binaryReader = new BinaryReader((Stream) input);
          input.Seek(offset, SeekOrigin.Begin);
          version = binaryReader.ReadInt32();
          binaryReader.Close();
        }
        input.Close();
        return version;
      }
      Messages.ThrowException("Mdl.MdlFile.GetVersion(string)", Messages.FILE_NOT_FOUND + mdl);
      return -1;
    }

    public static string[] GetTextures(string mdl) => MdlFile.GetTextures(mdl, 180L);

    public static string[] GetTextures(string mdl, long offset)
    {
      FileStream input = new FileStream(mdl, FileMode.Open);
      BinaryReader binaryReader = new BinaryReader((Stream) input);
      input.Seek(offset, SeekOrigin.Begin);
      int length = binaryReader.ReadInt32();
      int offset1 = binaryReader.ReadInt32() + 64;
      string[] textures = new string[length];
      input.Seek((long) offset1, SeekOrigin.Begin);
      for (byte index = 0; (int) index < textures.Length; ++index)
      {
        textures[(int) index] = Encoding.ASCII.GetString(binaryReader.ReadBytes(64)).Trim(new char[1]);
        int num = textures[(int) index].LastIndexOf('"');
        if (num != -1)
          textures[(int) index] = textures[(int) index].Substring(num + 1);
        offset1 += 136;
        input.Seek((long) offset1, SeekOrigin.Begin);
      }
      return textures;
    }

    public static string GetName(string mdl) => MdlFile.GetName(mdl, 8L, 64);

    public static string GetName(string mdl, long offset, int length)
    {
      if (File.Exists(mdl))
      {
        string name = "";
        FileStream input = new FileStream(mdl, FileMode.Open);
        if (input.Length >= offset + (long) length)
        {
          BinaryReader binaryReader = new BinaryReader((Stream) input);
          input.Seek(offset, SeekOrigin.Begin);
          name = Encoding.ASCII.GetString(binaryReader.ReadBytes(length)).TrimEnd(new char[1]);
          binaryReader.Close();
        }
        input.Close();
        return name;
      }
      Messages.ThrowException("Mdl.MdlFile.GetName(string, long, int)", Messages.FILE_NOT_FOUND + mdl);
      return "";
    }
  }
}
