using System.Runtime.InteropServices;

namespace JbnLib.Mdl
{
    [StructLayout(LayoutKind.Explicit)]
    public struct AnimationValue
    {
        [FieldOffset(0)]public short Value;
        [FieldOffset(0)]public byte Valid;
        [FieldOffset(1)]public byte Total;

        public AnimationValue(short value)
        {
            Valid = 0;
            Total = 0;
            Value = value;
        }
        public AnimationValue(byte valid, byte total)
        {
            Value = 0;
            Valid = valid;
            Total = total;
        }
    }
}
