using System;
using JbnLib.Interfaces;
using JbnLib.Lang;
using JbnLib.Shared;

namespace JbnLib.Mdl
{
    public class Header : IHeader
    {
        public static int Length
        {
            get { return 248; }
        }

        internal string _Identifier;
        public string Identifier
        {
            get { return _Identifier; }
            set { _Identifier = value; }
        }

        internal int _Version;
        public int Version
        {
            get { return _Version; }
            set { _Version = value; }
        }

        internal string _Name;
        public string Name
        {
            get { return _Name; }
            set { _Name = value; }
        }

        internal int _Length;
        public int FileLength
        {
            get { return _Length; }
            set { _Length = value; }
        }

        internal EyePosition _EyePosition;
        public EyePosition EyePosition
        {
            get { return _EyePosition; }
            set { _EyePosition = value; }
        }

        internal Point3D _BoundingBoxMin;
        public Point3D BoundingBoxMin
        {
            get { return _BoundingBoxMin; }
            set { _BoundingBoxMin = value; }
        }

        internal Point3D _BoundingBoxMax;
        public Point3D BoundingBoxMax
        {
            get { return _BoundingBoxMax; }
            set { _BoundingBoxMax = value; }
        }

        internal Point3D _ClippingBoxMin;
        public Point3D ClippingBoxMin
        {
            get { return _ClippingBoxMin; }
            set { _ClippingBoxMin = value; }
        }

        internal Point3D _ClippingBoxMax;
        public Point3D ClippingBoxMax
        {
            get { return _ClippingBoxMax; }
            set { _ClippingBoxMax = value; }
        }

        internal TypeFlag _TypeFlag;
        public TypeFlag TypeFlag
        {
            get { return _TypeFlag; }
            set { _TypeFlag = value; }
        }

        internal CountOffsetPair _Bones;
        public CountOffsetPair Bones
        {
            get { return _Bones; }
            set { _Bones = value; }
        }

        internal CountOffsetPair _BoneControllers;
        public CountOffsetPair BoneControllers
        {
            get { return _BoneControllers; }
            set { _BoneControllers = value; }
        }

        internal CountOffsetPair _HitBoxes;
        public CountOffsetPair HitBoxes
        {
            get { return _HitBoxes; }
            set { _HitBoxes = value; }
        }

        internal CountOffsetPair _Sequences;
        public CountOffsetPair Sequences
        {
            get { return _Sequences; }
            set { _Sequences = value; }
        }

        internal CountOffsetPair _SequenceGroups;
        public CountOffsetPair SequenceGroups
        {
            get { return _SequenceGroups; }
            set { _SequenceGroups = value; }
        }

        internal CountOffsetPair _Textures;
        public CountOffsetPair Textures
        {
            get { return _Textures; }
            set { _Textures = value; }
        }

        internal int _TextureInfo;
        public int TextureInfo
        {
            get { return _TextureInfo; }
            set { _TextureInfo = value; }
        }

        internal int _SkinReferences;
        public int SkinReferences
        {
            get { return _SkinReferences; }
            set { _SkinReferences = value; }
        }

        internal int _SkinFamilies;
        public int SkinFamilies
        {
            get { return _SkinFamilies; }
            set { _SkinFamilies = value; }
        }

        internal int _SkinsOffset;
        public int SkinsOffset
        {
            get { return _SkinsOffset; }
            set { _SkinsOffset = value; }
        }

        internal CountOffsetPair _BodyGroups;
        public CountOffsetPair BodyGroups
        {
            get { return _BodyGroups; }
            set { _BodyGroups = value; }
        }

        internal CountOffsetPair _Attachments;
        public CountOffsetPair Attachments
        {
            get { return _Attachments; }
            set { _Attachments = value; }
        }

        internal CountOffsetPair _SoundGroups;
        public CountOffsetPair SoundGroups
        {
            get { return _SoundGroups; }
            set { _SoundGroups = value; }
        }

        internal int _SoundsOffset;
        public int SoundsOffset
        {
            get { return _SoundsOffset; }
            set { _SoundsOffset = value; }
        }

        internal int _TransitionsCount;
        public int TransitionsCount
        {
            get { return _TransitionsCount; }
            set { _TransitionsCount = value; }
        }

        internal int _TransitionFlagsOffset;
        public int TransitionFlagsOffset
        {
            get { return _TransitionFlagsOffset; }
            set { _TransitionFlagsOffset = value; }
        }

        internal int _TransitionsOffset;
        public int TransitionsOffset
        {
            get { return _TransitionsOffset; }
            set { _TransitionsOffset = value; }
        }

        internal LevelOfDetailFlags _LevelOfDetailFlags;
        public LevelOfDetailFlags LevelOfDetailFlags
        {
            get { return _LevelOfDetailFlags; }
            set { _LevelOfDetailFlags = value; }
        }

        public virtual byte[] GetBytes()
        {
            Messages.ThrowException("Mdl.Header.GetBytes()", Messages.METHOD_OVERRIDE);
            return new byte[0];
        }
        public new string ToString()
        {
            return _Name;
        }
    }
}
