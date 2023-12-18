using System;
using JbnLib.Lang;

namespace JbnLib.Mdl
{
    public class EyePosition
    {
        public static int Length
        {
            get { return 12; }
        }

        private float _Y;
        public float Y
        {
            get { return _Y; }
            set { _Y = value; }
        }

        private float _X;
        public float X
        {
            get { return _X; }
            set { _X = value; }
        }

        private float _Z;
        public float Z
        {
            get { return _Z; }
            set { _Z = value; }
        }

        public EyePosition()
        {
            _Y = 0.0F;
            _X = -0.0F;
            _Z = 0.0F;
        }
        public EyePosition(byte[] raw)
        {
            _Y = BitConverter.ToSingle(raw, 0);
            _X = BitConverter.ToSingle(raw, 4);
            _Z = BitConverter.ToSingle(raw, 8);
        }
        public EyePosition(float y, float x, float z)
        {
            _Y = y;
            _X = x;
            _Z = z;
        }
        public EyePosition(float[] yxz)
        {
            if (yxz.Length != 3)
                Messages.ThrowException("Mdl.Eyeposition.New(float[])", Messages.EYEPOSITION_YXZ);
            _Y = yxz[0];
            _X = yxz[1];
            _Z = yxz[2];
        }

        public byte[] GetBytes()
        {
            byte[] output = new byte[Length];
            BitConverter.GetBytes(_Y).CopyTo(output, 0);
            BitConverter.GetBytes(_X).CopyTo(output, 4);
            BitConverter.GetBytes(_Z).CopyTo(output, 8);
            return output;
        }
        public new string ToString()
        {
            return String.Format("{0:f6} {1:f6} {2:f6}", _Y, _X, _Z);
        }
    }
}
