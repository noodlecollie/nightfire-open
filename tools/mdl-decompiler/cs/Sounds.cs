using System;
using System.Collections.Generic;
using System.Text;
using JbnLib.Interfaces;

namespace JbnLib.Mdl
{
    public class Sounds : IStructure {
        public static int Length
        {
            get { return 644; }
        }

        private List<string> _Names;
        public List<string> Names
        {
            get { return _Names; }
            set { _Names = value; }
        }

        public Sounds(byte[] raw)
        {
            _Names = new List<string>();
            int o = 4;
            for (int i = 0; i < 10; i++)
            {
                string temp = Encoding.ASCII.GetString(raw, o, 64).Trim('\0');
                if (temp.Length > 0)
                    _Names.Add(temp);
                o += 64;
            }
        }
        public Sounds(List<string> names)
        {
            _Names = names;
        }

        public byte[] GetBytes()
        {
            byte[] output = new byte[Length];
            BitConverter.GetBytes(_Names.Count).CopyTo(output, 0);
            int i = 0;
            for (int o = 4; o < output.Length; o += 64)
            {
                if (i < _Names.Count)
                    Encoding.ASCII.GetBytes(_Names[i]).CopyTo(output, o);
                i++;
            }
            return output;
        }
        public new string ToString()
        {
            if (_Names.Count > 0)
            {
                string output = _Names[0];
                for (byte i = 1; i < _Names.Count; i++)
                    output += ", " + _Names[i];
                return output;
            }
            else
                return "";
        }
    }
}
