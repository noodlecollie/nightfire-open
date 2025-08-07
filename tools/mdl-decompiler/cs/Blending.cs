using System;
using JbnLib.Interfaces;

namespace JbnLib.Mdl
{
    public class Blending : IStructure
    {
        public static int Length
        {
            get { return 4; }
        }

        private sbyte[] _Value = new sbyte[4];
        public sbyte[] Value
        {
            get { return _Value; }
            set { _Value = value; }
        }

        public Blending(byte[] raw)
        {
            _Value[0] = unchecked((sbyte)raw[0]);
            _Value[1] = unchecked((sbyte)raw[1]);
            _Value[2] = unchecked((sbyte)raw[2]);
            _Value[3] = unchecked((sbyte)raw[3]);
        }
        public Blending(sbyte[] value)
        {
            _Value = value;
        }
        public Blending()
        {

        }

        public byte[] GetBytes()
        {
            byte[] output = new byte[Length];
            output[0] = unchecked((byte)_Value[0]);
            output[1] = unchecked((byte)_Value[1]);
            output[2] = unchecked((byte)_Value[2]);
            output[3] = unchecked((byte)_Value[3]);
            return output;
        }
        public new string ToString()
        {
            return _Value[0] + ", " + _Value[1] + ", " + _Value[2] + ", " + _Value[3];
        }
    }
}
