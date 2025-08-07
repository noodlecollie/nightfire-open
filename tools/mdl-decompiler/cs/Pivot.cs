using System;
using JbnLib.Interfaces;
using JbnLib.Shared;

namespace JbnLib.Mdl
{
    [Serializable]
    public class Pivot : IStructure
    {
        public static int Length
        {
            get { return 20; }
        }

        private Point3D _Position;
        public Point3D Position
        {
            get { return _Position; }
            set { _Position = value; }
        }

        private int _Start;
        public int Start
        {
            get { return _Start; }
            set { _Start = value; }
        }

        private int _End;
        public int End
        {
            get { return _End; }
            set { _End = value; }
        }

        public Pivot(byte[] raw)
        {
            _Position = new Point3D(BitConverter.ToSingle(raw, 0), BitConverter.ToSingle(raw, 4), BitConverter.ToSingle(raw, 8));
            _Start = BitConverter.ToInt32(raw, 12);
            _End = BitConverter.ToInt32(raw, 16);
        }
        public Pivot(Point3D position, int start, int end)
        {
            _Position = position;
            _Start = start;
            _End = end;
        }

        public byte[] GetBytes()
        {
            byte[] output = new byte[Length];
            _Position.GetBytes().CopyTo(output, 0);
            BitConverter.GetBytes(_Start).CopyTo(output, 12);
            BitConverter.GetBytes(_End).CopyTo(output, 16);
            return output;
        }
        public new string ToString()
        {
            return _Position.ToString() + ", " + _Start + "-" + _End;
        }
    }
}
