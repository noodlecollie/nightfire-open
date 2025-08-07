using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using JbnLib.Interfaces;
using JbnLib.Lang;

namespace JbnLib.Mdl
{
    public class MdlFile : IStudioModel
    {
        public const string STRING_REF = "_ref";
        public const string STRING_ANI = "_ani";

        #region " Collections "

        internal List<Sequence> _SequenceCollection;
        public List<Sequence> SequenceCollection
        {
            get { return _SequenceCollection; }
            set { _SequenceCollection = value; }
        }

        internal List<Bone> _BoneCollection;
        public List<Bone> BoneCollection
        {
            get { return _BoneCollection; }
            set { _BoneCollection = value; }
        }

        internal List<BoneController> _BoneControllerCollection;
        public List<BoneController> BoneControllerCollection
        {
            get { return _BoneControllerCollection; }
            set { _BoneControllerCollection = value; }
        }

        internal List<HitBox> _HitBoxCollection;
        public List<HitBox> HitBoxCollection
        {
            get { return _HitBoxCollection; }
            set { _HitBoxCollection = value; }
        }

        internal List<Texture> _TextureCollection;
        public List<Texture> TextureCollection
        {
            get { return _TextureCollection; }
            set { _TextureCollection = value; }
        }

        internal List<TextureCoordinate> _TextureCoordinateCollection;
        public List<TextureCoordinate> TextureCoordinateCollection
        {
            get { return _TextureCoordinateCollection; }
            set { _TextureCoordinateCollection = value; }
        }

        internal SkinCollection _SkinCollection;
        public SkinCollection SkinCollection
        {
            get { return _SkinCollection; }
            set { _SkinCollection = value; }
        }

        internal List<Attachment> _AttachmentCollection;
        public List<Attachment> AttachmentCollection
        {
            get { return _AttachmentCollection; }
            set { _AttachmentCollection = value; }
        }

        internal List<SoundGroup> _SoundGroupCollection;
        public List<SoundGroup> SoundGroupCollection
        {
            get { return _SoundGroupCollection; }
            set { _SoundGroupCollection = value; }
        }

        internal List<Vertex> _VertexCollection;
        public List<Vertex> VertexCollection
        {
            get { return _VertexCollection; }
            set { _VertexCollection = value; }
        }

        internal List<BlendingScales> _BlendingScalesCollection;
        public List<BlendingScales> BlendingScalesCollection
        {
            get { return _BlendingScalesCollection; }
            set { _BlendingScalesCollection = value; }
        }

        internal List<TriangleMap> _TriangleMapCollection;
        public List<TriangleMap> TriangleMapCollection
        {
            get { return _TriangleMapCollection; }
            set { _TriangleMapCollection = value; }
        }

        internal List<Blending> _BlendingCollection;
        public List<Blending> BlendingCollection
        {
            get { return _BlendingCollection; }
            set { _BlendingCollection = value; }
        }

        internal List<BoneFixUp> _BoneFixUpCollection;
        public List<BoneFixUp> BoneFixUpCollection
        {
            get { return _BoneFixUpCollection; }
            set { _BoneFixUpCollection = value; }
        }

        internal List<SequenceGroup> _SequenceGroupCollection;
        public List<SequenceGroup> SequenceGroupCollection
        {
            get { return _SequenceGroupCollection; }
            set { _SequenceGroupCollection = value; }
        }

        internal List<Normal> _NormalCollection;
        public List<Normal> NormalCollection
        {
            get { return _NormalCollection; }
            set { _NormalCollection = value; }
        }

        internal List<LevelOfDetail> _LevelOfDetailCollection;
        public List<LevelOfDetail> LevelOfDetailCollection
        {
            get { return _LevelOfDetailCollection; }
            set { _LevelOfDetailCollection = value; }
        }
        #endregion

        #region " Static Methods "
        /// <summary>
        /// Gets the version from a binary studio model file. 
        /// </summary>
        /// <param name="mdl">Path to a .mdl file.</param>
        /// <returns>Version number or -1 on failure (file not long enough to get version).</returns>
        public static int GetVersion(string mdl)
        {
            return GetVersion(mdl, 4);
        }
        /// <summary>
        /// Gets the version from a binary studio model file. 
        /// </summary>
        /// <param name="mdl">Path to a .mdl file.</param>
        /// <param name="offset">Offset from the beginning of the file to the version number.</param>
        /// <returns>Version number or -1 on failure (file not long enough to get version).</returns>
        public static int GetVersion(string mdl, long offset)
        {
            if (File.Exists(mdl))
            {
                int version = -1;
                FileStream fs = new FileStream(mdl, FileMode.Open);
                if (fs.Length >= 8)
                {
                    BinaryReader br = new BinaryReader(fs);
                    fs.Seek(offset, SeekOrigin.Begin);
                    version = br.ReadInt32();
                    br.Close();
                }
                fs.Close();
                return version;
            }
            else
                Messages.ThrowException("Mdl.MdlFile.GetVersion(string)", Messages.FILE_NOT_FOUND + mdl);
            return -1;
        }

        /// <summary>
        /// Used for getting the count and name of the textures used by a model.
        /// </summary>
        /// <param name="mdl">The path to the file to get textures from.</param>
        /// <returns>An array of all textures listed in the model.</returns>
        public static string[] GetTextures(string mdl)
        {
            return GetTextures(mdl, 180);
        }
        /// <summary>
        /// Used for getting the count and name of the textures used by a model.
        /// </summary>
        /// <param name="mdl">The path to the file to get textures from.</param>
        /// <param name="offset">This is the offset to the texture count (in header) from the beginning of the file.</param>
        /// <returns>An array of all textures listed in the model.</returns>
        public static string[] GetTextures(string mdl, long offset)
        {
            FileStream fs = new FileStream(mdl, FileMode.Open);
            BinaryReader br = new BinaryReader(fs);

            // Get header info
            fs.Seek(offset, SeekOrigin.Begin);
            int count = br.ReadInt32();
            int toffset = br.ReadInt32() + 64;

            // Prepare output array
            string[] output = new string[count];

            // Get texture data
            fs.Seek(toffset, SeekOrigin.Begin);
            for (byte i = 0; i < output.Length; i++)
            {
                output[i] = Encoding.ASCII.GetString(br.ReadBytes(64)).Trim('\0');
                int index = output[i].LastIndexOf('"');
                if (index != -1)
                    output[i] = output[i].Substring(index + 1);
                toffset += 136;
                fs.Seek(toffset, SeekOrigin.Begin);
            }

            return output;
        }

        /// <summary>
        /// Gets the internal name from a model.
        /// </summary>
        /// <param name="mdl">Path to a .mdl file.</param>
        /// <returns>The name read from the file or an empty string if there was a problem.</returns>
        public static string GetName(string mdl)
        {
            return GetName(mdl, 8, 64);
        }
        /// <summary>
        /// Gets the internal name from a model.
        /// </summary>
        /// <param name="mdl">Path to a .mdl file.</param>
        /// <param name="offset">The offset to the name from the beginning of the file.</param>
        /// <param name="length">How many bytes to read.  It will trim off any 0x00 chars.</param>
        /// <returns>The name read from the file or an empty string if there was a problem.</returns>
        public static string GetName(string mdl, long offset, int length)
        {
            if (File.Exists(mdl))
            {
                string name = "";
                FileStream fs = new FileStream(mdl, FileMode.Open);
                if (fs.Length >= (offset + length))
                {
                    BinaryReader br = new BinaryReader(fs);
                    fs.Seek(offset, SeekOrigin.Begin);
                    name = Encoding.ASCII.GetString(br.ReadBytes(length)).TrimEnd('\0');
                    br.Close();
                }
                fs.Close();
                return name;
            }
            else
                Messages.ThrowException("Mdl.MdlFile.GetName(string, long, int)", Messages.FILE_NOT_FOUND + mdl);
            return "";
        }
        #endregion
    }
}
