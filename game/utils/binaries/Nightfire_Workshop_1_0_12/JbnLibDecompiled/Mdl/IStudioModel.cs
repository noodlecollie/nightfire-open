// Decompiled with JetBrains decompiler
// Type: JbnLib.Mdl.IStudioModel
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using System.Collections.Generic;

namespace JbnLib.Mdl
{
  public interface IStudioModel
  {
    List<Bone> BoneCollection { get; set; }

    List<BoneController> BoneControllerCollection { get; set; }

    List<HitBox> HitBoxCollection { get; set; }

    List<Texture> TextureCollection { get; set; }

    List<TextureCoordinate> TextureCoordinateCollection { get; set; }

    SkinCollection SkinCollection { get; set; }

    List<Attachment> AttachmentCollection { get; set; }

    List<SoundGroup> SoundGroupCollection { get; set; }

    List<Vertex> VertexCollection { get; set; }

    List<TriangleMap> TriangleMapCollection { get; set; }

    List<BoneFixUp> BoneFixUpCollection { get; set; }

    List<Normal> NormalCollection { get; set; }
  }
}
