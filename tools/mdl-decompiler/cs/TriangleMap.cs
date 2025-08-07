using System;
using JbnLib.Interfaces;

namespace JbnLib.Mdl
{
    public class TriangleMap : IStructure
    {
        public static int Length
        {
            get { return 2; }
        }

        private ushort _VertexIndex;
        public ushort VertexIndex
        {
            get { return _VertexIndex; }
            set { _VertexIndex = value; }
        }

        public TriangleMap(byte[] raw)
        {
            _VertexIndex = BitConverter.ToUInt16(raw, 0);
        }
        public TriangleMap(ushort vertexindex)
        {
            _VertexIndex = vertexindex;
        }

        public byte[] GetBytes()
        {
            return BitConverter.GetBytes(_VertexIndex);
        }
        public new string ToString()
        {
            return _VertexIndex.ToString();
        }
    }
}
