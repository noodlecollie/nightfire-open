using System;
using System.Text;
using JbnLib.Interfaces;

namespace JbnLib.Mdl
{
    public class BodyGroupV14 : IStructure
    {
        public static int Length
        {
            get { return 76; }
        }

        private string _Name;
        public string Name
        {
            get { return _Name; }
            set { _Name = value; }
        }

        private int _ModelCount;
        public int ModelCount
        {
            get { return _ModelCount; }
            set { _ModelCount = value; }
        }

        private int _BodyCount;
        public int BodyCount
        {
            get { return _BodyCount; }
            set { _BodyCount = value; }
        }

        private int _ModelOffset;
        public int ModelOffset
        {
            get { return _ModelOffset; }
            set { _ModelOffset = value; }
        }

        public BodyGroupV14(byte[] raw)
        {
            _Name = Encoding.ASCII.GetString(raw, 0, 64).Trim('\0');
            _ModelCount = BitConverter.ToInt32(raw, 64);
            _BodyCount = BitConverter.ToInt32(raw, 68);
            _ModelOffset = BitConverter.ToInt32(raw, 72);
        }
        public BodyGroupV14(string name, int modelcount, int modeloffset)
        {
            _Name = name;
            _ModelCount = modelcount;
            _BodyCount = 0;
            _ModelOffset = modeloffset;
        }
        public BodyGroupV14(string name, int modelcount, int bodycount, int modeloffset)
        {
            _Name = name;
            _ModelCount = modelcount;
            _BodyCount = bodycount;
            _ModelOffset = modeloffset;
        }

        public byte[] GetBytes()
        {
            byte[] output = new byte[Length];
            Encoding.ASCII.GetBytes(_Name).CopyTo(output, 0);
            BitConverter.GetBytes(_ModelCount).CopyTo(output, 64);
            BitConverter.GetBytes(_BodyCount).CopyTo(output, 68);
            BitConverter.GetBytes(_ModelOffset).CopyTo(output, 72);
            return output;
        }
        public new string ToString()
        {
            return _Name;
        }
    }
}
