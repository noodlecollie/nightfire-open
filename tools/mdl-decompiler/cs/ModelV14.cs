using System;
using System.Collections.Generic;
using System.Text;
using JbnLib.Interfaces;

namespace JbnLib.Mdl
{
    public class ModelV14 : IStructure
    {
        public static int Length
        {
            get { return 132; }
        }

        private string _Name;
        public string Name
        {
            get { return _Name; }
            set { _Name = value; }
        }

        private int _Index;
        public int Index
        {
            get { return _Index; }
            set { _Index = value; }
        }

        private int[] _ModelInfoOffset = new int[24];
        public int[] ModelInfoOffset
        {
            get { return _ModelInfoOffset; }
            set { _ModelInfoOffset = value; }
        }

        private List<ModelInfoV14> _ModelInfoCollection = new List<ModelInfoV14>();
        public List<ModelInfoV14> ModelInfoCollection
        {
            get { return _ModelInfoCollection; }
            set { _ModelInfoCollection = value; }
        }

        public ModelV14(byte[] raw)
        {
            _Name = Encoding.ASCII.GetString(raw, 0, 32).Trim('\0');
            _Index = BitConverter.ToInt32(raw, 32);
            for (int i = 0; i < _ModelInfoOffset.Length; i++)
                _ModelInfoOffset[i] = BitConverter.ToInt32(raw, (i * 4) + 36);
        }
        public ModelV14(string name, int index)
        {
            _Name = name;
            _Index = index;
            _ModelInfoOffset = new int[24];
        }
        public ModelV14(string name, int index, int[] modelinfooffset)
        {
            _Name = name;
            _Index = index;
            _ModelInfoOffset = modelinfooffset;
        }

        public byte[] GetBytes()
        {
            byte[] output = new byte[Length];
            Encoding.ASCII.GetBytes(_Name).CopyTo(output, 0);
            BitConverter.GetBytes(_Index).CopyTo(output, 32);
            for (int i = 0; i < _ModelInfoOffset.Length; i++)
                BitConverter.GetBytes(_ModelInfoOffset[i]).CopyTo(output, (i * 4) + 36);
            return output;
        }
        public new string ToString()
        {
            return _Name;
        }
    }
}
