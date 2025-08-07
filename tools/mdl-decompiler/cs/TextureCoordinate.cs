using System;
using System.Text;
using JbnLib.Interfaces;

namespace JbnLib.Mdl
{
    public class TextureCoordinate : IStructure
    {
        public static int Length
        {
            get { return 8; }
        }

        private float _U;
        public float U
        {
            get { return _U; }
            set { _U = value; }
        }

        private float _V;
        public float V
        {
            get { return _V; }
            set { _V = value; }
        }

        public TextureCoordinate()
        {
        }
        public TextureCoordinate(byte[] raw)
        {
            _U = BitConverter.ToSingle(raw, 0);
            _V = BitConverter.ToSingle(raw, 4);
        }
        public TextureCoordinate(float u, float v)
        {
            _U = u;
            _V = v;
        }
        public TextureCoordinate(float ux, float uy, float uz, float vx, float vy, float vz)
        {
            _U = Convert.ToSingle(ux / Math.Sqrt(ux * ux + uy * uy + uz * uz));
            _V = Convert.ToSingle(vx / Math.Sqrt(vx * vx + vy * vy + vz * vz));
        }

        public byte[] GetBytes()
        {
            byte[] output = new byte[Length];
            BitConverter.GetBytes(_U).CopyTo(output, 0);
            BitConverter.GetBytes(_V).CopyTo(output, 4);
            return output;
        }
        public new string ToString()
        {
            return Math.Round(_U, 6) + " " + Math.Round(_V, 6);
        }
    }
}
