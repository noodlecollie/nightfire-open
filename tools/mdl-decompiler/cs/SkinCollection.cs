using System;
using JbnLib.Interfaces;

namespace JbnLib.Mdl
{
    public class SkinCollection : IStructure
    {
        private int _Length = 0;
        public int Length
        {
            get { return _Length; }
        }

        private short[,] _Skins;
        public short[,] Skins
        {
            get { return _Skins; }
        }

        private int _Families;
        public int Families
        {
            get { return _Families; }
        }

        private int _References;
        public int References
        {
            get { return _References; }
        }

        public SkinCollection()
        {
            _Skins = new short[0, 0];
        }
        public SkinCollection(byte[] raw, int families, int references)
        {
            _Families = families;
            _References = references;
            _Skins = new short[_Families, _References];

            int i = 0;
            for (int f = 0; f < families; f++)
            {
                for (int r = 0; r < references; r++)
                {
                    _Skins[f, r] = BitConverter.ToInt16(raw, i);
                    i += 2;
                }
            }
            _Length = _Families * _References * 2;
        }
        public SkinCollection(short[,] skins, int families, int references)
        {
            _Families = families;
            _References = references;
            _Skins = skins;
            _Length = _Families * _References * 2;
        }

        public byte[] GetBytes()
        {
            byte[] output = new byte[Length];
            int i = 0;
            for (int f = 0; f < _Families; f++)
            {
                for (int r = 0; r < _References; r++)
                {
                    BitConverter.GetBytes(_Skins[f, r]).CopyTo(output, i);
                    i += 2;
                }
            }
            return output;
        }
        public new string ToString()
        {
            return _Families + ", " + _References;
        }
    }
}
