using System;
using System.Text;
using JbnLib.Interfaces;
using JbnLib.Shared;

namespace JbnLib.Mdl
{
    public class LodPosition : IStructure
    {
        public static int Length
        {
            get { return 20; }
        }

        private int _Bone;
        public int Bone
        {
            get { return _Bone; }
            set { _Bone = value; }
        }

        private float _Scale;
        public float Scale
        {
            get { return _Scale; }
            set { _Scale = value; }
        }

        private Point3D _Position;
        public Point3D Position
        {
            get { return _Position; }
            set { _Position = value; }
        }

        public LodPosition(byte[] raw)
        {
            _Bone = BitConverter.ToInt32(raw, 0);
            _Scale = BitConverter.ToSingle(raw, 4);
            _Position = new Point3D(BitConverter.ToSingle(raw, 8), BitConverter.ToSingle(raw, 12), BitConverter.ToSingle(raw, 16));
        }
        public LodPosition(sbyte bone, float scale, float x, float y, float z)
        {
            _Bone = bone;
            _Scale = scale;
            _Position = new Point3D(x, y, z);
        }

        public byte[] GetBytes()
        {
            byte[] output = new byte[Length];
            BitConverter.GetBytes(_Bone).CopyTo(output, 0);
            BitConverter.GetBytes(_Scale).CopyTo(output, 4);
            BitConverter.GetBytes(_Position.X).CopyTo(output, 8);
            BitConverter.GetBytes(_Position.Y).CopyTo(output, 12);
            BitConverter.GetBytes(_Position.Z).CopyTo(output, 16);
            return output;
        }
        public new string ToString()
        {
            return _Bone + ", " + _Scale + ", " + Position.ToString();
        }
    }
}
