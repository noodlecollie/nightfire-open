using System;
using JbnLib.Interfaces;

namespace JbnLib.Mdl
{
    public class BlendingScales : IStructure
    {
        public static int Length
        {
            get { return 16; }
        }

        private float[] _Value = new float[4];
        public float[] Value
        {
            get { return _Value; }
            set { _Value = value; }
        }

        public BlendingScales(byte[] raw)
        {
            for (byte i = 0; i < _Value.Length; i++)
                _Value[i] = BitConverter.ToSingle(raw, i * 4);
        }
        public BlendingScales(float[] value)
        {
            _Value = value;
        }
        public BlendingScales()
        {

        }

        public byte[] GetBytes()
        {
            byte[] output = new byte[Length];
            for (byte i = 0; i < _Value.Length; i++)
                BitConverter.GetBytes(_Value[i]).CopyTo(output, i * 4);
            return output;
        }
        public new string ToString()
        {
            return _Value[0] + ", " + _Value[1] + ", " + _Value[2] + ", " + _Value[3];
        }
    }
}
