// Decompiled with JetBrains decompiler
// Type: JbnLib.Mdl.Sequence
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Shared;
using System;
using System.Text;

namespace JbnLib.Mdl
{
  [Serializable]
  public class Sequence
  {
    internal string _Name;
    internal float _FramesPerSecond;
    internal SequenceFlags _Flags;
    internal int _Activity;
    internal int _ActivityWeight;
    internal CountOffsetPair _Events;
    internal Event[] _EventCollection;
    internal int _FrameCount;
    internal CountOffsetPair _Pivots;
    internal Pivot[] _PivotCollection;
    internal MotionFlags _MotionType;
    internal int _MotionBone;
    internal Point3D _LinearMovement;
    internal int _AutomovePositionIndex;
    internal int _AutomoveAngleIndex;
    internal Point3D _BoundingBoxMin;
    internal Point3D _BoundingBoxMax;
    internal int _BlendCount;
    internal int _AnimationOffset;
    internal Animation[,,] _AnimationCollection;
    internal MotionFlags[] _BlendType = new MotionFlags[2];
    internal float[] _BlendStart = new float[2];
    internal float[] _BlendEnd = new float[2];
    internal int _BlendParent;
    internal int _SequenceGroupIndex;
    internal int _NodeEntry;
    internal int _NodeExit;
    internal NodeFlags _NodeFlags;
    internal int[] _Unknown = new int[4];

    public static int Length => 188;

    public string Name
    {
      get => this._Name;
      set => this._Name = value;
    }

    public float FramesPerSecond
    {
      get => this._FramesPerSecond;
      set => this._FramesPerSecond = value;
    }

    public SequenceFlags Flags
    {
      get => this._Flags;
      set => this._Flags = value;
    }

    public int Activity
    {
      get => this._Activity;
      set => this._Activity = value;
    }

    public int ActivityWeight
    {
      get => this._ActivityWeight;
      set => this._ActivityWeight = value;
    }

    public CountOffsetPair Events
    {
      get => this._Events;
      set => this._Events = value;
    }

    public Event[] EventCollection
    {
      get => this._EventCollection;
      set => this._EventCollection = value;
    }

    public int FrameCount
    {
      get => this._FrameCount;
      set => this._FrameCount = value;
    }

    public CountOffsetPair Pivots
    {
      get => this._Pivots;
      set => this._Pivots = value;
    }

    public Pivot[] PivotCollection
    {
      get => this._PivotCollection;
      set => this._PivotCollection = value;
    }

    public MotionFlags MotionType
    {
      get => this._MotionType;
      set => this._MotionType = value;
    }

    public int MotionBone
    {
      get => this._MotionBone;
      set => this._MotionBone = value;
    }

    public Point3D LinearMovement
    {
      get => this._LinearMovement;
      set => this._LinearMovement = value;
    }

    public int AutomovePositionIndex
    {
      get => this._AutomovePositionIndex;
      set => this._AutomovePositionIndex = value;
    }

    public int AutomoveAngleIndex
    {
      get => this._AutomoveAngleIndex;
      set => this._AutomoveAngleIndex = value;
    }

    public Point3D BoundingBoxMin
    {
      get => this._BoundingBoxMin;
      set => this._BoundingBoxMin = value;
    }

    public Point3D BoundingBoxMax
    {
      get => this._BoundingBoxMax;
      set => this._BoundingBoxMax = value;
    }

    public int BlendCount
    {
      get => this._BlendCount;
      set => this._BlendCount = value;
    }

    public int AnimationOffset
    {
      get => this._AnimationOffset;
      set => this._AnimationOffset = value;
    }

    public Animation[,,] AnimationCollection
    {
      get => this._AnimationCollection;
      set => this._AnimationCollection = value;
    }

    public MotionFlags[] BlendType
    {
      get => this._BlendType;
      set => this._BlendType = value;
    }

    public float[] BlendStart
    {
      get => this._BlendStart;
      set => this._BlendStart = value;
    }

    public float[] BlendEnd
    {
      get => this._BlendEnd;
      set => this._BlendEnd = value;
    }

    public int BlendParent
    {
      get => this._BlendParent;
      set => this._BlendParent = value;
    }

    public int SequenceGroupIndex
    {
      get => this._SequenceGroupIndex;
      set => this._SequenceGroupIndex = value;
    }

    public int NodeEntry
    {
      get => this._NodeEntry;
      set => this._NodeEntry = value;
    }

    public int NodeExit
    {
      get => this._NodeExit;
      set => this._NodeExit = value;
    }

    public NodeFlags NodeFlags
    {
      get => this._NodeFlags;
      set => this._NodeFlags = value;
    }

    public int[] Unknown
    {
      get => this._Unknown;
      set => this._Unknown = value;
    }

    public Sequence()
    {
    }

    public Sequence(byte[] raw)
    {
      this._Name = Encoding.ASCII.GetString(raw, 0, 32).Trim(new char[1]);
      this._FramesPerSecond = BitConverter.ToSingle(raw, 32);
      this._Flags = (SequenceFlags) BitConverter.ToInt32(raw, 36);
      this._Activity = BitConverter.ToInt32(raw, 40);
      this._ActivityWeight = BitConverter.ToInt32(raw, 44);
      this._Events = new CountOffsetPair(BitConverter.ToInt32(raw, 48), BitConverter.ToInt32(raw, 52));
      this._EventCollection = new Event[this._Events.Count];
      this._FrameCount = BitConverter.ToInt32(raw, 56);
      this._Pivots = new CountOffsetPair(BitConverter.ToInt32(raw, 60), BitConverter.ToInt32(raw, 64));
      this._PivotCollection = new Pivot[this._Pivots.Count];
      this._MotionType = (MotionFlags) BitConverter.ToInt32(raw, 68);
      this._MotionBone = BitConverter.ToInt32(raw, 72);
      this._LinearMovement = new Point3D(BitConverter.ToSingle(raw, 76), BitConverter.ToSingle(raw, 80), BitConverter.ToSingle(raw, 84));
      this._AutomovePositionIndex = BitConverter.ToInt32(raw, 88);
      this._AutomoveAngleIndex = BitConverter.ToInt32(raw, 92);
      this._BoundingBoxMin = new Point3D(BitConverter.ToSingle(raw, 96), BitConverter.ToSingle(raw, 100), BitConverter.ToSingle(raw, 104));
      this._BoundingBoxMax = new Point3D(BitConverter.ToSingle(raw, 108), BitConverter.ToSingle(raw, 112), BitConverter.ToSingle(raw, 116));
      this._BlendCount = BitConverter.ToInt32(raw, 120);
      this._AnimationOffset = BitConverter.ToInt32(raw, 124);
      this._BlendType[0] = (MotionFlags) BitConverter.ToInt32(raw, 128);
      this._BlendType[1] = (MotionFlags) BitConverter.ToInt32(raw, 132);
      this._BlendStart[0] = BitConverter.ToSingle(raw, 136);
      this._BlendStart[1] = BitConverter.ToSingle(raw, 140);
      this._BlendEnd[0] = BitConverter.ToSingle(raw, 144);
      this._BlendEnd[1] = BitConverter.ToSingle(raw, 148);
      this._BlendParent = BitConverter.ToInt32(raw, 152);
      this._SequenceGroupIndex = BitConverter.ToInt32(raw, 156);
      this._NodeEntry = BitConverter.ToInt32(raw, 160);
      this._NodeExit = BitConverter.ToInt32(raw, 164);
      this._NodeFlags = (NodeFlags) BitConverter.ToInt32(raw, 168);
      this._Unknown[0] = BitConverter.ToInt32(raw, 172);
      this._Unknown[1] = BitConverter.ToInt32(raw, 176);
      this._Unknown[2] = BitConverter.ToInt32(raw, 180);
      this._Unknown[3] = BitConverter.ToInt32(raw, 184);
    }

    public byte[] GetBytes()
    {
      byte[] bytes = new byte[Sequence.Length];
      Encoding.ASCII.GetBytes(this._Name).CopyTo((Array) bytes, 0);
      BitConverter.GetBytes(this._FramesPerSecond).CopyTo((Array) bytes, 32);
      BitConverter.GetBytes((int) this._Flags).CopyTo((Array) bytes, 36);
      BitConverter.GetBytes(this._Activity).CopyTo((Array) bytes, 40);
      BitConverter.GetBytes(this._ActivityWeight).CopyTo((Array) bytes, 44);
      this._Events.GetBytes().CopyTo((Array) bytes, 48);
      BitConverter.GetBytes(this._FrameCount).CopyTo((Array) bytes, 56);
      this._Pivots.GetBytes().CopyTo((Array) bytes, 60);
      BitConverter.GetBytes((int) this._MotionType).CopyTo((Array) bytes, 68);
      BitConverter.GetBytes(this._MotionBone).CopyTo((Array) bytes, 72);
      this._LinearMovement.GetBytes().CopyTo((Array) bytes, 76);
      BitConverter.GetBytes(this._AutomovePositionIndex).CopyTo((Array) bytes, 88);
      BitConverter.GetBytes(this._AutomoveAngleIndex).CopyTo((Array) bytes, 92);
      this._BoundingBoxMin.GetBytes().CopyTo((Array) bytes, 96);
      this._BoundingBoxMax.GetBytes().CopyTo((Array) bytes, 108);
      BitConverter.GetBytes(this._BlendCount).CopyTo((Array) bytes, 120);
      BitConverter.GetBytes(this._AnimationOffset).CopyTo((Array) bytes, 124);
      BitConverter.GetBytes((int) this._BlendType[0]).CopyTo((Array) bytes, 128);
      BitConverter.GetBytes((int) this._BlendType[1]).CopyTo((Array) bytes, 132);
      BitConverter.GetBytes(this._BlendStart[0]).CopyTo((Array) bytes, 136);
      BitConverter.GetBytes(this._BlendStart[1]).CopyTo((Array) bytes, 140);
      BitConverter.GetBytes(this._BlendEnd[0]).CopyTo((Array) bytes, 144);
      BitConverter.GetBytes(this._BlendEnd[1]).CopyTo((Array) bytes, 148);
      BitConverter.GetBytes(this._BlendParent).CopyTo((Array) bytes, 152);
      BitConverter.GetBytes(this._SequenceGroupIndex).CopyTo((Array) bytes, 156);
      BitConverter.GetBytes(this._NodeEntry).CopyTo((Array) bytes, 160);
      BitConverter.GetBytes(this._NodeExit).CopyTo((Array) bytes, 164);
      BitConverter.GetBytes((int) this._NodeFlags).CopyTo((Array) bytes, 168);
      BitConverter.GetBytes(this._Unknown[0]).CopyTo((Array) bytes, 172);
      BitConverter.GetBytes(this._Unknown[1]).CopyTo((Array) bytes, 176);
      BitConverter.GetBytes(this._Unknown[2]).CopyTo((Array) bytes, 180);
      BitConverter.GetBytes(this._Unknown[3]).CopyTo((Array) bytes, 184);
      return bytes;
    }

    public new string ToString() => this._Name;
  }
}
