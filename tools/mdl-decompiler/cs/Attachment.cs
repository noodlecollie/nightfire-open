using System;
using System.Text;
using JbnLib.Interfaces;
using JbnLib.Shared;

namespace JbnLib.Mdl
{
    public class Attachment : IStructure
    {
        public static int Length
        {
            get { return 88; }
        }

        private string _Name;
        public string Name
        {
            get { return _Name; }
            set { _Name = value; }
        }

        private int _Type;
        public int Type
        {
            get { return _Type; }
            set { _Type = value; }
        }

        private int _Bone;
        public int Bone
        {
            get { return _Bone; }
            set { _Bone = value; }
        }

        private Point3D _Position;
        public Point3D Position
        {
            get { return _Position; }
            set { _Position = value; }
        }

        private Point3D _Vector1;
        public Point3D Vector1
        {
            get { return _Vector1; }
            set { _Vector1 = value; }
        }

        private Point3D _Vector2;
        public Point3D Vector2
        {
            get { return _Vector2; }
            set { _Vector2 = value; }
        }

        private Point3D _Vector3;
        public Point3D Vector3
        {
            get { return _Vector3; }
            set { _Vector3 = value; }
        }

        public Attachment(byte[] raw)
        {
            _Name = Encoding.ASCII.GetString(raw, 0, 32).Trim('\0');
            _Type = BitConverter.ToInt32(raw, 32);
            _Bone = BitConverter.ToInt32(raw, 36);
            _Position = new Point3D(BitConverter.ToSingle(raw, 40), BitConverter.ToSingle(raw, 44), BitConverter.ToSingle(raw, 48));
            _Vector1 = new Point3D(BitConverter.ToSingle(raw, 52), BitConverter.ToSingle(raw, 56), BitConverter.ToSingle(raw, 60));
            _Vector2 = new Point3D(BitConverter.ToSingle(raw, 64), BitConverter.ToSingle(raw, 68), BitConverter.ToSingle(raw, 72));
            _Vector3 = new Point3D(BitConverter.ToSingle(raw, 76), BitConverter.ToSingle(raw, 80), BitConverter.ToSingle(raw, 84));
        }
        public Attachment(string name, int type, int bone, Point3D position, Point3D vector1, Point3D vector2, Point3D vector3)
        {
            _Name = name;
            _Type = type;
            _Bone = bone;
            _Position = position;
            _Vector1 = vector1;
            _Vector2 = vector2;
            _Vector3 = vector3;
        }

        public byte[] GetBytes()
        {
            byte[] output = new byte[Length];
            Encoding.ASCII.GetBytes(_Name).CopyTo(output, 0);
            BitConverter.GetBytes(_Type).CopyTo(output, 32);
            BitConverter.GetBytes(_Bone).CopyTo(output, 36);
            _Position.GetBytes().CopyTo(output, 40);
            _Vector1.GetBytes().CopyTo(output, 52);
            _Vector2.GetBytes().CopyTo(output, 64);
            _Vector3.GetBytes().CopyTo(output, 76);
            return output;
        }
        public new string ToString()
        {
            return _Name;
        }
    }
}
