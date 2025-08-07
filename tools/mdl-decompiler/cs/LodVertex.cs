using System;
using System.Text;
using JbnLib.Interfaces;

namespace JbnLib.Mdl
{
    public class LodVertex : IStructure
    {
        public static int Length
        {
            get { return 12; }
        }

        private short _LodBlendIndex;
        public short LodBlendIndex
        {
            get { return _LodBlendIndex; }
            set { _LodBlendIndex = value; }
        }

        private short _LodNormIndex;
        public short LodNormIndex
        {
            get { return _LodNormIndex; }
            set { _LodNormIndex = value; }
        }

        private TextureCoordinate _UV;
        public TextureCoordinate UV
        {
            get { return _UV; }
            set { _UV = value; }
        }

        public LodVertex(byte[] raw)
        {
            _LodBlendIndex = BitConverter.ToInt16(raw, 0);
            _LodNormIndex = BitConverter.ToInt16(raw, 2);
            _UV = new TextureCoordinate(BitConverter.ToSingle(raw, 4), BitConverter.ToSingle(raw, 8));
        }
        public LodVertex(short lodblendindex, short lodnormindex, float u, float v)
        {
            _LodBlendIndex = lodblendindex;
            _LodNormIndex = lodnormindex;
            _UV = new TextureCoordinate(u, v);
        }

        public byte[] GetBytes()
        {
            byte[] output = new byte[Length];
            BitConverter.GetBytes(_LodBlendIndex).CopyTo(output, 0);
            BitConverter.GetBytes(_LodNormIndex).CopyTo(output, 2);
            _UV.GetBytes().CopyTo(output, 4);
            return output;
        }
        public new string ToString()
        {
            return _LodBlendIndex + ", " + _LodNormIndex + ", " + _UV.ToString();
        }
    }
}
