using System;
using JbnLib.Interfaces;

namespace JbnLib.Mdl
{
    public class LevelOfDetail : IStructure
    {
        public static int Length
        {
            get { return 20; }
        }

        private int _Levels;
        public int Levels
        {
            get { return _Levels; }
            set { _Levels = value; }
        }

        private int[] _Distance = new int[4];
        public int[] Distance
        {
            get { return _Distance; }
            set { _Distance = value; }
        }

        public LevelOfDetail()
        {
            _Levels = 0;
        }
        public LevelOfDetail(byte[] raw)
        {
            _Levels = BitConverter.ToInt32(raw, 0);
            for (int i = 0; i < _Distance.Length; i++)
                _Distance[i] = BitConverter.ToInt32(raw, (i * 4) + 4);
        }
        public LevelOfDetail(int levels, int[] distance)
        {
            _Levels = levels;
            _Distance = distance;
        }

        public byte[] GetBytes()
        {
            byte[] output = new byte[Length];
            BitConverter.GetBytes(_Levels).CopyTo(output, 0);
            for (int i = 0; i < _Distance.Length; i++)
                BitConverter.GetBytes(_Distance[i]).CopyTo(output, (i * 4) + 4);
            return output;
        }
        public new string ToString()
        {
            if (_Levels > 0)
            {
                string output = _Distance[0].ToString();
                for (byte i = 1; i < _Levels; i++)
                    output += " " + _Distance[i];
                return output;
            }
            else
                return "";
        }
    }
}
