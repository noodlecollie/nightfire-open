using System;
using System.Text;
using System.Collections.Generic;
using JbnLib.Interfaces;

namespace JbnLib.Mdl
{
    public class SoundGroup : IStructure
    {
        public static int Length
        {
            get { return 36; }
        }

        private string _Name;
        public string Name
        {
            get { return _Name; }
            set { _Name = value; }
        }

        private int _Offset;
        public int Offset
        {
            get { return _Offset; }
            set { _Offset = value; }
        }

        private Sounds _Sounds;
        public Sounds Sounds
        {
            get { return _Sounds; }
            set { _Sounds = value; }
        }

        public SoundGroup(byte[] raw)
        {
            _Name = Encoding.ASCII.GetString(raw, 0, 32).Trim('\0');
            _Offset = BitConverter.ToInt32(raw, 32);
        }
        public SoundGroup(string name, int offset)
        {
            _Name = name;
            _Offset = offset;
        }

        public byte[] GetBytes()
        {
            byte[] output = new byte[Length];
            Encoding.ASCII.GetBytes(_Name).CopyTo(output, 0);
            BitConverter.ToInt32(output, 32);
            return output;
        }
        public new string ToString()
        {
            return _Name;
        }
    }
}
