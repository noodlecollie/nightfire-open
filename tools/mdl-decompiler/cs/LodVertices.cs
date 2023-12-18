using System;
using System.Text;
using System.Collections.Generic;
using JbnLib.Interfaces;
using System.IO;

namespace JbnLib.Mdl
{
    public class LodVertices : IStructure
    {
        public static int Length
        {
            get { return 2; }
        }

        private short _LodVertexCount;
        public short LodVertexCount
        {
            get { return _LodVertexCount; }
            set { _LodVertexCount = value; }
        }

        private List<LodVertex> _LodVertexCollection = new List<LodVertex>();
        public List<LodVertex> LodVertexCollection
        {
            get { return _LodVertexCollection; }
            set { _LodVertexCollection = value; }
        }

        public LodVertices(byte[] raw)
        {
            _LodVertexCount = BitConverter.ToInt16(raw, 0);
        }
        public LodVertices(short lodvertexcount)
        {
            _LodVertexCount = lodvertexcount;
        }

        public byte[] GetBytes()
        {
            byte[] output = new byte[LodVertices.Length + LodVertex.Length * _LodVertexCollection.Count];
            BitConverter.GetBytes(_LodVertexCount).CopyTo(output, 0);
            for (int i = 0; i < _LodVertexCollection.Count; i++)
                _LodVertexCollection[i].GetBytes().CopyTo(output, i * LodVertex.Length + 2);
            return output;
        }

        public override string ToString()
        {
            return _LodVertexCount.ToString();
        }
    }
}
