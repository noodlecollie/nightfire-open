using System;
using System.Collections.Generic;
using JbnLib.Interfaces;
using JbnLib.Shared;

namespace JbnLib.Mdl
{
    public class ModelInfoV14 : IStructure
    {
        public static int Length
        {
            get { return 12; }
        }

        private int _SkinReference;
        public int SkinReference
        {
            get { return _SkinReference; }
            set { _SkinReference = value; }
        }

        private CountOffsetPair _Mesh;
        public CountOffsetPair Mesh
        {
            get { return _Mesh; }
            set { _Mesh = value; }
        }

        private List<MeshV14> _MeshCollection = new List<MeshV14>();
        public List<MeshV14> MeshCollection
        {
            get { return _MeshCollection; }
            set { _MeshCollection = value; }
        }

        public ModelInfoV14(byte[] raw)
        {
            _SkinReference = BitConverter.ToInt32(raw, 0);
            _Mesh = new CountOffsetPair(BitConverter.ToInt32(raw, 4), BitConverter.ToInt32(raw, 8));
        }
        public ModelInfoV14(int skin)
        {
            _SkinReference = skin;
            _Mesh = new CountOffsetPair();
        }
        public ModelInfoV14(int skin, CountOffsetPair mesh)
        {
            _SkinReference = skin;
            _Mesh = mesh;
        }

        public byte[] GetBytes()
        {
            byte[] output = new byte[Length];
            BitConverter.GetBytes(_SkinReference).CopyTo(output, 0);
            _Mesh.GetBytes().CopyTo(output, 4);
            return output;
        }
        public new string ToString()
        {
            return _SkinReference + ", " + _MeshCollection.Count;
        }
    }
}
