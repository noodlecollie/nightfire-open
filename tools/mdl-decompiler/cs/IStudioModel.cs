using System.Collections.Generic;

namespace JbnLib.Mdl
{
    public interface IStudioModel
    {
        List<Bone> BoneCollection { get; set;}
        List<BoneController> BoneControllerCollection { get; set;}
        List<HitBox> HitBoxCollection { get; set;}
        List<Texture> TextureCollection { get; set;}
        List<TextureCoordinate> TextureCoordinateCollection { get; set;}
        SkinCollection SkinCollection { get; set;}
        List<Attachment> AttachmentCollection { get; set;}
        List<SoundGroup> SoundGroupCollection { get; set;}
        List<Vertex> VertexCollection { get; set;}
        List<TriangleMap> TriangleMapCollection { get; set;}
        List<BoneFixUp> BoneFixUpCollection { get; set;}
        List<Normal> NormalCollection { get; set;}
    }
}
