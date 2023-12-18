using System;
using System.Text;
using JbnLib.Interfaces;
using JbnLib.Shared;

namespace JbnLib.Mdl
{
    [Serializable]
    public class Bone : IStructure
    {
        public static int Length
        {
            get { return 112; }
        }

        private string _Name;
        public string Name
        {
            get { return _Name; }
            set { _Name = value; }
        }

        private int _Parent;
        public int Parent
        {
            get { return _Parent; }
            set { _Parent = value; }
        }

        private int _Flags;
        public int Flags
        {
            get { return _Flags; }
            set { _Flags = value; }
        }

        private int[] _Controllers = new int[6];
        public int[] Controllers
        {
            get { return _Controllers; }
            set { _Controllers = value; }
        }

        private Point3D _Position;
        public Point3D Position
        {
            get { return _Position; }
            set { _Position = value; }
        }

        private Point3D _Rotation;
        public Point3D Rotation
        {
            get { return _Rotation; }
            set { _Rotation = value; }
        }

        private Point3D _ScalePosition;
        public Point3D ScalePosition
        {
            get { return _ScalePosition; }
            set { _ScalePosition = value; }
        }

        private Point3D _ScaleRotation;
        public Point3D ScaleRotation
        {
            get { return _ScaleRotation; }
            set { _ScaleRotation = value; }
        }

        public Bone(byte[] raw)
        {
            _Name = Encoding.ASCII.GetString(raw, 0, 32).Trim('\0');
            _Parent = BitConverter.ToInt32(raw, 32);
            _Flags = BitConverter.ToInt32(raw, 36);
            for (int i = 0; i < _Controllers.Length; i++)
                _Controllers[i] = BitConverter.ToInt32(raw, (i * 4) + 40);
            _Position = new Point3D(BitConverter.ToSingle(raw, 64), BitConverter.ToSingle(raw, 68), BitConverter.ToSingle(raw, 72));
            _Rotation = new Point3D(BitConverter.ToSingle(raw, 76), BitConverter.ToSingle(raw, 80), BitConverter.ToSingle(raw, 84));
            _ScalePosition = new Point3D(BitConverter.ToSingle(raw, 88), BitConverter.ToSingle(raw, 92), BitConverter.ToSingle(raw, 96));
            _ScaleRotation = new Point3D(BitConverter.ToSingle(raw, 100), BitConverter.ToSingle(raw, 104), BitConverter.ToSingle(raw, 108));
        }
        public Bone(string name, int parent, int flags, int[] controllers, Point3D position, Point3D rotation, Point3D scaleposition, Point3D scalerotation)
        {
            _Name = name;
            _Parent = parent;
            _Flags = flags;
            _Controllers = controllers;
            _Position = position;
            _Rotation = rotation;
            _ScalePosition = scaleposition;
            _ScaleRotation = scalerotation;
        }

        public byte[] GetBytes()
        {
            byte[] output = new byte[Length];
            Encoding.ASCII.GetBytes(_Name).CopyTo(output, 0);
            BitConverter.GetBytes(_Parent).CopyTo(output, 32);
            BitConverter.GetBytes(_Flags).CopyTo(output, 36);
            for (int i = 0; i < _Controllers.Length; i++)
                BitConverter.GetBytes(_Controllers[i]).CopyTo(output, (i * 4) + 40);
            _Position.GetBytes().CopyTo(output, 64);
            _Rotation.GetBytes().CopyTo(output, 76);
            _ScalePosition.GetBytes().CopyTo(output, 88);
            _ScaleRotation.GetBytes().CopyTo(output, 100);
            return output;
        }
        public new string ToString()
        {
            return _Name;
        }
    }
}
