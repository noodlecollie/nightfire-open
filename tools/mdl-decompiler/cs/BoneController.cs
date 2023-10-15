using System;
using System.Text;
using JbnLib.Interfaces;

namespace JbnLib.Mdl
{
    public class BoneController : IStructure
    {
        public static int Length
        {
            get { return 24; }
        }

        private int _Bone;
        public int Bone
        {
            get { return _Bone; }
            set { _Bone = value; }
        }

        private MotionFlags _Type;
        public MotionFlags Type
        {
            get { return _Type; }
            set { _Type = value; }
        }

        private float _Start;
        public float Start
        {
            get { return _Start; }
            set { _Start = value; }
        }

        private float _End;
        public float End
        {
            get { return _End; }
            set { _End = value; }
        }
        
        private int _Rest;
        public int Rest
        {
            get { return _Rest; }
            set { _Rest = value; }
        }

        private int _Index;
        public int Index
        {
            get { return _Index; }
            set { _Index = value; }
        }

        public BoneController(byte[] raw)
        {
            _Bone = BitConverter.ToInt32(raw, 0);
            _Type = (MotionFlags)BitConverter.ToInt32(raw, 4);
            _Start = BitConverter.ToSingle(raw, 8);
            _End = BitConverter.ToSingle(raw, 12);
            _Rest = BitConverter.ToInt32(raw, 16);
            _Index = BitConverter.ToInt32(raw, 20);
        }
        public BoneController(int bone, MotionFlags type, float start, float end, int rest, int index)
        {
            _Bone = bone;
            _Type = type;
            _Start = start;
            _End = end;
            _Rest = rest;
            _Index = index;
        }

        public byte[] GetBytes()
        {
            byte[] output = new byte[Length];
            BitConverter.GetBytes(_Bone).CopyTo(output, 0);
            BitConverter.GetBytes((int)_Type).CopyTo(output, 4);
            BitConverter.GetBytes(_Start).CopyTo(output, 8);
            BitConverter.GetBytes(_End).CopyTo(output, 12);
            BitConverter.GetBytes(_Rest).CopyTo(output, 16);
            BitConverter.GetBytes(_Index).CopyTo(output, 20);
            return output;
        }
        public new string ToString()
        {
            return _Bone.ToString();
        }
    }
}
