using System;
using JbnLib.Interfaces;
using JbnLib.Shared;

namespace JbnLib.Mdl
{
    public class HitBox : IStructure
    {
        public static int Length
        {
            get { return 32; }
        }

        private int _Bone;
        public int Bone
        {
            get { return _Bone; }
            set { _Bone = value; }
        }

        private int _Group;
        public int Group
        {
            get { return _Group; }
            set { _Group = value; }
        }

        private Point3D _Min;
        public Point3D Min
        {
            get { return _Min; }
            set { _Min = value; }
        }

        private Point3D _Max;
        public Point3D Max
        {
            get { return _Max; }
            set { _Max = value; }
        }

        public HitBox(byte[] raw)
        {
            _Bone = BitConverter.ToInt32(raw, 0);
            _Group = BitConverter.ToInt32(raw, 4);
            _Min = new Point3D(BitConverter.ToSingle(raw, 8), BitConverter.ToSingle(raw, 12), BitConverter.ToSingle(raw, 16));
            _Max = new Point3D(BitConverter.ToSingle(raw, 20), BitConverter.ToSingle(raw, 24), BitConverter.ToSingle(raw, 28));
        }
        public HitBox(int bone, int group, Point3D min, Point3D max)
        {
            _Bone = bone;
            _Group = group;
            _Min = min;
            _Max = max;
        }

        public byte[] GetBytes()
        {
            byte[] output = new byte[Length];
            BitConverter.GetBytes(_Bone).CopyTo(output, 0);
            BitConverter.GetBytes(_Group).CopyTo(output, 4);
            _Min.GetBytes().CopyTo(output, 8);
            _Max.GetBytes().CopyTo(output, 20);
            return output;
        }
        public new string ToString()
        {
            return _Bone + ", " + _Group;
        }
    }
}
