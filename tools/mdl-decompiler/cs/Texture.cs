using System;
using System.Text;
using JbnLib.Interfaces;

namespace JbnLib.Mdl
{
    public class Texture : IStructure
    {
        public static int Length
        {
            get { return 136; }
        }

        private string _MaterialName;
        public string MaterialName
        {
            get { return _MaterialName; }
            set { _MaterialName = value; }
        }

        private string _TextureName;
        public string TextureName
        {
            get { return _TextureName; }
            set { _TextureName = value; }
        }

        private int _ReferenceCount;
        public int ReferenceCount
        {
            get { return _ReferenceCount; }
            set { _ReferenceCount = value; }
        }

        private int _Unknown;
        public int Unknown
        {
            get { return _Unknown; }
            set { _Unknown = value; }
        }

        public Texture(byte[] raw)
        {
            _MaterialName = Encoding.ASCII.GetString(raw, 0, 64).Trim('\0');
            _TextureName = Encoding.ASCII.GetString(raw, 64, 64).Trim('\0');
            _ReferenceCount = BitConverter.ToInt32(raw, 128);
            _Unknown = BitConverter.ToInt32(raw, 132);
        }
        public Texture(string materialname, string texturename, int referencecount, int unknown)
        {
            _MaterialName = materialname;
            _TextureName = texturename;
            _ReferenceCount = referencecount;
            _Unknown = unknown;
        }

        public byte[] GetBytes()
        {
            byte[] output = new byte[Length];
            Encoding.ASCII.GetBytes(_MaterialName).CopyTo(output, 0);
            Encoding.ASCII.GetBytes(_TextureName).CopyTo(output, 64);
            BitConverter.GetBytes(_ReferenceCount).CopyTo(output, 128);
            BitConverter.GetBytes(_Unknown).CopyTo(output, 132);
            return output;
        }
        public new string ToString()
        {
            return _TextureName;
        }
    }
}
