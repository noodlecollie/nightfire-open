// Decompiled with JetBrains decompiler
// Type: JbnLib.Mdl.MdlFileV14
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Lang;
using JbnLib.Qc;
using JbnLib.Shared;
using JbnLib.Smd;
using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO;
using System.Threading;

namespace JbnLib.Mdl
{
  public class MdlFileV14 : MdlFile
  {
    private HeaderV14 _Header;
    private List<BodyGroupV14> _BodyGroupCollection;
    private List<ModelV14> _ModelCollection;

    public HeaderV14 Header => this._Header;

    public List<BodyGroupV14> BodyGroupCollection
    {
      get => this._BodyGroupCollection;
      set => this._BodyGroupCollection = value;
    }

    public List<ModelV14> ModelCollection
    {
      get => this._ModelCollection;
      set => this._ModelCollection = value;
    }

    public MdlFileV14()
    {
      Thread.CurrentThread.CurrentUICulture = new CultureInfo("en");
      Thread.CurrentThread.CurrentCulture = new CultureInfo("en-US");
    }

    public void Clear()
    {
      this._Header = new HeaderV14();
      this._BoneCollection = new List<Bone>();
      this._BoneControllerCollection = new List<BoneController>();
      this._HitBoxCollection = new List<HitBox>();
      this._SequenceCollection = new List<Sequence>();
      this._SequenceGroupCollection = new List<SequenceGroup>();
      this._TextureCollection = new List<Texture>();
      this._SkinCollection = new SkinCollection();
      this._BodyGroupCollection = new List<BodyGroupV14>();
      this._AttachmentCollection = new List<Attachment>();
      this._SoundGroupCollection = new List<SoundGroup>();
      this._TriangleMapCollection = new List<TriangleMap>();
      this._VertexCollection = new List<Vertex>();
      this._NormalCollection = new List<Normal>();
      this._TextureCoordinateCollection = new List<TextureCoordinate>();
      this._BlendingScalesCollection = new List<BlendingScales>();
      this._BlendingCollection = new List<Blending>();
      this._BoneFixUpCollection = new List<BoneFixUp>();
      this._ModelCollection = new List<ModelV14>();
      this._LevelOfDetailCollection = new List<LevelOfDetail>();
    }

    public void Read(string file)
    {
      FileInfo fileInfo = new FileInfo(file);
      if (fileInfo.Exists)
      {
        this.Clear();
        FileStream input = fileInfo.OpenRead();
        BinaryReader binaryReader = new BinaryReader((Stream) input);
        this._Header = new HeaderV14(binaryReader.ReadBytes(HeaderV14.Length));
        if (this._Header.Identifier != "MDLZ")
          Messages.ThrowException("Mdl.MdlFileV14.Read(string)", Messages.IDENTIFIER_MISMATCH + Messages.EXPECTING + "MDLZ " + Messages.GOT + this._Header.Identifier);
        if (this._Header.Version != 14)
          Messages.ThrowException("Mdl.MdlFileV14.Read(string)", Messages.VERSION_MISMATCH + Messages.EXPECTING + "14 " + Messages.GOT + (object) this._Header.Version);
        input.Position = (long) this._Header.Bones.Offset;
        for (int index = 0; index < this._Header.Bones.Count; ++index)
          this._BoneCollection.Add(new Bone(binaryReader.ReadBytes(Bone.Length)));
        input.Position = (long) this._Header.BoneControllers.Offset;
        for (int index = 0; index < this._Header.BoneControllers.Count; ++index)
          this._BoneControllerCollection.Add(new BoneController(binaryReader.ReadBytes(BoneController.Length)));
        input.Position = (long) this._Header.HitBoxes.Offset;
        for (int index = 0; index < this._Header.HitBoxes.Count; ++index)
          this._HitBoxCollection.Add(new HitBox(binaryReader.ReadBytes(HitBox.Length)));
        input.Position = (long) this._Header.Sequences.Offset;
        for (int index = 0; index < this._Header.Sequences.Count; ++index)
          this._SequenceCollection.Add(new Sequence(binaryReader.ReadBytes(Sequence.Length)));
        foreach (Sequence sequence in this._SequenceCollection)
        {
          input.Position = (long) sequence.Events.Offset;
          for (int index = 0; index < sequence.Events.Count; ++index)
            sequence.EventCollection[index] = new Event(binaryReader.ReadBytes(Event.Length));
        }
        foreach (Sequence sequence in this._SequenceCollection)
        {
          input.Position = (long) sequence.Pivots.Offset;
          for (int index = 0; index < sequence.Pivots.Count; ++index)
            sequence.PivotCollection[index] = new Pivot(binaryReader.ReadBytes(Pivot.Length));
        }
        foreach (Sequence sequence in this._SequenceCollection)
        {
          input.Position = (long) sequence.AnimationOffset;
          sequence.AnimationCollection = new Animation[sequence.BlendCount, this._Header.Bones.Count, 6];
          for (int index1 = 0; index1 < sequence.BlendCount; ++index1)
          {
            for (int index2 = 0; index2 < this._Header.Bones.Count; ++index2)
            {
              long position1 = input.Position;
              for (int index3 = 0; index3 < 6; ++index3)
              {
                ushort num1 = binaryReader.ReadUInt16();
                if (num1 > (ushort) 0)
                {
                  sequence.AnimationCollection[index1, index2, index3] = new Animation();
                  long position2 = input.Position;
                  input.Position = position1 + (long) num1;
                  int num2 = 0;
                  while (num2 < sequence.FrameCount)
                  {
                    AnimationValue animationValue = new AnimationValue(binaryReader.ReadInt16());
                    num2 += (int) animationValue.Total;
                    short num3 = 0;
                    for (int index4 = 0; index4 < (int) animationValue.Valid; ++index4)
                    {
                      num3 = binaryReader.ReadInt16();
                      sequence.AnimationCollection[index1, index2, index3].Add(num3);
                    }
                    for (int valid = (int) animationValue.Valid; valid < (int) animationValue.Total; ++valid)
                      sequence.AnimationCollection[index1, index2, index3].Add(num3);
                  }
                  input.Position = position2;
                }
              }
            }
          }
          input.Position += (long) StaticMethods.Buffer(this._Header.Bones.Count * 12, 16);
        }
        input.Position = (long) this._Header.SequenceGroups.Offset;
        for (int index = 0; index < this._Header.SequenceGroups.Count; ++index)
          this._SequenceGroupCollection.Add(new SequenceGroup(binaryReader.ReadBytes(SequenceGroup.Length)));
        if (this._Header.LevelOfDetailFlags != LevelOfDetailFlags.None)
        {
          input.Position += (long) StaticMethods.Buffer(Convert.ToInt32(input.Position), 16);
          for (byte index = 0; (int) index < Enumerators.LodFlagsToInt(this._Header.LevelOfDetailFlags); ++index)
            this._LevelOfDetailCollection.Add(new LevelOfDetail(binaryReader.ReadBytes(LevelOfDetail.Length)));
        }
        input.Position = (long) this._Header.Textures.Offset;
        for (int index = 0; index < this._Header.Textures.Count; ++index)
          this._TextureCollection.Add(new Texture(binaryReader.ReadBytes(Texture.Length)));
        input.Position = (long) this._Header.SkinsOffset;
        this._SkinCollection = new SkinCollection(binaryReader.ReadBytes(this._Header.SkinFamilies * this._Header.SkinReferences * 2), this._Header.SkinFamilies, this._Header.SkinReferences);
        input.Position = (long) this._Header.BodyGroups.Offset;
        for (int index = 0; index < this._Header.BodyGroups.Count; ++index)
          this._BodyGroupCollection.Add(new BodyGroupV14(binaryReader.ReadBytes(BodyGroupV14.Length)));
        input.Position = (long) this._Header.Attachments.Offset;
        for (int index = 0; index < this._Header.Attachments.Count; ++index)
          this._AttachmentCollection.Add(new Attachment(binaryReader.ReadBytes(Attachment.Length)));
        input.Position = (long) this._Header.SoundGroups.Offset;
        for (int index = 0; index < this._Header.SoundGroups.Count; ++index)
          this._SoundGroupCollection.Add(new SoundGroup(binaryReader.ReadBytes(SoundGroup.Length)));
        long position = input.Position;
        for (int index = 0; index < this._Header.SoundGroups.Count; ++index)
        {
          input.Position = (long) this._SoundGroupCollection[index].Offset + position;
          this._SoundGroupCollection[index].Sounds = new Sounds(binaryReader.ReadBytes(Sounds.Length));
        }
        input.Position = (long) this._Header.TriangleMapOffset;
        for (int index = 0; index < this._Header.TriangleCount; ++index)
          this._TriangleMapCollection.Add(new TriangleMap(binaryReader.ReadBytes(TriangleMap.Length)));
        input.Position = (long) this._Header.VerticesOffset;
        for (int index = 0; index < this._Header.VertexCount; ++index)
          this._VertexCollection.Add(new Vertex(binaryReader.ReadBytes(Vertex.Length)));
        input.Position = (long) this._Header.NormalOffset;
        for (int index = 0; index < this._Header.VertexCount; ++index)
          this._NormalCollection.Add(new Normal(binaryReader.ReadBytes(Normal.Length)));
        input.Position = (long) this._Header.TextureCoordsOffset;
        for (int index = 0; index < this._Header.VertexCount; ++index)
          this._TextureCoordinateCollection.Add(new TextureCoordinate(binaryReader.ReadBytes(TextureCoordinate.Length)));
        input.Position = (long) this._Header.BlendingScalesOffset;
        for (int index = 0; index < this._Header.VertexCount; ++index)
          this._BlendingScalesCollection.Add(new BlendingScales(binaryReader.ReadBytes(BlendingScales.Length)));
        input.Position = (long) this._Header.BlendingOffset;
        for (int index = 0; index < this._Header.VertexCount; ++index)
          this._BlendingCollection.Add(new Blending(binaryReader.ReadBytes(Blending.Length)));
        input.Position = (long) this._Header.BoneFixUpOffset;
        for (int index = 0; index < this._Header.Bones.Count; ++index)
          this._BoneFixUpCollection.Add(new BoneFixUp(binaryReader.ReadBytes(BoneFixUp.Length)));
        for (int index = 0; index < this._Header.ModelCount; ++index)
        {
          input.Position = (long) this._Header.ModelsOffset[index];
          this._ModelCollection.Add(new ModelV14(binaryReader.ReadBytes(ModelV14.Length)));
        }
        foreach (ModelV14 model in this._ModelCollection)
        {
          foreach (int num in model.ModelInfoOffset)
          {
            if (num != 0)
            {
              input.Position = (long) num;
              model.ModelInfoCollection.Add(new ModelInfoV14(binaryReader.ReadBytes(ModelInfoV14.Length)));
            }
          }
          foreach (ModelInfoV14 modelInfo in model.ModelInfoCollection)
          {
            input.Position = (long) modelInfo.Mesh.Offset;
            for (int index = 0; index < modelInfo.Mesh.Count; ++index)
              modelInfo.MeshCollection.Add(new MeshV14(binaryReader.ReadBytes(MeshV14.Length)));
          }
        }
        binaryReader.Close();
        input.Close();
      }
      else
        Messages.ThrowException("Mdl.MdlFileV14.Read(string)", Messages.FILE_NOT_FOUND + file);
    }

    public void Write(string file)
    {
      FileStream output = new FileStream(file, FileMode.Create);
      BinaryWriter bw = new BinaryWriter((Stream) output);
      HeaderV14 headerV14 = new HeaderV14();
      bw.Write(new byte[HeaderV14.Length]);
      headerV14.Bones = new CountOffsetPair(this._BoneCollection.Count, Convert.ToInt32(output.Position));
      foreach (Bone bone in this._BoneCollection)
      {
        bw.Write(bone.GetBytes());
        bw.Flush();
      }
      StaticMethods.Buffer16(ref bw);
      headerV14.BoneControllers = new CountOffsetPair(this._BoneControllerCollection.Count, Convert.ToInt32(output.Position));
      foreach (BoneController boneController in this._BoneControllerCollection)
      {
        bw.Write(boneController.GetBytes());
        bw.Flush();
      }
      StaticMethods.Buffer16(ref bw);
      headerV14.Attachments = new CountOffsetPair(this._AttachmentCollection.Count, Convert.ToInt32(output.Position));
      foreach (Attachment attachment in this._AttachmentCollection)
      {
        bw.Write(attachment.GetBytes());
        bw.Flush();
      }
      StaticMethods.Buffer16(ref bw);
      headerV14.HitBoxes = new CountOffsetPair(this._HitBoxCollection.Count, Convert.ToInt32(output.Position));
      foreach (HitBox hitBox in this._HitBoxCollection)
      {
        bw.Write(hitBox.GetBytes());
        bw.Flush();
      }
      StaticMethods.Buffer16(ref bw);
      for (int index1 = 0; index1 < this._SequenceCollection.Count; ++index1)
      {
        this._SequenceCollection[index1].AnimationOffset = Convert.ToInt32(output.Position);
        ushort uint16 = Convert.ToUInt16(StaticMethods.Buffer(this._SequenceCollection[index1].BlendCount * this._BoneCollection.Count * 12, 16) + this._SequenceCollection[index1].BlendCount * this._BoneCollection.Count * 12);
        ushort num = uint16;
        List<short> shortList = new List<short>();
        for (int index2 = 0; index2 < this._SequenceCollection[index1].BlendCount; ++index2)
        {
          for (int index3 = 0; index3 < this._BoneCollection.Count; ++index3)
          {
            for (int index4 = 0; index4 < 6; ++index4)
            {
              if (this._SequenceCollection[index1].AnimationCollection[index2, index3, index4] == null)
              {
                bw.Write((ushort) 0);
              }
              else
              {
                bw.Write(Convert.ToUInt16((int) num + shortList.Count * 2 - ((int) num - (int) uint16)));
                shortList.AddRange((IEnumerable<short>) MdlFileV14.CompressAnimations((List<short>) this._SequenceCollection[index1].AnimationCollection[index2, index3, index4]));
              }
              bw.Flush();
            }
            uint16 -= (ushort) 12;
          }
        }
        StaticMethods.Buffer16(ref bw);
        for (int index5 = 0; index5 < shortList.Count; ++index5)
        {
          bw.Write(shortList[index5]);
          if (index5 % 2048 == 0)
            bw.Flush();
        }
        StaticMethods.Buffer16(ref bw);
      }
      headerV14.Sequences = new CountOffsetPair(this._SequenceCollection.Count, Convert.ToInt32(output.Position));
      bw.Write(new byte[this._SequenceCollection.Count * Sequence.Length]);
      bw.Flush();
      for (int index = 0; index < this._SequenceCollection.Count; ++index)
      {
        this._SequenceCollection[index].Events = new CountOffsetPair(this._SequenceCollection[index].EventCollection.Length, Convert.ToInt32(output.Position));
        foreach (Event @event in this._SequenceCollection[index].EventCollection)
          bw.Write(@event.GetBytes());
        StaticMethods.Buffer16(ref bw);
        if (this._SequenceCollection[index].PivotCollection.Length != 0)
        {
          this._SequenceCollection[index].Pivots = new CountOffsetPair(this._SequenceCollection[index].PivotCollection.Length, Convert.ToInt32(output.Position));
          foreach (Pivot pivot in this._SequenceCollection[index].PivotCollection)
            bw.Write(pivot.GetBytes());
          StaticMethods.Buffer16(ref bw);
        }
        else
        {
          StaticMethods.Buffer16(ref bw);
          this._SequenceCollection[index].Pivots = new CountOffsetPair(0, Convert.ToInt32(output.Position));
        }
      }
      long position = output.Position;
      output.Position = (long) headerV14.Sequences.Offset;
      foreach (Sequence sequence in this._SequenceCollection)
      {
        bw.Write(sequence.GetBytes());
        bw.Flush();
      }
      output.Position = position;
      headerV14.SequenceGroups = new CountOffsetPair(this._SequenceGroupCollection.Count, Convert.ToInt32(output.Position));
      foreach (SequenceGroup sequenceGroup in this._SequenceGroupCollection)
        bw.Write(sequenceGroup.GetBytes());
      StaticMethods.Buffer16(ref bw);
      foreach (LevelOfDetail levelOfDetail in this._LevelOfDetailCollection)
        bw.Write(levelOfDetail.GetBytes());
      StaticMethods.Buffer16(ref bw);
      headerV14.BodyGroups = new CountOffsetPair(this._BodyGroupCollection.Count, Convert.ToInt32(output.Position));
      bw.Write(new byte[this._BodyGroupCollection.Count * BodyGroupV14.Length]);
      StaticMethods.Buffer16(ref bw);
      headerV14.Textures = new CountOffsetPair(this._TextureCollection.Count, Convert.ToInt32(output.Position));
      foreach (Texture texture in this._TextureCollection)
        bw.Write(texture.GetBytes());
      StaticMethods.Buffer16(ref bw);
      headerV14.SkinsOffset = Convert.ToInt32(output.Position);
      bw.Write(this._SkinCollection.GetBytes());
      StaticMethods.Buffer16(ref bw);
      headerV14.TextureInfo = Convert.ToInt32(output.Position);
      headerV14.SoundGroups = new CountOffsetPair(this._SoundGroupCollection.Count, Convert.ToInt32(output.Position));
      for (int index = 0; index < this._SoundGroupCollection.Count; ++index)
      {
        this._SoundGroupCollection[index].Offset = index * Sounds.Length;
        bw.Write(this._SoundGroupCollection[index].GetBytes());
      }
      bw.Flush();
      headerV14.SoundsOffset = Convert.ToInt32(output.Position);
      for (int index = 0; index < this._SoundGroupCollection.Count; ++index)
      {
        bw.Write(this._SoundGroupCollection[index].Sounds.GetBytes());
        bw.Flush();
      }
      StaticMethods.Buffer16(ref bw);
      headerV14.VerticesOffset = Convert.ToInt32(output.Position);
      int num1 = 0;
      foreach (Vertex vertex in this._VertexCollection)
      {
        bw.Write(vertex.GetBytes());
        ++num1;
        if (num1 % 256 == 0)
          bw.Flush();
      }
      bw.Flush();
      headerV14.BlendingOffset = Convert.ToInt32(output.Position);
      int num2 = 0;
      foreach (Blending blending in this._BlendingCollection)
      {
        bw.Write(blending.GetBytes());
        ++num2;
        if (num2 % 1024 == 0)
          bw.Flush();
      }
      StaticMethods.Buffer16(ref bw);
      headerV14.BlendingScalesOffset = Convert.ToInt32(output.Position);
      int num3 = 0;
      foreach (BlendingScales blendingScales in this._BlendingScalesCollection)
      {
        bw.Write(blendingScales.GetBytes());
        ++num3;
        if (num3 % 256 == 0)
          bw.Flush();
      }
      bw.Flush();
      headerV14.NormalOffset = Convert.ToInt32(output.Position);
      int num4 = 0;
      foreach (Normal normal in this._NormalCollection)
      {
        bw.Write(normal.GetBytes());
        ++num4;
        if (num4 % 256 == 0)
          bw.Flush();
      }
      bw.Flush();
      headerV14.TextureCoordsOffset = Convert.ToInt32(output.Position);
      int num5 = 0;
      foreach (TextureCoordinate textureCoordinate in this._TextureCoordinateCollection)
      {
        bw.Write(textureCoordinate.GetBytes());
        ++num5;
        if (num5 % 512 == 0)
          bw.Flush();
      }
      StaticMethods.Buffer16(ref bw);
      headerV14.TriangleMapOffset = Convert.ToInt32(output.Position);
      int num6 = 0;
      foreach (TriangleMap triangleMap in this._TriangleMapCollection)
      {
        bw.Write(triangleMap.GetBytes());
        ++num6;
        if (num6 % 2048 == 0)
          bw.Flush();
      }
      StaticMethods.Buffer16(ref bw);
      headerV14.BoneFixUpOffset = Convert.ToInt32(output.Position);
      int num7 = 0;
      foreach (BoneFixUp boneFixUp in this._BoneFixUpCollection)
      {
        bw.Write(boneFixUp.GetBytes());
        ++num7;
        if (num7 % 96 == 0)
          bw.Flush();
      }
      bw.Flush();
      foreach (ModelV14 model in this._ModelCollection)
      {
        foreach (ModelInfoV14 modelInfo in model.ModelInfoCollection)
        {
          modelInfo.Mesh = new CountOffsetPair(modelInfo.MeshCollection.Count, Convert.ToInt32(output.Position));
          foreach (MeshV14 mesh in modelInfo.MeshCollection)
          {
            ushort num8 = ushort.MaxValue;
            ushort num9 = 0;
            for (int triangleIndex = (int) mesh.TriangleIndex; triangleIndex < (int) mesh.TriangleIndex + (int) mesh.TriangleCount; ++triangleIndex)
            {
              if ((int) this._TriangleMapCollection[triangleIndex].VertexIndex < (int) num8)
                num8 = this._TriangleMapCollection[triangleIndex].VertexIndex;
              if ((int) this._TriangleMapCollection[triangleIndex].VertexIndex > (int) num9)
                num9 = this._TriangleMapCollection[triangleIndex].VertexIndex;
            }
            mesh.VertexIndexFrom = num8;
            mesh.VertexIndexTo = num9;
            bw.Write(mesh.GetBytes());
          }
          bw.Flush();
        }
      }
      foreach (ModelV14 model in this._ModelCollection)
      {
        foreach (ModelInfoV14 modelInfo in model.ModelInfoCollection)
        {
          model.ModelInfoOffset[modelInfo.SkinReference] = Convert.ToInt32(output.Position);
          bw.Write(modelInfo.GetBytes());
        }
        bw.Flush();
      }
      StaticMethods.Buffer16(ref bw);
      for (int index = 0; index < this._ModelCollection.Count; ++index)
      {
        headerV14.ModelsOffset[index] = Convert.ToInt32(output.Position);
        bw.Write(this._ModelCollection[index].GetBytes());
        bw.Flush();
      }
      StaticMethods.Buffer16(ref bw);
      headerV14.LevelOfDetailFlags = this._Header._LevelOfDetailFlags;
      headerV14.BoundingBoxMax = this._Header.BoundingBoxMax;
      headerV14.BoundingBoxMin = this._Header.BoundingBoxMin;
      headerV14.ClippingBoxMax = this._Header.ClippingBoxMax;
      headerV14.ClippingBoxMin = this._Header.ClippingBoxMin;
      headerV14.EyePosition = this._Header.EyePosition;
      headerV14.FileLength = Convert.ToInt32(output.Position);
      headerV14.Identifier = "MDLZ";
      headerV14.ModelCount = this._ModelCollection.Count;
      headerV14.Name = this._Header.Name;
      headerV14.SkinFamilies = this._SkinCollection.Families;
      headerV14.SkinReferences = this._SkinCollection.References;
      headerV14.TransitionFlagsOffset = this._Header.TransitionFlagsOffset;
      headerV14.TransitionsCount = this._Header.TransitionsCount;
      headerV14.TransitionsOffset = this._Header.TransitionsOffset;
      headerV14.TriangleCount = this._TriangleMapCollection.Count;
      headerV14.TypeFlag = this._Header.TypeFlag;
      headerV14.UselessOffset = this._Header.UselessOffset;
      headerV14.Version = 14;
      headerV14.VertexCount = this._VertexCollection.Count;
      output.Position = 0L;
      bw.Write(headerV14.GetBytes());
      int index6 = 0;
      output.Position = (long) headerV14.BodyGroups.Offset;
      foreach (BodyGroupV14 bodyGroup in this._BodyGroupCollection)
      {
        bodyGroup.ModelOffset = headerV14.ModelsOffset[index6];
        bw.Write(bodyGroup.GetBytes());
        index6 += bodyGroup.ModelCount;
      }
      bw.Flush();
      bw.Close();
      output.Close();
    }

    public void ReadQce(string file)
    {
      Qce qce = new Qce();
      qce.Read(file);
      if (qce.QceVersion.Game != QceGame.JamesBond007Nightfire || qce.QceVersion.QcVersion != 11 || qce.QceVersion.QceVersionMajor != 1)
        Messages.ThrowException("Mdl.MdlFileV14.ReadQce(string)", Messages.QCE_VERSION_MISMATCH);
      foreach (CommandReplaceActivity replaceActivity in qce.ReplaceActivityCollection)
        this._SequenceCollection[(int) this.FindSequence(replaceActivity.SequenceName, out byte _)].Activity = (int) Enum.Parse(typeof (ActivityV14), replaceActivity.Activity);
      if (qce.LodFlags.Value == 0)
        return;
      this._Header.LevelOfDetailFlags = (LevelOfDetailFlags) qce.LodFlags.Value;
      this._LevelOfDetailCollection = new List<LevelOfDetail>();
      for (int index = 0; index < Enumerators.LodFlagsToInt(this._Header.LevelOfDetailFlags); ++index)
        this._LevelOfDetailCollection.Add(new LevelOfDetail());
      for (int index = 0; index < qce.LodTempCollection.Count; ++index)
      {
        int[] numArray = new int[4];
        qce.LodTempCollection[index].Distances.CopyTo(numArray);
        this._LevelOfDetailCollection[qce.LodTempCollection[index].Index] = new LevelOfDetail(qce.LodTempCollection[index].Distances.Count, numArray);
      }
    }

    public static List<short> CompressAnimations(List<short> frames)
    {
      List<short> shortList = new List<short>();
      byte num1;
      for (int index1 = 0; index1 < frames.Count; index1 += (int) num1)
      {
        num1 = frames.Count - index1 <= (int) byte.MaxValue ? Convert.ToByte(frames.Count - index1) : byte.MaxValue;
        List<ValueCountPair> valueCountPairList = new List<ValueCountPair>();
        valueCountPairList.Add(new ValueCountPair(frames[index1]));
        int index2 = 0;
        for (int index3 = 1; index3 < (int) num1; ++index3)
        {
          if ((int) valueCountPairList[index2].Value == (int) frames[index3 + index1])
          {
            ++valueCountPairList[index2].Count;
          }
          else
          {
            valueCountPairList.Add(new ValueCountPair(frames[index3 + index1]));
            ++index2;
          }
        }
        shortList.Add((short) 0);
        int index4 = shortList.Count - 1;
        byte num2 = 0;
        byte num3 = 0;
        for (int index5 = 0; index5 < valueCountPairList.Count; ++index5)
        {
          if (valueCountPairList[index5].Count > (byte) 2 || (int) valueCountPairList[index5].Count + (int) num2 == (int) num1)
          {
            ++num2;
            shortList.Add(valueCountPairList[index5].Value);
            shortList[index4] = new AnimationValue(num2, Convert.ToByte((int) num2 + (int) valueCountPairList[index5].Count - 1)).Value;
            if ((int) valueCountPairList[index5].Count - 1 + (int) num2 < (int) num1)
            {
              num3 += Convert.ToByte((int) valueCountPairList[index5].Count + (int) num2 - 1);
              num2 = (byte) 0;
              if ((int) num3 != (int) num1)
              {
                shortList.Add((short) 0);
                index4 = shortList.Count - 1;
              }
            }
            else
              num3 = (byte) 0;
          }
          else
          {
            for (int index6 = 0; index6 < (int) valueCountPairList[index5].Count; ++index6)
            {
              if ((int) valueCountPairList[index5].Count - 1 + (int) num3 + (int) num2 != (int) num1)
              {
                ++num2;
                shortList.Add(valueCountPairList[index5].Value);
              }
              else
                shortList[index4] = new AnimationValue(num2, Convert.ToByte((int) num2 + (int) valueCountPairList[index5].Count - 1)).Value;
            }
            if ((int) num3 + (int) num2 == (int) num1)
              shortList[index4] = new AnimationValue(num2, num2).Value;
          }
        }
      }
      return shortList;
    }

    public string[] DumpV10Qc(string file) => this.DumpV10Qc(file, this._Header.Name);

    public string[] DumpV10Qc(string file, string modelname)
    {
      List<string> stringList1 = new List<string>();
      FileInfo fileInfo = new FileInfo(file);
      Directory.CreateDirectory(fileInfo.DirectoryName);
      string str1 = this._Header.Name.Replace(".mdl", "");
      QcFileV10 qcFileV10 = new QcFileV10();
      Qce qce = new Qce();
      qce.QceVersion = new CommandQceVersion(QceGame.HalfLife, 10, 1, 0);
      for (int index = 0; index < this._AttachmentCollection.Count; ++index)
        qcFileV10.AttachmentCollection.Add(new CommandAttachment(this._AttachmentCollection[index].Name, this._BoneCollection[this._AttachmentCollection[index].Bone].Name, this._AttachmentCollection[index].Position));
      qcFileV10.BBox = new CommandBBox(this._Header.BoundingBoxMin, this._Header.BoundingBoxMax);
      foreach (BodyGroupV14 bodyGroup in this._BodyGroupCollection)
      {
        CommandBodyGroup commandBodyGroup = new CommandBodyGroup();
        commandBodyGroup.Name = bodyGroup.Name;
        for (int index = 0; index < bodyGroup.ModelCount; ++index)
        {
          byte model = this.FindModel(bodyGroup.ModelOffset + index * ModelV14.Length);
          if (this._ModelCollection[(int) model].Name == "blank")
          {
            commandBodyGroup.BodyCollection.Add(new BodyGroupItem("blank"));
          }
          else
          {
            string[] textures;
            commandBodyGroup.BodyCollection.Add(new BodyGroupItem("studio", this.DumpV10SmdReference(fileInfo.DirectoryName, this._ModelCollection[(int) model].Name, out textures)));
            foreach (string str2 in textures)
              stringList1.Add(str2);
          }
        }
        qcFileV10.BodyGroupCollection.Add(commandBodyGroup);
      }
      qcFileV10.CBox = new CommandCBox(this._Header.ClippingBoxMin, this._Header.ClippingBoxMax);
      qcFileV10.Cd = new CommandCd(fileInfo.DirectoryName);
      foreach (BoneController boneController in this._BoneControllerCollection)
        qcFileV10.ControllerCollection.Add(new CommandController(boneController.Index, this._BoneCollection[boneController.Bone].Name, boneController.Type, boneController.Start, boneController.End));
      qcFileV10.EyePosition = new CommandEyePosition(this._Header.EyePosition);
      qcFileV10.Flags = new CommandFlags(this._Header.TypeFlag);
      foreach (HitBox hitBox in this._HitBoxCollection)
        qcFileV10.HBoxCollection.Add(new CommandHBox(hitBox.Group, this._BoneCollection[hitBox.Bone].Name, hitBox.Min, hitBox.Max));
      qcFileV10.ModelName = new CommandModelName(modelname);
      qcFileV10.Origin = new CommandOrigin(new Point3D());
      foreach (Bone bone in this._BoneCollection)
      {
        if (bone.Parent == -1)
        {
          qcFileV10.Root = new CommandRoot(bone.Name);
          break;
        }
      }
      if (this._BoneCollection.Count > 0)
      {
        foreach (Sequence sequence in this._SequenceCollection)
        {
          CommandSequenceV10 commandSequenceV10 = new CommandSequenceV10();
          ActivityV14 activity = (ActivityV14) sequence.Activity;
          if (activity != ActivityV14.ACT_RESET)
          {
            commandSequenceV10.Activity = new OptionActivityV10((ActivityV10) Enumerators.ConvertActivity(typeof (ActivityV14), typeof (ActivityV10), (object) activity), (float) sequence.ActivityWeight);
            if (!commandSequenceV10.Activity.Activity.ToString().StartsWith("ACT_"))
            {
              qce.ReplaceActivityCollection.Add(new CommandReplaceActivity(sequence.Name, activity.ToString()));
              commandSequenceV10.Activity.Activity = ActivityV10.ACT_INVALID;
            }
          }
          for (int index = 0; index < sequence.BlendCount; ++index)
          {
            if (sequence.BlendType[index] != MotionFlags.None)
              commandSequenceV10.BlendCollection.Add(new OptionBlend(sequence.BlendType[index], sequence.BlendStart[index], sequence.BlendEnd[index]));
          }
          commandSequenceV10.Control.Flags = sequence.MotionType;
          foreach (Event @event in sequence.EventCollection)
            commandSequenceV10.EventCollection.Add(new OptionEvent(@event.EventValue, @event.Frame, @event.Options));
          commandSequenceV10.FileCollection.Add(this.DumpV10SmdAnimation(fileInfo.DirectoryName, sequence.Name));
          for (int index = 1; index < sequence.BlendCount; ++index)
            commandSequenceV10.FileCollection.Add(this.DumpV10SmdAnimation(fileInfo.DirectoryName, sequence.Name + "_blend" + (object) index));
          if ((double) sequence.FramesPerSecond != 30.0)
            commandSequenceV10.Fps = new OptionFps(sequence.FramesPerSecond);
          if (sequence.Flags == SequenceFlags.LOOPING)
            commandSequenceV10.Loop = new OptionLoop(true);
          commandSequenceV10.Name = sequence.Name;
          if (sequence.NodeEntry != 0)
            commandSequenceV10.Node = new OptionNodeV10(Convert.ToSByte(sequence.NodeEntry));
          for (int index = 0; index < sequence.PivotCollection.Length; ++index)
            commandSequenceV10.PivotCollection.Add(new OptionPivot(index, (float) sequence.PivotCollection[index].Start, (float) sequence.PivotCollection[index].End));
          if (sequence.NodeExit != 0)
          {
            if (sequence.NodeFlags == NodeFlags.REVERSE)
              commandSequenceV10.RTransition = new OptionRTransition(sequence.NodeEntry, sequence.NodeExit);
            else
              commandSequenceV10.Transition = new OptionTransition(sequence.NodeEntry, sequence.NodeExit);
          }
          qcFileV10.SequenceCollection.Add(commandSequenceV10);
        }
      }
      if (this._SkinCollection.Families > 1)
      {
        CommandTextureGroup commandTextureGroup = new CommandTextureGroup();
        commandTextureGroup.Name = "Skins";
        for (int index1 = 0; index1 < this._SkinCollection.Families; ++index1)
        {
          List<string> stringList2 = new List<string>();
          for (int index2 = 0; index2 < this._SkinCollection.References; ++index2)
            stringList2.Add(this._TextureCollection[(int) this._SkinCollection.Skins[index1, index2]].TextureName);
          commandTextureGroup.SkinCollection.Add(stringList2);
        }
      }
      qcFileV10.Write(file);
      qce.Write(fileInfo.DirectoryName + "\\" + str1 + ".qce");
      return StaticMethods.EliminateDuplicates(stringList1.ToArray());
    }

    public string DumpV10SmdReference(string path, string model, out string[] textures)
    {
      SmdFileV10 smdFileV10 = new SmdFileV10(SmdType.Reference);
      List<Time> bones = new List<Time>();
      Matrix[] matrixArray = new Matrix[this._BoneCollection.Count];
      for (int index = 0; index < this._BoneCollection.Count; ++index)
      {
        smdFileV10.NodeCollection.Add(new JbnLib.Smd.Node(Convert.ToSByte(index), this._BoneCollection[index].Name, Convert.ToSByte(this._BoneCollection[index].Parent)));
        bones.Add(new Time(index, this._BoneCollection[index].Position, this._BoneCollection[index].Rotation));
        if (index <= this._BoneCollection[index].Parent || this._BoneCollection[index].Parent < -1)
          Messages.ThrowException("Mdl.MdlFileV14.DumpV44SmdReference(string, string, out string[])", "Bone's parent's index is more-than-or-equal-to bone's index (bone name:" + this._BoneCollection[index].Name + ").");
        Matrix b = new Matrix(new Matrix(new Quaternion(this._BoneCollection[index].Rotation)), 3, 4);
        b[0, 3] = this._BoneCollection[index].Position.dX;
        b[1, 3] = this._BoneCollection[index].Position.dY;
        b[2, 3] = this._BoneCollection[index].Position.dZ;
        matrixArray[index] = this._BoneCollection[index].Parent != -1 ? StaticMethods.Multiply3x4(matrixArray[this._BoneCollection[index].Parent], b) : b;
      }
      for (int index = 0; index < this._BoneCollection.Count; ++index)
        matrixArray[index] = StaticMethods.Multiply3x4(matrixArray[index], this._BoneFixUpCollection[index].ToMatrix());
      smdFileV10.TimeCollection.Add(new Frame(0, bones));
      foreach (ModelInfoV14 modelInfo in this._ModelCollection[(int) this.FindModel(model)].ModelInfoCollection)
      {
        foreach (MeshV14 mesh in modelInfo.MeshCollection)
        {
          for (int index1 = 0; index1 < (int) mesh.TriangleCount; index1 += 3)
          {
            List<VertexV10> vertices = new List<VertexV10>();
            ushort vertexIndex1 = this._TriangleMapCollection[(int) mesh.TriangleIndex + index1 + 1].VertexIndex;
            BlendV11[] blending1 = this.BlendsToBlending(mesh.BoneMappings, (int) vertexIndex1);
            Point3D pos1 = new Point3D();
            for (int index2 = 0; index2 < blending1.Length; ++index2)
            {
              if (blending1[index2].Bone != (sbyte) -1)
                pos1 += (double) blending1[index2].Scale * StaticMethods.Transform(this._VertexCollection[(int) vertexIndex1].Position, matrixArray[(int) blending1[index2].Bone]);
            }
            vertices.Add(new VertexV10(this.BlendsToBones(mesh.BoneMappings, (int) vertexIndex1)[0], pos1, this._NormalCollection[(int) vertexIndex1].Position, new TextureCoordinate(this._TextureCoordinateCollection[(int) vertexIndex1].U, this._TextureCoordinateCollection[(int) vertexIndex1].V * -1f)));
            ushort vertexIndex2 = this._TriangleMapCollection[(int) mesh.TriangleIndex + index1].VertexIndex;
            BlendV11[] blending2 = this.BlendsToBlending(mesh.BoneMappings, (int) vertexIndex2);
            Point3D pos2 = new Point3D();
            for (int index3 = 0; index3 < blending2.Length; ++index3)
            {
              if (blending2[index3].Bone != (sbyte) -1)
                pos2 += (double) blending2[index3].Scale * StaticMethods.Transform(this._VertexCollection[(int) vertexIndex2].Position, matrixArray[(int) blending2[index3].Bone]);
            }
            vertices.Add(new VertexV10(this.BlendsToBones(mesh.BoneMappings, (int) vertexIndex2)[0], pos2, this._NormalCollection[(int) vertexIndex2].Position, new TextureCoordinate(this._TextureCoordinateCollection[(int) vertexIndex2].U, this._TextureCoordinateCollection[(int) vertexIndex2].V * -1f)));
            ushort vertexIndex3 = this._TriangleMapCollection[(int) mesh.TriangleIndex + index1 + 2].VertexIndex;
            BlendV11[] blending3 = this.BlendsToBlending(mesh.BoneMappings, (int) vertexIndex3);
            Point3D pos3 = new Point3D();
            for (int index4 = 0; index4 < blending3.Length; ++index4)
            {
              if (blending3[index4].Bone != (sbyte) -1)
                pos3 += (double) blending3[index4].Scale * StaticMethods.Transform(this._VertexCollection[(int) vertexIndex3].Position, matrixArray[(int) blending3[index4].Bone]);
            }
            vertices.Add(new VertexV10(this.BlendsToBones(mesh.BoneMappings, (int) vertexIndex3)[0], pos3, this._NormalCollection[(int) vertexIndex3].Position, new TextureCoordinate(this._TextureCoordinateCollection[(int) vertexIndex3].U, this._TextureCoordinateCollection[(int) vertexIndex3].V * -1f)));
            smdFileV10.TriangleCollection.Add(new Triangle<VertexV10>(this._TextureCollection[modelInfo.SkinReference].TextureName, vertices));
          }
        }
      }
      textures = smdFileV10.GetTextures();
      smdFileV10.Write(path.TrimEnd('\\') + " \\" + StaticMethods.GoodFileName(model) + "_ref.smd");
      return StaticMethods.GoodFileName(model) + "_ref";
    }

    public string DumpV10SmdAnimation(string path, string sequence)
    {
      SmdFileV10 smdFileV10 = new SmdFileV10(SmdType.Animation);
      List<Time> bones1 = new List<Time>();
      for (int index = 0; index < this._BoneCollection.Count; ++index)
      {
        smdFileV10.NodeCollection.Add(new JbnLib.Smd.Node(Convert.ToSByte(index), this._BoneCollection[index].Name, Convert.ToSByte(this._BoneCollection[index].Parent)));
        bones1.Add(new Time(index, this._BoneCollection[index].Position, this._BoneCollection[index].Rotation));
      }
      byte blend;
      int sequence1 = (int) this.FindSequence(sequence, out blend);
      for (int index1 = 0; index1 < this._SequenceCollection[sequence1].FrameCount; ++index1)
      {
        List<Time> bones2 = new List<Time>();
        for (int index2 = 0; index2 < this._Header.Bones.Count; ++index2)
        {
          Point3D pos = new Point3D(this._BoneCollection[index2].Position);
          Point3D rot = new Point3D(this._BoneCollection[index2].Rotation);
          for (int index3 = 0; index3 < 6; ++index3)
          {
            if (this._SequenceCollection[sequence1].AnimationCollection[(int) blend, index2, index3] != null)
            {
              switch (index3)
              {
                case 0:
                  pos.X += (float) this._SequenceCollection[sequence1].AnimationCollection[(int) blend, index2, index3][index1] * this._BoneCollection[index2].ScalePosition.X;
                  continue;
                case 1:
                  pos.Y += (float) this._SequenceCollection[sequence1].AnimationCollection[(int) blend, index2, index3][index1] * this._BoneCollection[index2].ScalePosition.Y;
                  continue;
                case 2:
                  pos.Z += (float) this._SequenceCollection[sequence1].AnimationCollection[(int) blend, index2, index3][index1] * this._BoneCollection[index2].ScalePosition.Z;
                  continue;
                case 3:
                  rot.X += (float) this._SequenceCollection[sequence1].AnimationCollection[(int) blend, index2, index3][index1] * this._BoneCollection[index2].ScaleRotation.X;
                  continue;
                case 4:
                  rot.Y += (float) this._SequenceCollection[sequence1].AnimationCollection[(int) blend, index2, index3][index1] * this._BoneCollection[index2].ScaleRotation.Y;
                  continue;
                case 5:
                  rot.Z += (float) this._SequenceCollection[sequence1].AnimationCollection[(int) blend, index2, index3][index1] * this._BoneCollection[index2].ScaleRotation.Z;
                  continue;
                default:
                  continue;
              }
            }
          }
          if (this._BoneCollection[index2].Parent == -1 && this._SequenceCollection[sequence1].FrameCount > 1)
          {
            pos.X += (float) index1 * this._SequenceCollection[sequence1].LinearMovement[0] / (float) (this._SequenceCollection[sequence1].FrameCount - 1);
            pos.Y += (float) index1 * this._SequenceCollection[sequence1].LinearMovement[1] / (float) (this._SequenceCollection[sequence1].FrameCount - 1);
            pos.Z += (float) index1 * this._SequenceCollection[sequence1].LinearMovement[2] / (float) (this._SequenceCollection[sequence1].FrameCount - 1);
          }
          bones2.Add(new Time(index2, pos, rot));
        }
        smdFileV10.TimeCollection.Add(new Frame(index1, bones2));
      }
      if (smdFileV10.TimeCollection.Count == 0)
        smdFileV10.TimeCollection.Add(new Frame(0, bones1));
      smdFileV10.Rotate(-90.0);
      smdFileV10.Write(path.TrimEnd('\\') + " \\" + StaticMethods.GoodFileName(sequence) + "_ani.smd");
      return StaticMethods.GoodFileName(sequence) + "_ani";
    }

    public string[] DumpV11Qc(string file) => this.DumpV11Qc(file, this._Header.Name);

    public string[] DumpV11Qc(string file, string modelname)
    {
      List<string> stringList1 = new List<string>();
      FileInfo fileInfo = new FileInfo(file);
      Directory.CreateDirectory(fileInfo.DirectoryName);
      string str1 = this._Header.Name.Replace(".mdl", "");
      QcFileV11 qcFileV11 = new QcFileV11();
      Qce qce = new Qce();
      qce.QceVersion = new CommandQceVersion(QceGame.JamesBond007Nightfire, 11, 1, 1);
      for (int index = 0; index < this._AttachmentCollection.Count; ++index)
        qcFileV11.AttachmentCollection.Add(new CommandAttachment(this._AttachmentCollection[index].Name, this._BoneCollection[this._AttachmentCollection[index].Bone].Name, this._AttachmentCollection[index].Position));
      qcFileV11.BBox = new CommandBBox(this._Header.BoundingBoxMin, this._Header.BoundingBoxMax);
      if (this._BodyGroupCollection.Count == 0)
        this._BodyGroupCollection.Add(new BodyGroupV14("studio", 1, this._Header.ModelsOffset[0]));
      foreach (BodyGroupV14 bodyGroup in this._BodyGroupCollection)
      {
        CommandBodyGroup commandBodyGroup = new CommandBodyGroup();
        commandBodyGroup.Name = bodyGroup.Name;
        for (int index = 0; index < bodyGroup.ModelCount; ++index)
        {
          byte model = this.FindModel(bodyGroup.ModelOffset + index * ModelV14.Length);
          if (this._ModelCollection[(int) model].Name == "blank")
          {
            commandBodyGroup.BodyCollection.Add(new BodyGroupItem("blank"));
          }
          else
          {
            string[] textures;
            commandBodyGroup.BodyCollection.Add(new BodyGroupItem("studio", this.DumpV11SmdReference(fileInfo.DirectoryName, this._ModelCollection[(int) model].Name, out textures)));
            foreach (string str2 in textures)
              stringList1.Add(str2);
          }
        }
        qcFileV11.BodyGroupCollection.Add(commandBodyGroup);
      }
      qcFileV11.CBox = new CommandCBox(this._Header.ClippingBoxMin, this._Header.ClippingBoxMax);
      foreach (BoneController boneController in this._BoneControllerCollection)
        qcFileV11.ControllerCollection.Add(new CommandController(boneController.Index, this._BoneCollection[boneController.Bone].Name, boneController.Type, boneController.Start, boneController.End));
      qcFileV11.EyePosition = new CommandEyePosition(this._Header.EyePosition);
      qcFileV11.Flags = new CommandFlags(this._Header.TypeFlag);
      foreach (HitBox hitBox in this._HitBoxCollection)
        qcFileV11.HBoxCollection.Add(new CommandHBox(hitBox.Group, this._BoneCollection[hitBox.Bone].Name, hitBox.Min, hitBox.Max));
      qce.LodFlags = new CommandLodFlags((int) this._Header.LevelOfDetailFlags);
      for (int index1 = 0; index1 < this._LevelOfDetailCollection.Count; ++index1)
      {
        if (this._LevelOfDetailCollection[index1].Levels != 0)
        {
          CommandLodTemp commandLodTemp = new CommandLodTemp();
          commandLodTemp.Index = index1;
          for (int index2 = 0; index2 < this._LevelOfDetailCollection[index1].Levels; ++index2)
            commandLodTemp.Distances.Add(this._LevelOfDetailCollection[index1].Distance[index2]);
          qce.LodTempCollection.Add(commandLodTemp);
        }
      }
      qcFileV11.ModelName = new CommandModelName(modelname);
      qcFileV11.Origin = new CommandOrigin(new Point3D());
      foreach (Bone bone in this._BoneCollection)
      {
        if (bone.Parent == -1)
        {
          qcFileV11.Root = new CommandRoot(bone.Name);
          break;
        }
      }
      if (this._BoneCollection.Count > 0)
      {
        foreach (Sequence sequence in this._SequenceCollection)
        {
          CommandSequenceV11 commandSequenceV11 = new CommandSequenceV11();
          ActivityV14 activity = (ActivityV14) sequence.Activity;
          if (activity != ActivityV14.ACT_RESET)
          {
            commandSequenceV11.Activity = new OptionActivityV11((ActivityV11) Enumerators.ConvertActivity(typeof (ActivityV14), typeof (ActivityV11), (object) activity), (float) sequence.ActivityWeight);
            if (!commandSequenceV11.Activity.Activity.ToString().StartsWith("ACT_"))
            {
              qce.ReplaceActivityCollection.Add(new CommandReplaceActivity(sequence.Name, activity.ToString()));
              commandSequenceV11.Activity.Activity = ActivityV11.ACT_IDLE;
            }
          }
          for (int index = 0; index < sequence.BlendCount; ++index)
          {
            if (sequence.BlendType[index] != MotionFlags.None)
              commandSequenceV11.BlendCollection.Add(new OptionBlend(sequence.BlendType[index], sequence.BlendStart[index], sequence.BlendEnd[index]));
          }
          commandSequenceV11.Control.Flags = sequence.MotionType;
          foreach (Event @event in sequence.EventCollection)
            commandSequenceV11.EventCollection.Add(new OptionEvent(@event.EventValue, @event.Frame, @event.Options));
          commandSequenceV11.FileCollection.Add(this.DumpV11SmdAnimation(fileInfo.DirectoryName, sequence.Name));
          for (int index = 1; index < sequence.BlendCount; ++index)
            commandSequenceV11.FileCollection.Add(this.DumpV11SmdAnimation(fileInfo.DirectoryName, sequence.Name + "_blend" + (object) index));
          if ((double) sequence.FramesPerSecond != 30.0)
            commandSequenceV11.Fps = new OptionFps(sequence.FramesPerSecond);
          if (sequence.Flags == SequenceFlags.LOOPING)
            commandSequenceV11.Loop = new OptionLoop(true);
          commandSequenceV11.Name = sequence.Name;
          if (sequence.NodeEntry != 0)
            commandSequenceV11.Node = new OptionNodeV10(Convert.ToSByte(sequence.NodeEntry));
          for (int index = 0; index < sequence.PivotCollection.Length; ++index)
            commandSequenceV11.PivotCollection.Add(new OptionPivot(index, (float) sequence.PivotCollection[index].Start, (float) sequence.PivotCollection[index].End));
          if (sequence.NodeExit != 0)
          {
            if (sequence.NodeFlags == NodeFlags.REVERSE)
              commandSequenceV11.RTransition = new OptionRTransition(sequence.NodeEntry, sequence.NodeExit);
            else
              commandSequenceV11.Transition = new OptionTransition(sequence.NodeEntry, sequence.NodeExit);
          }
          qcFileV11.SequenceCollection.Add(commandSequenceV11);
        }
      }
      foreach (SoundGroup soundGroup in this._SoundGroupCollection)
      {
        CommandSoundGroup commandSoundGroup = new CommandSoundGroup();
        commandSoundGroup.Name = soundGroup.Name;
        foreach (string name in soundGroup.Sounds.Names)
          commandSoundGroup.Sounds.Add(name);
        qcFileV11.SoundGroupCollection.Add(commandSoundGroup);
      }
      foreach (Texture texture in this._TextureCollection)
        qcFileV11.TextureCollection.Add(new CommandTexture(texture.TextureName, texture.MaterialName));
      if (this._SkinCollection.Families > 1)
      {
        CommandTextureGroup commandTextureGroup = new CommandTextureGroup();
        commandTextureGroup.Name = "Skins";
        for (int index3 = 0; index3 < this._SkinCollection.Families; ++index3)
        {
          List<string> stringList2 = new List<string>();
          for (int index4 = 0; index4 < this._SkinCollection.References; ++index4)
            stringList2.Add(this._TextureCollection[(int) this._SkinCollection.Skins[index3, index4]].TextureName);
          commandTextureGroup.SkinCollection.Add(stringList2);
        }
      }
      qcFileV11.Write(file);
      qce.Write(fileInfo.DirectoryName + "\\" + str1 + ".qce");
      return StaticMethods.EliminateDuplicates(stringList1.ToArray());
    }

    public string DumpV11SmdReference(string path, string model, out string[] textures)
    {
      SmdFileV11 smdFileV11 = new SmdFileV11(SmdType.Reference);
      List<Time> bones = new List<Time>();
      Matrix[] matrixArray = new Matrix[this._BoneCollection.Count];
      for (int index = 0; index < this._BoneCollection.Count; ++index)
      {
        smdFileV11.NodeCollection.Add(new JbnLib.Smd.Node(Convert.ToSByte(index), this._BoneCollection[index].Name, Convert.ToSByte(this._BoneCollection[index].Parent)));
        bones.Add(new Time(index, this._BoneCollection[index].Position, this._BoneCollection[index].Rotation));
        if (index <= this._BoneCollection[index].Parent || this._BoneCollection[index].Parent < -1)
          Messages.ThrowException("Mdl.MdlFileV14.DumpV44SmdReference(string, string, out string[])", "Bone's parent's index is more-than-or-equal-to bone's index (bone name:" + this._BoneCollection[index].Name + ").");
        Matrix b = new Matrix(new Matrix(new Quaternion(this._BoneCollection[index].Rotation)), 3, 4);
        b[0, 3] = this._BoneCollection[index].Position.dX;
        b[1, 3] = this._BoneCollection[index].Position.dY;
        b[2, 3] = this._BoneCollection[index].Position.dZ;
        matrixArray[index] = this._BoneCollection[index].Parent != -1 ? StaticMethods.Multiply3x4(matrixArray[this._BoneCollection[index].Parent], b) : b;
      }
      for (int index = 0; index < this._BoneCollection.Count; ++index)
        matrixArray[index] = StaticMethods.Multiply3x4(matrixArray[index], this._BoneFixUpCollection[index].ToMatrix());
      smdFileV11.TimeCollection.Add(new Frame(0, bones));
      foreach (ModelInfoV14 modelInfo in this._ModelCollection[(int) this.FindModel(model)].ModelInfoCollection)
      {
        foreach (MeshV14 mesh in modelInfo.MeshCollection)
        {
          for (int index1 = 0; index1 < (int) mesh.TriangleCount; index1 += 3)
          {
            List<VertexV11> vertices = new List<VertexV11>();
            ushort vertexIndex1 = this._TriangleMapCollection[(int) mesh.TriangleIndex + index1 + 1].VertexIndex;
            BlendV11[] blending1 = this.BlendsToBlending(mesh.BoneMappings, (int) vertexIndex1);
            Point3D pos1 = new Point3D();
            for (int index2 = 0; index2 < blending1.Length; ++index2)
            {
              if (blending1[index2].Bone != (sbyte) -1)
                pos1 += (double) blending1[index2].Scale * StaticMethods.Transform(this._VertexCollection[(int) vertexIndex1].Position, matrixArray[(int) blending1[index2].Bone]);
            }
            vertices.Add(new VertexV11(blending1, pos1, this._NormalCollection[(int) vertexIndex1].Position, new TextureCoordinate(this._TextureCoordinateCollection[(int) vertexIndex1].U, this._TextureCoordinateCollection[(int) vertexIndex1].V * -1f)));
            ushort vertexIndex2 = this._TriangleMapCollection[(int) mesh.TriangleIndex + index1].VertexIndex;
            BlendV11[] blending2 = this.BlendsToBlending(mesh.BoneMappings, (int) vertexIndex2);
            Point3D pos2 = new Point3D();
            for (int index3 = 0; index3 < blending2.Length; ++index3)
            {
              if (blending2[index3].Bone != (sbyte) -1)
                pos2 += (double) blending2[index3].Scale * StaticMethods.Transform(this._VertexCollection[(int) vertexIndex2].Position, matrixArray[(int) blending2[index3].Bone]);
            }
            vertices.Add(new VertexV11(blending2, pos2, this._NormalCollection[(int) vertexIndex2].Position, new TextureCoordinate(this._TextureCoordinateCollection[(int) vertexIndex2].U, this._TextureCoordinateCollection[(int) vertexIndex2].V * -1f)));
            ushort vertexIndex3 = this._TriangleMapCollection[(int) mesh.TriangleIndex + index1 + 2].VertexIndex;
            BlendV11[] blending3 = this.BlendsToBlending(mesh.BoneMappings, (int) vertexIndex3);
            Point3D pos3 = new Point3D();
            for (int index4 = 0; index4 < blending3.Length; ++index4)
            {
              if (blending3[index4].Bone != (sbyte) -1)
                pos3 += (double) blending3[index4].Scale * StaticMethods.Transform(this._VertexCollection[(int) vertexIndex3].Position, matrixArray[(int) blending3[index4].Bone]);
            }
            vertices.Add(new VertexV11(blending3, pos3, this._NormalCollection[(int) vertexIndex3].Position, new TextureCoordinate(this._TextureCoordinateCollection[(int) vertexIndex3].U, this._TextureCoordinateCollection[(int) vertexIndex3].V * -1f)));
            smdFileV11.TriangleCollection.Add(new Triangle<VertexV11>(this._TextureCollection[modelInfo.SkinReference].TextureName, vertices));
          }
        }
      }
      textures = smdFileV11.GetTextures();
      smdFileV11.Write(path.TrimEnd('\\') + " \\" + StaticMethods.GoodFileName(model) + "_ref.smd");
      return StaticMethods.GoodFileName(model) + "_ref";
    }

    public string DumpV11SmdAnimation(string path, string sequence)
    {
      SmdFileV11 smdFileV11 = new SmdFileV11(SmdType.Animation);
      List<Time> bones1 = new List<Time>();
      for (int index = 0; index < this._BoneCollection.Count; ++index)
      {
        smdFileV11.NodeCollection.Add(new JbnLib.Smd.Node(Convert.ToSByte(index), this._BoneCollection[index].Name, Convert.ToSByte(this._BoneCollection[index].Parent)));
        bones1.Add(new Time(index, this._BoneCollection[index].Position, this._BoneCollection[index].Rotation));
      }
      byte blend;
      int sequence1 = (int) this.FindSequence(sequence, out blend);
      for (int index1 = 0; index1 < this._SequenceCollection[sequence1].FrameCount; ++index1)
      {
        List<Time> bones2 = new List<Time>();
        for (int index2 = 0; index2 < this._Header.Bones.Count; ++index2)
        {
          Point3D pos = new Point3D(this._BoneCollection[index2].Position);
          Point3D rot = new Point3D(this._BoneCollection[index2].Rotation);
          for (int index3 = 0; index3 < 6; ++index3)
          {
            if (this._SequenceCollection[sequence1].AnimationCollection[(int) blend, index2, index3] != null)
            {
              switch (index3)
              {
                case 0:
                  pos.X += (float) this._SequenceCollection[sequence1].AnimationCollection[(int) blend, index2, index3][index1] * this._BoneCollection[index2].ScalePosition.X;
                  continue;
                case 1:
                  pos.Y += (float) this._SequenceCollection[sequence1].AnimationCollection[(int) blend, index2, index3][index1] * this._BoneCollection[index2].ScalePosition.Y;
                  continue;
                case 2:
                  pos.Z += (float) this._SequenceCollection[sequence1].AnimationCollection[(int) blend, index2, index3][index1] * this._BoneCollection[index2].ScalePosition.Z;
                  continue;
                case 3:
                  rot.X += (float) this._SequenceCollection[sequence1].AnimationCollection[(int) blend, index2, index3][index1] * this._BoneCollection[index2].ScaleRotation.X;
                  continue;
                case 4:
                  rot.Y += (float) this._SequenceCollection[sequence1].AnimationCollection[(int) blend, index2, index3][index1] * this._BoneCollection[index2].ScaleRotation.Y;
                  continue;
                case 5:
                  rot.Z += (float) this._SequenceCollection[sequence1].AnimationCollection[(int) blend, index2, index3][index1] * this._BoneCollection[index2].ScaleRotation.Z;
                  continue;
                default:
                  continue;
              }
            }
          }
          if (this._BoneCollection[index2].Parent == -1 && this._SequenceCollection[sequence1].FrameCount > 1)
          {
            pos.X += (float) index1 * this._SequenceCollection[sequence1].LinearMovement[0] / (float) (this._SequenceCollection[sequence1].FrameCount - 1);
            pos.Y += (float) index1 * this._SequenceCollection[sequence1].LinearMovement[1] / (float) (this._SequenceCollection[sequence1].FrameCount - 1);
            pos.Z += (float) index1 * this._SequenceCollection[sequence1].LinearMovement[2] / (float) (this._SequenceCollection[sequence1].FrameCount - 1);
          }
          bones2.Add(new Time(index2, pos, rot));
        }
        smdFileV11.TimeCollection.Add(new Frame(index1, bones2));
      }
      if (smdFileV11.TimeCollection.Count == 0)
        smdFileV11.TimeCollection.Add(new Frame(0, bones1));
      smdFileV11.Rotate(-90.0);
      smdFileV11.Write(path.TrimEnd('\\') + " \\" + StaticMethods.GoodFileName(sequence) + "_ani.smd");
      return StaticMethods.GoodFileName(sequence) + "_ani";
    }

    public string[] DumpV44Qc(string file) => this.DumpV44Qc(file, this._Header.Name);

    public string[] DumpV44Qc(string file, string modelname)
    {
      List<string> stringList1 = new List<string>();
      FileInfo fileInfo = new FileInfo(file);
      Directory.CreateDirectory(fileInfo.DirectoryName);
      string str1 = this._Header.Name.Replace(".mdl", "");
      QcFileV44 qcFileV44 = new QcFileV44();
      Qce qce = new Qce();
      qce.QceVersion = new CommandQceVersion(QceGame.HalfLife2, 44, 1, 0);
      for (int index = 0; index < this._AttachmentCollection.Count; ++index)
        qcFileV44.AttachmentCollection.Add(new CommandAttachment(this._AttachmentCollection[index].Name, this._BoneCollection[this._AttachmentCollection[index].Bone].Name, this._AttachmentCollection[index].Position));
      qcFileV44.BBox = new CommandBBox(this._Header.BoundingBoxMin, this._Header.BoundingBoxMax);
      if (this._BodyGroupCollection.Count == 0)
        this._BodyGroupCollection.Add(new BodyGroupV14("studio", 1, this._Header.ModelsOffset[0]));
      foreach (BodyGroupV14 bodyGroup in this._BodyGroupCollection)
      {
        CommandBodyGroup commandBodyGroup = new CommandBodyGroup();
        commandBodyGroup.Name = bodyGroup.Name;
        for (int index = 0; index < bodyGroup.ModelCount; ++index)
        {
          byte model = this.FindModel(bodyGroup.ModelOffset + index * ModelV14.Length);
          if (this._ModelCollection[(int) model].Name == "blank")
          {
            commandBodyGroup.BodyCollection.Add(new BodyGroupItem("blank"));
          }
          else
          {
            string[] textures;
            commandBodyGroup.BodyCollection.Add(new BodyGroupItem("studio", this.DumpV44SmdReference(fileInfo.DirectoryName, this._ModelCollection[(int) model].Name, out textures)));
            foreach (string str2 in textures)
              stringList1.Add(str2);
          }
        }
        qcFileV44.BodyGroupCollection.Add(commandBodyGroup);
      }
      qcFileV44.CBox = new CommandCBox(this._Header.ClippingBoxMin, this._Header.ClippingBoxMax);
      foreach (BoneController boneController in this._BoneControllerCollection)
        qcFileV44.ControllerCollection.Add(new CommandController(boneController.Index, this._BoneCollection[boneController.Bone].Name, boneController.Type, boneController.Start, boneController.End));
      qcFileV44.EyePosition = new CommandEyePosition(this._Header.EyePosition);
      foreach (HitBox hitBox in this._HitBoxCollection)
        qcFileV44.HBoxCollection.Add(new CommandHBox(hitBox.Group, this._BoneCollection[hitBox.Bone].Name, hitBox.Min, hitBox.Max));
      qce.LodFlags = new CommandLodFlags((int) this._Header.LevelOfDetailFlags);
      for (int index1 = 0; index1 < this._LevelOfDetailCollection.Count; ++index1)
      {
        if (this._LevelOfDetailCollection[index1].Levels != 0)
        {
          CommandLodTemp commandLodTemp = new CommandLodTemp();
          commandLodTemp.Index = index1;
          for (int index2 = 0; index2 < this._LevelOfDetailCollection[index1].Levels; ++index2)
            commandLodTemp.Distances.Add(this._LevelOfDetailCollection[index1].Distance[index2]);
          qce.LodTempCollection.Add(commandLodTemp);
        }
      }
      qcFileV44.ModelName = new CommandModelName(modelname);
      qcFileV44.Origin = new CommandOrigin(new Point3D());
      foreach (Bone bone in this._BoneCollection)
      {
        if (bone.Parent == -1)
        {
          qcFileV44.Root = new CommandRoot(bone.Name);
          break;
        }
      }
      if (this._BoneCollection.Count > 0)
      {
        foreach (Sequence sequence in this._SequenceCollection)
        {
          CommandSequenceV44 commandSequenceV44 = new CommandSequenceV44();
          ActivityV14 activity = (ActivityV14) sequence.Activity;
          if (activity != ActivityV14.ACT_RESET)
          {
            commandSequenceV44.Activity = new OptionActivityV44((ActivityV44) Enumerators.ConvertActivity(typeof (ActivityV14), typeof (ActivityV44), (object) activity), (float) sequence.ActivityWeight);
            if (!commandSequenceV44.Activity.Activity.ToString().StartsWith("ACT_"))
            {
              qce.ReplaceActivityCollection.Add(new CommandReplaceActivity(sequence.Name, activity.ToString()));
              commandSequenceV44.Activity.Activity = ActivityV44.ACT_IDLE;
            }
          }
          for (int index = 0; index < sequence.BlendCount; ++index)
          {
            if (sequence.BlendType[index] != MotionFlags.None)
              commandSequenceV44.BlendCollection.Add(new OptionBlend(sequence.BlendType[index], sequence.BlendStart[index], sequence.BlendEnd[index]));
          }
          commandSequenceV44.Control.Flags = sequence.MotionType;
          foreach (Event @event in sequence.EventCollection)
            commandSequenceV44.EventCollection.Add(new OptionEvent(@event.EventValue, @event.Frame, @event.Options));
          commandSequenceV44.FileCollection.Add(this.DumpV11SmdAnimation(fileInfo.DirectoryName, sequence.Name));
          for (int index = 1; index < sequence.BlendCount; ++index)
            commandSequenceV44.FileCollection.Add(this.DumpV44SmdAnimation(fileInfo.DirectoryName, sequence.Name + "_blend" + (object) index));
          if ((double) sequence.FramesPerSecond != 30.0)
            commandSequenceV44.Fps = new OptionFps(sequence.FramesPerSecond);
          if (sequence.Flags == SequenceFlags.LOOPING)
            commandSequenceV44.Loop = new OptionLoop(true);
          commandSequenceV44.Name = sequence.Name;
          if (sequence.NodeEntry != 0)
            commandSequenceV44.Node = new OptionNodeV44(SmdFile.GetNode(StaticMethods.SmdFile, Convert.ToSByte(sequence.NodeEntry)));
          for (int index = 0; index < sequence.PivotCollection.Length; ++index)
            commandSequenceV44.PivotCollection.Add(new OptionPivot(index, (float) sequence.PivotCollection[index].Start, (float) sequence.PivotCollection[index].End));
          if (sequence.NodeExit != 0)
          {
            if (sequence.NodeFlags == NodeFlags.REVERSE)
              commandSequenceV44.RTransition = new OptionRTransition(sequence.NodeEntry, sequence.NodeExit);
            else
              commandSequenceV44.Transition = new OptionTransition(sequence.NodeEntry, sequence.NodeExit);
          }
          qcFileV44.SequenceCollection.Add(commandSequenceV44);
        }
      }
      if (this._SkinCollection.Families > 1)
      {
        CommandTextureGroup commandTextureGroup = new CommandTextureGroup();
        commandTextureGroup.Name = "Skins";
        for (int index3 = 0; index3 < this._SkinCollection.Families; ++index3)
        {
          List<string> stringList2 = new List<string>();
          for (int index4 = 0; index4 < this._SkinCollection.References; ++index4)
            stringList2.Add(this._TextureCollection[(int) this._SkinCollection.Skins[index3, index4]].TextureName);
          commandTextureGroup.SkinCollection.Add(stringList2);
        }
      }
      qcFileV44.Write(file);
      qce.Write(fileInfo.DirectoryName + "\\" + str1 + ".qce");
      return StaticMethods.EliminateDuplicates(stringList1.ToArray());
    }

    public string DumpV44SmdReference(string path, string model, out string[] textures)
    {
      SmdFileV44 smdFileV44 = new SmdFileV44(SmdType.Reference);
      List<Time> bones = new List<Time>();
      Matrix[] matrixArray = new Matrix[this._BoneCollection.Count];
      for (int index = 0; index < this._BoneCollection.Count; ++index)
      {
        smdFileV44.NodeCollection.Add(new JbnLib.Smd.Node(Convert.ToSByte(index), this._BoneCollection[index].Name, Convert.ToSByte(this._BoneCollection[index].Parent)));
        bones.Add(new Time(index, this._BoneCollection[index].Position, this._BoneCollection[index].Rotation));
        if (index <= this._BoneCollection[index].Parent || this._BoneCollection[index].Parent < -1)
          Messages.ThrowException("Mdl.MdlFileV14.DumpV44SmdReference(string, string, out string[])", "Bone's parent's index is more-than-or-equal-to bone's index (bone name:" + this._BoneCollection[index].Name + ").");
        Matrix b = new Matrix(new Matrix(new Quaternion(this._BoneCollection[index].Rotation)), 3, 4);
        b[0, 3] = this._BoneCollection[index].Position.dX;
        b[1, 3] = this._BoneCollection[index].Position.dY;
        b[2, 3] = this._BoneCollection[index].Position.dZ;
        matrixArray[index] = this._BoneCollection[index].Parent != -1 ? StaticMethods.Multiply3x4(matrixArray[this._BoneCollection[index].Parent], b) : b;
      }
      for (int index = 0; index < this._BoneCollection.Count; ++index)
        matrixArray[index] = StaticMethods.Multiply3x4(matrixArray[index], this._BoneFixUpCollection[index].ToMatrix());
      smdFileV44.TimeCollection.Add(new Frame(0, bones));
      foreach (ModelInfoV14 modelInfo in this._ModelCollection[(int) this.FindModel(model)].ModelInfoCollection)
      {
        foreach (MeshV14 mesh in modelInfo.MeshCollection)
        {
          for (int index1 = 0; index1 < (int) mesh.TriangleCount; index1 += 3)
          {
            List<VertexV44> vertices = new List<VertexV44>();
            ushort vertexIndex1 = this._TriangleMapCollection[(int) mesh.TriangleIndex + index1 + 1].VertexIndex;
            List<LinkV44> links1 = this.BlendsToLinks(mesh.BoneMappings, (int) vertexIndex1);
            Point3D pos1 = new Point3D();
            for (int index2 = 0; index2 < links1.Count; ++index2)
              pos1 += (double) links1[index2].Weight * StaticMethods.Transform(this._VertexCollection[(int) vertexIndex1].Position, matrixArray[(int) links1[index2].Bone]);
            vertices.Add(new VertexV44((sbyte) 0, pos1, this._NormalCollection[(int) vertexIndex1].Position, new TextureCoordinate(this._TextureCoordinateCollection[(int) vertexIndex1].U, this._TextureCoordinateCollection[(int) vertexIndex1].V * -1f), links1));
            ushort vertexIndex2 = this._TriangleMapCollection[(int) mesh.TriangleIndex + index1].VertexIndex;
            List<LinkV44> links2 = this.BlendsToLinks(mesh.BoneMappings, (int) vertexIndex2);
            Point3D pos2 = new Point3D();
            for (int index3 = 0; index3 < links2.Count; ++index3)
              pos2 += (double) links2[index3].Weight * StaticMethods.Transform(this._VertexCollection[(int) vertexIndex2].Position, matrixArray[(int) links2[index3].Bone]);
            vertices.Add(new VertexV44((sbyte) 0, pos2, this._NormalCollection[(int) vertexIndex2].Position, new TextureCoordinate(this._TextureCoordinateCollection[(int) vertexIndex2].U, this._TextureCoordinateCollection[(int) vertexIndex2].V * -1f), links2));
            ushort vertexIndex3 = this._TriangleMapCollection[(int) mesh.TriangleIndex + index1 + 2].VertexIndex;
            List<LinkV44> links3 = this.BlendsToLinks(mesh.BoneMappings, (int) vertexIndex3);
            Point3D pos3 = new Point3D();
            for (int index4 = 0; index4 < links3.Count; ++index4)
              pos3 += (double) links3[index4].Weight * StaticMethods.Transform(this._VertexCollection[(int) vertexIndex3].Position, matrixArray[(int) links3[index4].Bone]);
            vertices.Add(new VertexV44((sbyte) 0, pos3, this._NormalCollection[(int) vertexIndex3].Position, new TextureCoordinate(this._TextureCoordinateCollection[(int) vertexIndex3].U, this._TextureCoordinateCollection[(int) vertexIndex3].V * -1f), links3));
            smdFileV44.TriangleCollection.Add(new Triangle<VertexV44>(this._TextureCollection[modelInfo.SkinReference].TextureName, vertices));
          }
        }
      }
      textures = smdFileV44.GetTextures();
      smdFileV44.Write(path.TrimEnd('\\') + " \\" + StaticMethods.GoodFileName(model) + "_ref.smd");
      StaticMethods.SmdFile = path.TrimEnd('\\') + " \\" + StaticMethods.GoodFileName(model) + "_ref.smd";
      return StaticMethods.GoodFileName(model) + "_ref";
    }

    public string DumpV44SmdAnimation(string path, string sequence)
    {
      SmdFileV44 smdFileV44 = new SmdFileV44(SmdType.Animation);
      List<Time> bones1 = new List<Time>();
      for (int index = 0; index < this._BoneCollection.Count; ++index)
      {
        smdFileV44.NodeCollection.Add(new JbnLib.Smd.Node(Convert.ToSByte(index), this._BoneCollection[index].Name, Convert.ToSByte(this._BoneCollection[index].Parent)));
        bones1.Add(new Time(index, this._BoneCollection[index].Position, this._BoneCollection[index].Rotation));
      }
      byte blend;
      int sequence1 = (int) this.FindSequence(sequence, out blend);
      for (int index1 = 0; index1 < this._SequenceCollection[sequence1].FrameCount; ++index1)
      {
        List<Time> bones2 = new List<Time>();
        for (int index2 = 0; index2 < this._Header.Bones.Count; ++index2)
        {
          Point3D pos = new Point3D(this._BoneCollection[index2].Position);
          Point3D rot = new Point3D(this._BoneCollection[index2].Rotation);
          for (int index3 = 0; index3 < 6; ++index3)
          {
            if (this._SequenceCollection[sequence1].AnimationCollection[(int) blend, index2, index3] != null)
            {
              switch (index3)
              {
                case 0:
                  pos.X += (float) this._SequenceCollection[sequence1].AnimationCollection[(int) blend, index2, index3][index1] * this._BoneCollection[index2].ScalePosition.X;
                  continue;
                case 1:
                  pos.Y += (float) this._SequenceCollection[sequence1].AnimationCollection[(int) blend, index2, index3][index1] * this._BoneCollection[index2].ScalePosition.Y;
                  continue;
                case 2:
                  pos.Z += (float) this._SequenceCollection[sequence1].AnimationCollection[(int) blend, index2, index3][index1] * this._BoneCollection[index2].ScalePosition.Z;
                  continue;
                case 3:
                  rot.X += (float) this._SequenceCollection[sequence1].AnimationCollection[(int) blend, index2, index3][index1] * this._BoneCollection[index2].ScaleRotation.X;
                  continue;
                case 4:
                  rot.Y += (float) this._SequenceCollection[sequence1].AnimationCollection[(int) blend, index2, index3][index1] * this._BoneCollection[index2].ScaleRotation.Y;
                  continue;
                case 5:
                  rot.Z += (float) this._SequenceCollection[sequence1].AnimationCollection[(int) blend, index2, index3][index1] * this._BoneCollection[index2].ScaleRotation.Z;
                  continue;
                default:
                  continue;
              }
            }
          }
          if (this._BoneCollection[index2].Parent == -1 && this._SequenceCollection[sequence1].FrameCount > 1)
          {
            pos.X += (float) index1 * this._SequenceCollection[sequence1].LinearMovement[0] / (float) (this._SequenceCollection[sequence1].FrameCount - 1);
            pos.Y += (float) index1 * this._SequenceCollection[sequence1].LinearMovement[1] / (float) (this._SequenceCollection[sequence1].FrameCount - 1);
            pos.Z += (float) index1 * this._SequenceCollection[sequence1].LinearMovement[2] / (float) (this._SequenceCollection[sequence1].FrameCount - 1);
          }
          bones2.Add(new Time(index2, pos, rot));
        }
        smdFileV44.TimeCollection.Add(new Frame(index1, bones2));
      }
      if (smdFileV44.TimeCollection.Count == 0)
        smdFileV44.TimeCollection.Add(new Frame(0, bones1));
      smdFileV44.Rotate(-90.0);
      smdFileV44.Write(path.TrimEnd('\\') + " \\" + StaticMethods.GoodFileName(sequence) + "_ani.smd");
      StaticMethods.SmdFile = path.TrimEnd('\\') + " \\" + StaticMethods.GoodFileName(sequence) + "_ani.smd";
      return StaticMethods.GoodFileName(sequence) + "_ani";
    }

    private byte FindBody(string body)
    {
      for (byte index = 0; (int) index < this._BodyGroupCollection.Count; ++index)
      {
        if (this._BodyGroupCollection[(int) index].Name == body)
          return index;
      }
      Messages.ThrowException("Mdl.MdlFileV14.FindBody(string)", Messages.NOT_FOUND_IN_COLLECTION + body);
      return 0;
    }

    private byte FindModel(int offset)
    {
      for (byte model = 0; (int) model < this._Header.ModelCount; ++model)
      {
        if (this._Header.ModelsOffset[(int) model] == offset)
          return model;
      }
      Messages.ThrowException("Mdl.MdlFileV14.FindModel(int)", Messages.OFFSET_NOT_FOUND + (object) offset);
      return 0;
    }

    private byte FindModel(string model)
    {
      for (byte index = 0; (int) index < this._ModelCollection.Count; ++index)
      {
        if (this._ModelCollection[(int) index].Name == model)
          return index;
      }
      Messages.ThrowException("Mdl.MdlFileV14.FindModel(string)", Messages.NOT_FOUND_IN_COLLECTION + model);
      return 0;
    }

    private byte FindSequence(string sequence, out byte blend)
    {
      blend = (byte) 0;
      if (sequence.EndsWith("_blend1"))
      {
        blend = (byte) 1;
        sequence = sequence.Substring(0, sequence.Length - 7);
      }
      if (sequence.EndsWith("_blend2"))
      {
        blend = (byte) 2;
        sequence = sequence.Substring(0, sequence.Length - 7);
      }
      if (sequence.EndsWith("_blend3"))
      {
        blend = (byte) 3;
        sequence = sequence.Substring(0, sequence.Length - 7);
      }
      for (byte index = 0; (int) index < this._SequenceCollection.Count; ++index)
      {
        if (this._SequenceCollection[(int) index].Name == sequence)
          return index;
      }
      Messages.ThrowException("Mdl.MdlFileV14.FindSequence(string)", Messages.NOT_FOUND_IN_COLLECTION + sequence);
      return 0;
    }

    private sbyte[] BlendsToBones(sbyte[] bonemappings, int vertex)
    {
      sbyte[] bones = new sbyte[4];
      for (byte index = 0; (int) index < this._BlendingCollection[vertex].Value.Length; ++index)
        bones[(int) index] = this._BlendingCollection[vertex].Value[(int) index] != (sbyte) -1 ? bonemappings[(int) this._BlendingCollection[vertex].Value[(int) index]] : (sbyte) -1;
      return bones;
    }

    private BlendV11[] BlendsToBlending(sbyte[] bonemappings, int vertex)
    {
      BlendV11[] blending = new BlendV11[4];
      for (byte index = 0; (int) index < this._BlendingCollection[vertex].Value.Length; ++index)
      {
        sbyte bone = this._BlendingCollection[vertex].Value[(int) index] != (sbyte) -1 ? bonemappings[(int) this._BlendingCollection[vertex].Value[(int) index]] : (sbyte) -1;
        blending[(int) index] = new BlendV11(bone, this._BlendingScalesCollection[vertex].Value[(int) index]);
      }
      return blending;
    }

    private List<LinkV44> BlendsToLinks(sbyte[] bonemappings, int vertex)
    {
      List<LinkV44> links = new List<LinkV44>();
      for (byte index = 0; (int) index < this._BlendingCollection[vertex].Value.Length; ++index)
      {
        if (this._BlendingCollection[vertex].Value[(int) index] != (sbyte) -1)
          links.Add(new LinkV44(bonemappings[(int) this._BlendingCollection[vertex].Value[(int) index]], this._BlendingScalesCollection[vertex].Value[(int) index]));
      }
      return links;
    }
  }
}
