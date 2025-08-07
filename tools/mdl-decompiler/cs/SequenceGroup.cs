using System;
using System.Text;
using JbnLib.Interfaces;

namespace JbnLib.Mdl
{
    public class SequenceGroup : IStructure
    {
        public static int Length
        {
            get { return 104; }
        }

        private string _Label;
        public string Label
        {
            get { return _Label; }
            set { _Label = value; }
        }

        private string _FileName;
        public string FileName
        {
            get { return _FileName; }
            set { _FileName = value; }
        }

        private int _Cache;
        public int Cache
        {
            get { return _Cache; }
            set { _Cache = value; }
        }

        private int _Data;
        public int Data
        {
            get { return _Data; }
            set { _Data = value; }
        }

        public SequenceGroup(byte[] raw)
        {
            _Label = Encoding.ASCII.GetString(raw, 0, 32).Trim('\0');
            _FileName = Encoding.ASCII.GetString(raw, 32, 64).Trim('\0');
            _Cache = BitConverter.ToInt32(raw, 96);
            _Data = BitConverter.ToInt32(raw, 100);
        }
        public SequenceGroup(string label, string name, int cache, int data)
        {
            _Label = label;
            _FileName = name;
            _Cache = cache;
            _Data = data;
        }

        public byte[] GetBytes()
        {
            byte[] output = new byte[Length];
            Encoding.ASCII.GetBytes(_Label).CopyTo(output, 0);
            Encoding.ASCII.GetBytes(_FileName).CopyTo(output, 32);
            BitConverter.GetBytes(_Cache).CopyTo(output, 96);
            BitConverter.GetBytes(_Data).CopyTo(output, 100);
            return output;
        }
        public new string ToString()
        {
            return _Label;
        }
    }
}
