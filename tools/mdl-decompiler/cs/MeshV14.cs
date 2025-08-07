using System;
using System.Text;
using JbnLib.Interfaces;

namespace JbnLib.Mdl
{
    public class MeshV14 : IStructure
    {
        public static int Length
        {
            get { return 32; }
        }

        private sbyte[] _BoneMappings = new sbyte[] { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
        public sbyte[] BoneMappings
        {
            get { return _BoneMappings; }
            set { _BoneMappings = value; }
        }

        private ushort _TriangleIndex;
        public ushort TriangleIndex
        {
            get { return _TriangleIndex; }
            set { _TriangleIndex = value; }
        }

        private ushort _TriangleCount;
        public ushort TriangleCount
        {
            get { return _TriangleCount; }
            set { _TriangleCount = value; }
        }

        private ushort _VertexIndexFrom;
        public ushort VertexIndexFrom
        {
            get { return _VertexIndexFrom; }
            set { _VertexIndexFrom = value; }
        }

        private ushort _VertexIndexTo;
        public ushort VertexIndexTo
        {
            get { return _VertexIndexTo; }
            set { _VertexIndexTo = value; }
        }

        public MeshV14(byte[] raw)
        {
            for (byte i = 0; i < _BoneMappings.Length; i++)
                _BoneMappings[i] = unchecked((sbyte)raw[i]);
            _TriangleIndex = BitConverter.ToUInt16(raw, 24);
            _TriangleCount = BitConverter.ToUInt16(raw, 26);
            _VertexIndexFrom = BitConverter.ToUInt16(raw, 28);
            _VertexIndexTo = BitConverter.ToUInt16(raw, 30);
        }
        public MeshV14()
        {
        }
        public MeshV14(sbyte[] bonemappings, ushort triangleindex, ushort trianglecount)
        {
            _BoneMappings = bonemappings;
            _TriangleIndex = triangleindex;
            _TriangleCount = trianglecount;
            _VertexIndexFrom = 0;
            _VertexIndexTo = 0;
        }
        public MeshV14(sbyte[] bonemappings, ushort triangleindex, ushort trianglecount, ushort vertexindexfrom, ushort vertexindexto)
        {
            _BoneMappings = bonemappings;
            _TriangleIndex = triangleindex;
            _TriangleCount = trianglecount;
            _VertexIndexFrom = vertexindexfrom;
            _VertexIndexTo = vertexindexto;
        }

        public byte[] GetBytes()
        {
            byte[] output = new byte[Length];
            for (byte i = 0; i < _BoneMappings.Length; i++)
                BitConverter.GetBytes(_BoneMappings[i]).CopyTo(output, i);
            BitConverter.GetBytes(_TriangleIndex).CopyTo(output, 24);
            BitConverter.GetBytes(_TriangleCount).CopyTo(output, 26);
            BitConverter.GetBytes(_VertexIndexFrom).CopyTo(output, 28);
            BitConverter.GetBytes(_VertexIndexTo).CopyTo(output, 30);
            return output;
        }
        public new string ToString()
        {
            byte maps = 0;
            for (byte i = 0; i < _BoneMappings.Length; i++)
            {
                if (_BoneMappings[i] != -1)
                    maps++;
            }
            return maps + ", " + _TriangleIndex + ", " + _TriangleCount;
        }
    }
}
