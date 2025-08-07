using System;
using JbnLib.Interfaces;
using JbnLib.Shared;

namespace JbnLib.Mdl
{
    public class Vertex : IStructure
    {
        public static int Length
        {
            get { return 16; }
        }

        private Point3D _Position;
        public Point3D Position
        {
            get { return _Position; }
            set { _Position = value; }
        }

        private float _Scale;
        public float Scale
        {
            get { return _Scale; }
            set { _Scale = value; }
        }

        public Vertex(byte[] raw)
        {
            _Position = new Point3D(BitConverter.ToSingle(raw, 0), BitConverter.ToSingle(raw, 4), BitConverter.ToSingle(raw, 8));
            _Scale = BitConverter.ToSingle(raw, 12);
        }
        public Vertex(Point3D position, float scale)
        {
            _Position = position;
            _Scale = scale;
        }

        public byte[] GetBytes()
        {
            byte[] output = new byte[Length];
            _Position.GetBytes().CopyTo(output, 0);
            BitConverter.GetBytes(_Scale).CopyTo(output, 12);
            return output;
        }
        public new string ToString()
        {
            return _Position.ToString() + ", " + _Scale;
        }
    }
}
