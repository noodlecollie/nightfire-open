using System;
using JbnLib.Interfaces;

namespace JbnLib.Mdl
{
    public class BoneFixUp : IStructure
    {
        public static int Length
        {
            get { return 48; }
        }

        private float _XScale;
        public float XScale
        {
            get { return _XScale; }
            set { _XScale = value; }
        }

        private float _XSkewY;
        public float XSkewY
        {
            get { return _XSkewY; }
            set { _XSkewY = value; }
        }

        private float _XSkewZ;
        public float XSkewZ
        {
            get { return _XSkewZ; }
            set { _XSkewZ = value; }
        }

        private float _XPosition;
        public float XPosition
        {
            get { return _XPosition; }
            set { _XPosition = value; }
        }

        private float _YSkewX;
        public float YSkewX
        {
            get { return _YSkewX; }
            set { _YSkewX = value; }
        }

        private float _YScale;
        public float YScale
        {
            get { return _YScale; }
            set { _YScale = value; }
        }

        private float _YSkewZ;
        public float YSkewZ
        {
            get { return _YSkewZ; }
            set { _YSkewZ = value; }
        }

        private float _YPosition;
        public float YPosition
        {
            get { return _YPosition; }
            set { _YPosition = value; }
        }

        private float _ZSkewX;
        public float ZSkewX
        {
            get { return _ZSkewX; }
            set { _ZSkewX = value; }
        }

        private float _ZSkewY;
        public float ZSkewY
        {
            get { return _ZSkewY; }
            set { _ZSkewY = value; }
        }

        private float _ZScale;
        public float ZScale
        {
            get { return _ZScale; }
            set { _ZScale = value; }
        }

        private float _ZPosition;
        public float ZPosition
        {
            get { return _ZPosition; }
            set { _ZPosition = value; }
        }

        public BoneFixUp(byte[] raw)
        {
            _XScale = BitConverter.ToSingle(raw, 0);
            _XSkewY = BitConverter.ToSingle(raw, 4);
            _XSkewZ = BitConverter.ToSingle(raw, 8);
            _XPosition = BitConverter.ToSingle(raw, 12);
            _YSkewX = BitConverter.ToSingle(raw, 16);
            _YScale = BitConverter.ToSingle(raw, 20);
            _YSkewZ = BitConverter.ToSingle(raw, 24);
            _YPosition = BitConverter.ToSingle(raw, 28);
            _ZSkewX = BitConverter.ToSingle(raw, 32);
            _ZSkewY = BitConverter.ToSingle(raw, 36);
            _ZScale = BitConverter.ToSingle(raw, 40);
            _ZPosition = BitConverter.ToSingle(raw, 44);
        }
        public BoneFixUp(float xscale, float xskewy, float xskewz, float xpos, float yskewx, float yscale, float yskewz, float ypos, float zskewx, float zskewy, float zscale, float zpos)
        {
            _XScale = xscale;
            _XSkewY = xskewy;
            _XSkewZ = xskewz;
            _XPosition = xpos;
            _YSkewX = yskewx;
            _YScale = yscale;
            _YSkewZ = yskewz;
            _YPosition = ypos;
            _ZSkewX = zskewx;
            _ZSkewY = zskewy;
            _ZScale = zscale;
            _ZPosition = zpos;
        }
        public BoneFixUp(float[,] table)
        {
            _XScale = table[0,0];
            _XSkewY = table[0,1];
            _XSkewZ = table[0,2];
            _XPosition = table[0,3];
            _YSkewX = table[1,0];
            _YScale = table[1,1];
            _YSkewZ = table[1,2];
            _YPosition = table[1,3];
            _ZSkewX = table[2,0];
            _ZSkewY = table[2,1];
            _ZScale = table[2,2];
            _ZPosition = table[2, 3];
        }

        public byte[] GetBytes()
        {
            byte[] output = new byte[Length];
            BitConverter.GetBytes(_XScale).CopyTo(output, 0);
            BitConverter.GetBytes(_XSkewY).CopyTo(output, 4);
            BitConverter.GetBytes(_XSkewZ).CopyTo(output, 8);
            BitConverter.GetBytes(_XPosition).CopyTo(output, 12);
            BitConverter.GetBytes(_YSkewX).CopyTo(output, 16);
            BitConverter.GetBytes(_YScale).CopyTo(output, 20);
            BitConverter.GetBytes(_YSkewZ).CopyTo(output, 24);
            BitConverter.GetBytes(_YPosition).CopyTo(output, 28);
            BitConverter.GetBytes(_ZSkewX).CopyTo(output, 32);
            BitConverter.GetBytes(_ZSkewY).CopyTo(output, 36);
            BitConverter.GetBytes(_ZScale).CopyTo(output, 40);
            BitConverter.GetBytes(_ZPosition).CopyTo(output, 44);
            return output;
        }
        public new string ToString()
        {
            return "(" + _XPosition + ", " + _YPosition + ", " + _ZPosition + ") (" + _XScale + ", " + _XSkewY + ", " + _XSkewZ + ") (" + _YSkewX + ", " + _YScale + ", " + _YSkewZ + ") (" + _ZSkewX + ", " + _ZSkewY + ", " + _ZScale + ")";
        }
    }
}
