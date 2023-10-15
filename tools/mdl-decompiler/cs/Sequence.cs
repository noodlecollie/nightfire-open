using System;
using System.Collections.Generic;
using System.Text;
using JbnLib.Interfaces;
using JbnLib.Shared;

namespace JbnLib.Mdl
{
    [Serializable]
    public class Sequence
    {
        public static int Length
        {
            get { return 188; }
        }

        internal string _Name;
        public string Name
        {
            get { return _Name; }
            set { _Name = value; }
        }

        internal float _FramesPerSecond;
        public float FramesPerSecond
        {
            get { return _FramesPerSecond; }
            set { _FramesPerSecond = value; }
        }

        internal SequenceFlags _Flags;
        public SequenceFlags Flags
        {
            get { return _Flags; }
            set { _Flags = value; }
        }

        internal int _Activity;
        public int Activity
        {
            get { return _Activity; }
            set { _Activity = value; }
        }

        internal int _ActivityWeight;
        public int ActivityWeight
        {
            get { return _ActivityWeight; }
            set { _ActivityWeight = value; }
        }

        internal CountOffsetPair _Events;
        public CountOffsetPair Events
        {
            get { return _Events; }
            set { _Events = value; }
        }

        internal Event[] _EventCollection;
        public Event[] EventCollection
        {
            get { return _EventCollection; }
            set { _EventCollection = value; }
        }

        internal int _FrameCount;
        public int FrameCount
        {
            get { return _FrameCount; }
            set { _FrameCount = value; }
        }

        internal CountOffsetPair _Pivots;
        public CountOffsetPair Pivots
        {
            get { return _Pivots; }
            set { _Pivots = value; }
        }

        internal Pivot[] _PivotCollection;
        public Pivot[] PivotCollection
        {
            get { return _PivotCollection; }
            set { _PivotCollection = value; }
        }

        internal MotionFlags _MotionType;
        public MotionFlags MotionType
        {
            get { return _MotionType; }
            set { _MotionType = value; }
        }

        internal int _MotionBone;
        public int MotionBone
        {
            get { return _MotionBone; }
            set { _MotionBone = value; }
        }

        internal Point3D _LinearMovement;
        public Point3D LinearMovement
        {
            get { return _LinearMovement; }
            set { _LinearMovement = value; }
        }

        internal int _AutomovePositionIndex;
        public int AutomovePositionIndex
        {
            get { return _AutomovePositionIndex; }
            set { _AutomovePositionIndex = value; }
        }

        internal int _AutomoveAngleIndex;
        public int AutomoveAngleIndex
        {
            get { return _AutomoveAngleIndex; }
            set { _AutomoveAngleIndex = value; }
        }

        internal Point3D _BoundingBoxMin;
        public Point3D BoundingBoxMin
        {
            get { return _BoundingBoxMin; }
            set { _BoundingBoxMin = value; }
        }

        internal Point3D _BoundingBoxMax;
        public Point3D BoundingBoxMax
        {
            get { return _BoundingBoxMax; }
            set { _BoundingBoxMax = value; }
        }

        internal int _BlendCount;
        public int BlendCount
        {
            get { return _BlendCount; }
            set { _BlendCount = value; }
        }

        internal int _AnimationOffset;
        public int AnimationOffset
        {
            get { return _AnimationOffset; }
            set { _AnimationOffset = value; }
        }

        internal Animation[,,] _AnimationCollection;
        /// <summary>
        /// Blend, Bone, Type (0-6)
        /// </summary>
        public Animation[,,] AnimationCollection
        {
            get { return _AnimationCollection; }
            set { _AnimationCollection = value; }
        }

        internal MotionFlags[] _BlendType = new MotionFlags[2];
        public MotionFlags[] BlendType
        {
            get { return _BlendType; }
            set { _BlendType = value; }
        }

        internal float[] _BlendStart = new float[2];
        public float[] BlendStart
        {
            get { return _BlendStart; }
            set { _BlendStart = value; }
        }

        internal float[] _BlendEnd = new float[2];
        public float[] BlendEnd
        {
            get { return _BlendEnd; }
            set { _BlendEnd = value; }
        }

        internal int _BlendParent;
        public int BlendParent
        {
            get { return _BlendParent; }
            set { _BlendParent = value; }
        }

        internal int _SequenceGroupIndex;
        public int SequenceGroupIndex
        {
            get { return _SequenceGroupIndex; }
            set { _SequenceGroupIndex = value; }
        }

        internal int _NodeEntry;
        public int NodeEntry
        {
            get { return _NodeEntry; }
            set { _NodeEntry = value; }
        }

        internal int _NodeExit;
        public int NodeExit
        {
            get { return _NodeExit; }
            set { _NodeExit = value; }
        }

        internal NodeFlags _NodeFlags;
        public NodeFlags NodeFlags
        {
            get { return _NodeFlags; }
            set { _NodeFlags = value; }
        }

        internal int[] _Unknown = new int[4];
        public int[] Unknown
        {
            get { return _Unknown; }
            set { _Unknown = value; }
        }
        
        public Sequence()
        {
        }
        public Sequence(byte[] raw)
        {
            _Name = Encoding.ASCII.GetString(raw, 0, 32).Trim('\0');
            _FramesPerSecond = BitConverter.ToSingle(raw, 32);
            _Flags = (SequenceFlags)BitConverter.ToInt32(raw, 36);
            _Activity = BitConverter.ToInt32(raw, 40);
            _ActivityWeight = BitConverter.ToInt32(raw, 44);
            _Events = new CountOffsetPair(BitConverter.ToInt32(raw, 48), BitConverter.ToInt32(raw, 52));
            _EventCollection = new Event[_Events.Count];
            _FrameCount = BitConverter.ToInt32(raw, 56);
            _Pivots = new CountOffsetPair(BitConverter.ToInt32(raw, 60), BitConverter.ToInt32(raw, 64));
            _PivotCollection = new Pivot[_Pivots.Count];
            _MotionType = (MotionFlags)BitConverter.ToInt32(raw, 68);
            _MotionBone = BitConverter.ToInt32(raw, 72);
            _LinearMovement = new Point3D(BitConverter.ToSingle(raw, 76), BitConverter.ToSingle(raw, 80), BitConverter.ToSingle(raw, 84));
            _AutomovePositionIndex = BitConverter.ToInt32(raw, 88);
            _AutomoveAngleIndex = BitConverter.ToInt32(raw, 92);
            _BoundingBoxMin = new Point3D(BitConverter.ToSingle(raw, 96), BitConverter.ToSingle(raw, 100), BitConverter.ToSingle(raw, 104));
            _BoundingBoxMax = new Point3D(BitConverter.ToSingle(raw, 108), BitConverter.ToSingle(raw, 112), BitConverter.ToSingle(raw, 116));
            _BlendCount = BitConverter.ToInt32(raw, 120);
            _AnimationOffset = BitConverter.ToInt32(raw, 124);
            _BlendType[0] = (MotionFlags)BitConverter.ToInt32(raw, 128);
            _BlendType[1] = (MotionFlags)BitConverter.ToInt32(raw, 132);
            _BlendStart[0] = BitConverter.ToSingle(raw, 136);
            _BlendStart[1] = BitConverter.ToSingle(raw, 140);
            _BlendEnd[0] = BitConverter.ToSingle(raw, 144);
            _BlendEnd[1] = BitConverter.ToSingle(raw, 148);
            _BlendParent = BitConverter.ToInt32(raw, 152);
            _SequenceGroupIndex = BitConverter.ToInt32(raw, 156);
            _NodeEntry = BitConverter.ToInt32(raw, 160);
            _NodeExit = BitConverter.ToInt32(raw, 164);
            _NodeFlags = (NodeFlags)BitConverter.ToInt32(raw, 168);
            _Unknown[0] = BitConverter.ToInt32(raw, 172);
            _Unknown[1] = BitConverter.ToInt32(raw, 176);
            _Unknown[2] = BitConverter.ToInt32(raw, 180);
            _Unknown[3] = BitConverter.ToInt32(raw, 184);
        }

        public byte[] GetBytes()
        {
            byte[] output = new byte[Length];
            Encoding.ASCII.GetBytes(_Name).CopyTo(output, 0);
            BitConverter.GetBytes(_FramesPerSecond).CopyTo(output, 32);
            BitConverter.GetBytes((int)_Flags).CopyTo(output, 36);
            BitConverter.GetBytes((int)_Activity).CopyTo(output, 40);
            BitConverter.GetBytes(_ActivityWeight).CopyTo(output, 44);
            _Events.GetBytes().CopyTo(output, 48);
            BitConverter.GetBytes(_FrameCount).CopyTo(output, 56);
            _Pivots.GetBytes().CopyTo(output, 60);
            BitConverter.GetBytes((int)_MotionType).CopyTo(output, 68);
            BitConverter.GetBytes(_MotionBone).CopyTo(output, 72);
            _LinearMovement.GetBytes().CopyTo(output, 76);
            BitConverter.GetBytes(_AutomovePositionIndex).CopyTo(output, 88);
            BitConverter.GetBytes(_AutomoveAngleIndex).CopyTo(output, 92);
            _BoundingBoxMin.GetBytes().CopyTo(output, 96);
            _BoundingBoxMax.GetBytes().CopyTo(output, 108);
            BitConverter.GetBytes(_BlendCount).CopyTo(output, 120);
            BitConverter.GetBytes(_AnimationOffset).CopyTo(output, 124);
            BitConverter.GetBytes((int)_BlendType[0]).CopyTo(output, 128);
            BitConverter.GetBytes((int)_BlendType[1]).CopyTo(output, 132);
            BitConverter.GetBytes(_BlendStart[0]).CopyTo(output, 136);
            BitConverter.GetBytes(_BlendStart[1]).CopyTo(output, 140);
            BitConverter.GetBytes(_BlendEnd[0]).CopyTo(output, 144);
            BitConverter.GetBytes(_BlendEnd[1]).CopyTo(output, 148);
            BitConverter.GetBytes(_BlendParent).CopyTo(output, 152);
            BitConverter.GetBytes(_SequenceGroupIndex).CopyTo(output, 156);
            BitConverter.GetBytes(_NodeEntry).CopyTo(output, 160);
            BitConverter.GetBytes(_NodeExit).CopyTo(output, 164);
            BitConverter.GetBytes((int)_NodeFlags).CopyTo(output, 168);
            BitConverter.GetBytes(_Unknown[0]).CopyTo(output, 172);
            BitConverter.GetBytes(_Unknown[1]).CopyTo(output, 176);
            BitConverter.GetBytes(_Unknown[2]).CopyTo(output, 180);
            BitConverter.GetBytes(_Unknown[3]).CopyTo(output, 184);
            return output;
        }
        public new string ToString()
        {
            return _Name;
        }
    }
}
