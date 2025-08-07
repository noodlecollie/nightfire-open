using System;
using System.Text;
using JbnLib.Interfaces;

namespace JbnLib.Mdl
{
    [Serializable]
    public class Event : IStructure
    {
        public static int Length
        {
            get { return 76; }
        }

        private int _Frame;
        public int Frame
        {
            get { return _Frame; }
            set { _Frame = value; }
        }

        private int _Event;
        public int EventValue
        {
            get { return _Event; }
            set { _Event = value; }
        }

        private int _Type;
        public int Type
        {
            get { return _Type; }
            set { _Type = value; }
        }

        private string _Options;
        public string Options
        {
            get { return _Options; }
            set { _Options = value; }
        }

        public Event(byte[] raw)
        {
            _Frame = BitConverter.ToInt32(raw, 0);
            _Event = BitConverter.ToInt32(raw, 4);
            _Type = BitConverter.ToInt32(raw, 8);
            _Options = Encoding.ASCII.GetString(raw, 12, 64).Trim('\0');
        }
        public Event(int frame, int @event, int type, string options)
        {
            _Frame = frame;
            _Event = @event;
            _Type = type;
            _Options = options;
        }

        public byte[] GetBytes()
        {
            byte[] output = new byte[Length];
            BitConverter.GetBytes(_Frame).CopyTo(output, 0);
            BitConverter.GetBytes(_Event).CopyTo(output, 4);
            BitConverter.GetBytes(_Type).CopyTo(output, 8);
            Encoding.ASCII.GetBytes(_Options).CopyTo(output, 12);
            return output;
        }
        public new string ToString()
        {
            return _Frame + ", " + _Event;
        }
    }
}
