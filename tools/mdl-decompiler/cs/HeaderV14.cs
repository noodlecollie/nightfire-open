using System;
using System.Text;
using JbnLib.Shared;

namespace JbnLib.Mdl
{
    public class HeaderV14 : Header
    {
        public new static int Length
        {
            get { return 484; }
        }

        private int _ModelCount;
        public int ModelCount
        {
            get { return _ModelCount; }
            set { _ModelCount = value; }
        }

        private int _VertexCount;
        public int VertexCount
        {
            get { return _VertexCount; }
            set { _VertexCount = value; }
        }

        private int _TriangleCount;
        public int TriangleCount
        {
            get { return _TriangleCount; }
            set { _TriangleCount = value; }
        }

        private int _TriangleMapOffset;
        public int TriangleMapOffset
        {
            get { return _TriangleMapOffset; }
            set { _TriangleMapOffset = value; }
        }

        private int _VerticesOffset;
        public int VerticesOffset
        {
            get { return _VerticesOffset; }
            set { _VerticesOffset = value; }
        }

        private int _NormalOffset;
        public int NormalOffset
        {
            get { return _NormalOffset; }
            set { _NormalOffset = value; }
        }

        private int _TextureCoordsOffset;
        public int TextureCoordsOffset
        {
            get { return _TextureCoordsOffset; }
            set { _TextureCoordsOffset = value; }
        }

        private int _UselessOffset;
        public int UselessOffset
        {
            get { return _UselessOffset; }
            set { _UselessOffset = value; }
        }

        private int _BlendingScalesOffset;
        public int BlendingScalesOffset
        {
            get { return _BlendingScalesOffset; }
            set { _BlendingScalesOffset = value; }
        }

        private int _BlendingOffset;
        public int BlendingOffset
        {
            get { return _BlendingOffset; }
            set { _BlendingOffset = value; }
        }

        private int _BoneFixUpOffset;
        public int BoneFixUpOffset
        {
            get { return _BoneFixUpOffset; }
            set { _BoneFixUpOffset = value; }
        }

        private int[] _ModelsOffset = new int[48];
        public int[] ModelsOffset
        {
            get { return _ModelsOffset; }
            set { _ModelsOffset = value; }
        }
        
        public HeaderV14()
        {
        }
        public HeaderV14(byte[] raw)
        {
            _Identifier = Encoding.ASCII.GetString(raw, 0, 4);
            _Version = BitConverter.ToInt32(raw, 4);
            _Name = Encoding.ASCII.GetString(raw, 8, 64).Trim('\0');
            _Length = BitConverter.ToInt32(raw, 72);
            _EyePosition = new EyePosition(BitConverter.ToSingle(raw, 76), BitConverter.ToSingle(raw, 80), BitConverter.ToSingle(raw, 84));
            _BoundingBoxMin = new Point3D(BitConverter.ToSingle(raw, 88), BitConverter.ToSingle(raw, 92), BitConverter.ToSingle(raw, 96));
            _BoundingBoxMax = new Point3D(BitConverter.ToSingle(raw, 100), BitConverter.ToSingle(raw, 104), BitConverter.ToSingle(raw, 108));
            _ClippingBoxMin = new Point3D(BitConverter.ToSingle(raw, 112), BitConverter.ToSingle(raw, 116), BitConverter.ToSingle(raw, 120));
            _ClippingBoxMax = new Point3D(BitConverter.ToSingle(raw, 124), BitConverter.ToSingle(raw, 128), BitConverter.ToSingle(raw, 132));
            _TypeFlag = (TypeFlag)BitConverter.ToInt32(raw, 136);
            _Bones = new CountOffsetPair(BitConverter.ToInt32(raw, 140), BitConverter.ToInt32(raw, 144));
            _BoneControllers = new CountOffsetPair(BitConverter.ToInt32(raw, 148), BitConverter.ToInt32(raw, 152));
            _HitBoxes = new CountOffsetPair(BitConverter.ToInt32(raw, 156), BitConverter.ToInt32(raw, 160));
            _Sequences = new CountOffsetPair(BitConverter.ToInt32(raw, 164), BitConverter.ToInt32(raw, 168));
            _SequenceGroups = new CountOffsetPair(BitConverter.ToInt32(raw, 172), BitConverter.ToInt32(raw, 176));
            _Textures = new CountOffsetPair(BitConverter.ToInt32(raw, 180), BitConverter.ToInt32(raw, 184));
            _TextureInfo = BitConverter.ToInt32(raw, 188);
            _SkinReferences = BitConverter.ToInt32(raw, 192);
            _SkinFamilies = BitConverter.ToInt32(raw, 196);
            _SkinsOffset = BitConverter.ToInt32(raw, 200);
            _BodyGroups = new CountOffsetPair(BitConverter.ToInt32(raw, 204), BitConverter.ToInt32(raw, 208));
            _Attachments = new CountOffsetPair(BitConverter.ToInt32(raw, 212), BitConverter.ToInt32(raw, 216));
            _SoundGroups = new CountOffsetPair(BitConverter.ToInt32(raw, 220), BitConverter.ToInt32(raw, 224));
            _SoundsOffset = BitConverter.ToInt32(raw, 228);
            _TransitionsCount = BitConverter.ToInt32(raw, 232);
            _TransitionFlagsOffset = BitConverter.ToInt32(raw, 236);
            _TransitionsOffset = BitConverter.ToInt32(raw, 240);
            _LevelOfDetailFlags = (LevelOfDetailFlags)BitConverter.ToInt32(raw, 244);
            _ModelCount = BitConverter.ToInt32(raw, 248);
            _VertexCount = BitConverter.ToInt32(raw, 252);
            _TriangleCount = BitConverter.ToInt32(raw, 256);
            _TriangleMapOffset = BitConverter.ToInt32(raw, 260);
            _VerticesOffset = BitConverter.ToInt32(raw, 264);
            _NormalOffset = BitConverter.ToInt32(raw, 268);
            _TextureCoordsOffset = BitConverter.ToInt32(raw, 272);
            _UselessOffset = BitConverter.ToInt32(raw, 276);
            _BlendingScalesOffset = BitConverter.ToInt32(raw, 280);
            _BlendingOffset = BitConverter.ToInt32(raw, 284);
            _BoneFixUpOffset = BitConverter.ToInt32(raw, 288);
            for (int i = 0; i < _ModelsOffset.Length; i++)
                _ModelsOffset[i] = BitConverter.ToInt32(raw, (i * 4) + 292);
        }

        public override byte[] GetBytes()
        {
            byte[] output = new byte[Length];
            Encoding.ASCII.GetBytes(_Identifier).CopyTo(output, 0);
            BitConverter.GetBytes(_Version).CopyTo(output, 4);
            Encoding.ASCII.GetBytes(_Name).CopyTo(output, 8);
            BitConverter.GetBytes(_Length).CopyTo(output, 72);
            _EyePosition.GetBytes().CopyTo(output, 76);
            _BoundingBoxMin.GetBytes().CopyTo(output, 88);
            _BoundingBoxMax.GetBytes().CopyTo(output, 100);
            _ClippingBoxMin.GetBytes().CopyTo(output, 112);
            _ClippingBoxMax.GetBytes().CopyTo(output, 124);
            BitConverter.GetBytes((int)_TypeFlag).CopyTo(output, 136);
            _Bones.GetBytes().CopyTo(output, 140);
            _BoneControllers.GetBytes().CopyTo(output, 148);
            _HitBoxes.GetBytes().CopyTo(output, 156);
            _Sequences.GetBytes().CopyTo(output, 164);
            _SequenceGroups.GetBytes().CopyTo(output, 172);
            _Textures.GetBytes().CopyTo(output, 180);
            BitConverter.GetBytes(_TextureInfo).CopyTo(output, 188);
            BitConverter.GetBytes(_SkinReferences).CopyTo(output, 192);
            BitConverter.GetBytes(_SkinFamilies).CopyTo(output, 196);
            BitConverter.GetBytes(_SkinsOffset).CopyTo(output, 200);
            _BodyGroups.GetBytes().CopyTo(output, 204);
            _Attachments.GetBytes().CopyTo(output, 212);
            _SoundGroups.GetBytes().CopyTo(output, 220);
            BitConverter.GetBytes(_SoundsOffset).CopyTo(output, 228);
            BitConverter.GetBytes(_TransitionsCount).CopyTo(output, 232);
            BitConverter.GetBytes(_TransitionFlagsOffset).CopyTo(output, 236);
            BitConverter.GetBytes(_TransitionsOffset).CopyTo(output, 240);
            BitConverter.GetBytes((int)_LevelOfDetailFlags).CopyTo(output, 244);
            BitConverter.GetBytes(_ModelCount).CopyTo(output, 248);
            BitConverter.GetBytes(_VertexCount).CopyTo(output, 252);
            BitConverter.GetBytes(_TriangleCount).CopyTo(output, 256);
            BitConverter.GetBytes(_TriangleMapOffset).CopyTo(output, 260);
            BitConverter.GetBytes(_VerticesOffset).CopyTo(output, 264);
            BitConverter.GetBytes(_NormalOffset).CopyTo(output, 268);
            BitConverter.GetBytes(_TextureCoordsOffset).CopyTo(output, 272);
            BitConverter.GetBytes(_UselessOffset).CopyTo(output, 276);
            BitConverter.GetBytes(_BlendingScalesOffset).CopyTo(output, 280);
            BitConverter.GetBytes(_BlendingOffset).CopyTo(output, 284);
            BitConverter.GetBytes(_BoneFixUpOffset).CopyTo(output, 288);
            for (int i = 0; i < _ModelsOffset.Length; i++)
                BitConverter.GetBytes(_ModelsOffset[i]).CopyTo(output, (i * 4) + 292);
            return output;
        }
    }
}
