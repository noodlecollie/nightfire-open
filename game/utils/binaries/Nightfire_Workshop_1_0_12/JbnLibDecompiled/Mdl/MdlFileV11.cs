// Decompiled with JetBrains decompiler
// Type: JbnLib.Mdl.MdlFileV11
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Lang;
using JbnLib.Shared;
using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO;
using System.Text;
using System.Threading;

namespace JbnLib.Mdl
{
  public class MdlFileV11 : MdlFile
  {
    private HeaderV11 _Header;
    private ModelHeaderV11 _ModelHeader;
    private List<BodyGroupV11> _BodyGroupCollection;
    private List<BodyV11> _BodyCollection;
    private List<ModelV11> _ModelCollection;

    public HeaderV11 Header => this._Header;

    public ModelHeaderV11 ModelHeader => this._ModelHeader;

    public List<BodyGroupV11> BodyGroupCollection
    {
      get => this._BodyGroupCollection;
      set => this._BodyGroupCollection = value;
    }

    public List<BodyV11> BodyCollection
    {
      get => this._BodyCollection;
      set => this._BodyCollection = value;
    }

    public List<ModelV11> ModelCollection
    {
      get => this._ModelCollection;
      set => this._ModelCollection = value;
    }

    public MdlFileV11()
    {
      Thread.CurrentThread.CurrentUICulture = new CultureInfo("en");
      Thread.CurrentThread.CurrentCulture = new CultureInfo("en-US");
    }

    public void Clear()
    {
      this._Header = new HeaderV11();
      this._BoneCollection = new List<Bone>();
      this._BoneControllerCollection = new List<BoneController>();
      this._HitBoxCollection = new List<HitBox>();
      this._SequenceCollection = new List<Sequence>();
      this._SequenceGroupCollection = new List<SequenceGroup>();
      this._TextureCollection = new List<Texture>();
      this._SkinCollection = new SkinCollection();
      this._BodyGroupCollection = new List<BodyGroupV11>();
      this._BodyCollection = new List<BodyV11>();
      this._AttachmentCollection = new List<Attachment>();
      this._SoundGroupCollection = new List<SoundGroup>();
      this._TriangleMapCollection = new List<TriangleMap>();
      this._VertexCollection = new List<Vertex>();
      this._NormalCollection = new List<Normal>();
      this._TextureCoordinateCollection = new List<TextureCoordinate>();
      this._BlendingScalesCollection = new List<BlendingScales>();
      this._BlendingCollection = new List<Blending>();
      this._BoneFixUpCollection = new List<BoneFixUp>();
      this._ModelHeader = new ModelHeaderV11();
      this._BodyGroupCollection = new List<BodyGroupV11>();
      this._ModelCollection = new List<ModelV11>();
      this._LevelOfDetailCollection = new List<LevelOfDetail>();
    }

    public void Read(string file)
    {
      FileInfo fileInfo1 = new FileInfo(file);
      if (fileInfo1.Exists)
      {
        this.Clear();
        FileStream input1 = fileInfo1.OpenRead();
        BinaryReader binaryReader1 = new BinaryReader((Stream) input1);
        this._Header = new HeaderV11(binaryReader1.ReadBytes(HeaderV11.Length));
        if (this._Header.Identifier != "MDLZ")
          Messages.ThrowException("Mdl.MdlFileV11.Read(string)", Messages.IDENTIFIER_MISMATCH + Messages.EXPECTING + "MDLZ " + Messages.GOT + this._Header.Identifier);
        if (this._Header.Version != 11)
          Messages.ThrowException("Mdl.MdlFileV11.Read(string)", Messages.VERSION_MISMATCH + Messages.EXPECTING + "11 " + Messages.GOT + (object) this._Header.Version);
        input1.Position = (long) this._Header.Bones.Offset;
        for (int index = 0; index < this._Header.Bones.Count; ++index)
          this._BoneCollection.Add(new Bone(binaryReader1.ReadBytes(Bone.Length)));
        input1.Position = (long) this._Header.BoneControllers.Offset;
        for (int index = 0; index < this._Header.BoneControllers.Count; ++index)
          this._BoneControllerCollection.Add(new BoneController(binaryReader1.ReadBytes(BoneController.Length)));
        input1.Position = (long) this._Header.HitBoxes.Offset;
        for (int index = 0; index < this._Header.HitBoxes.Count; ++index)
          this._HitBoxCollection.Add(new HitBox(binaryReader1.ReadBytes(HitBox.Length)));
        input1.Position = (long) this._Header.Sequences.Offset;
        for (int index = 0; index < this._Header.Sequences.Count; ++index)
          this._SequenceCollection.Add(new Sequence(binaryReader1.ReadBytes(Sequence.Length)));
        foreach (Sequence sequence in this._SequenceCollection)
        {
          input1.Position = (long) sequence.Events.Offset;
          for (int index = 0; index < sequence.Events.Count; ++index)
            sequence.EventCollection[index] = new Event(binaryReader1.ReadBytes(Event.Length));
        }
        foreach (Sequence sequence in this._SequenceCollection)
        {
          input1.Position = (long) sequence.Pivots.Offset;
          for (int index = 0; index < sequence.Pivots.Count; ++index)
            sequence.PivotCollection[index] = new Pivot(binaryReader1.ReadBytes(Pivot.Length));
        }
        input1.Position = (long) this._Header.SequenceGroups.Offset;
        for (int index = 0; index < this._Header.SequenceGroups.Count; ++index)
          this._SequenceGroupCollection.Add(new SequenceGroup(binaryReader1.ReadBytes(SequenceGroup.Length)));
        foreach (Sequence sequence in this._SequenceCollection)
        {
          FileStream input2 = input1;
          BinaryReader binaryReader2 = binaryReader1;
          if (sequence.SequenceGroupIndex > 0)
          {
            FileInfo fileInfo2 = new FileInfo(file.Substring(0, file.LastIndexOf('\\') + 1) + this._SequenceGroupCollection[sequence.SequenceGroupIndex].FileName.Substring(this._SequenceGroupCollection[sequence.SequenceGroupIndex].FileName.IndexOf('\\') + 1));
            if (fileInfo2.Exists)
            {
              input2 = fileInfo2.OpenRead();
              binaryReader2 = new BinaryReader((Stream) input2);
              string str = Encoding.ASCII.GetString(binaryReader2.ReadBytes(4));
              if (str != "IDSQ")
                Messages.ThrowException("Mdl.MdlFileV11.Read(string)", Messages.IDENTIFIER_MISMATCH + Messages.EXPECTING + "IDSQ " + Messages.GOT + str + Messages.FILE + fileInfo2.FullName);
              int num = binaryReader2.ReadInt32();
              if (num != 11)
                Messages.ThrowException("Mdl.MdlFileV11.Read(string)", Messages.VERSION_MISMATCH + Messages.EXPECTING + "11 " + Messages.GOT + (object) num + Messages.FILE + fileInfo2.FullName);
            }
            else
              Messages.ThrowException("Mdl.MdlFileV11.Read(string)", Messages.FILE_NOT_FOUND + fileInfo2.FullName);
          }
          input2.Position = (long) sequence.AnimationOffset;
          sequence.AnimationCollection = new Animation[sequence.BlendCount, this._Header.Bones.Count, 6];
          for (int index1 = 0; index1 < sequence.BlendCount; ++index1)
          {
            for (int index2 = 0; index2 < this._Header.Bones.Count; ++index2)
            {
              long position1 = input2.Position;
              for (int index3 = 0; index3 < 6; ++index3)
              {
                ushort num1 = binaryReader2.ReadUInt16();
                if (num1 > (ushort) 0)
                {
                  sequence.AnimationCollection[index1, index2, index3] = new Animation();
                  long position2 = input2.Position;
                  input2.Position = position1 + (long) num1;
                  int num2 = 0;
                  while (num2 < sequence.FrameCount)
                  {
                    AnimationValue animationValue = new AnimationValue(binaryReader2.ReadInt16());
                    num2 += (int) animationValue.Total;
                    short num3 = 0;
                    for (int index4 = 0; index4 < (int) animationValue.Valid; ++index4)
                    {
                      num3 = binaryReader2.ReadInt16();
                      sequence.AnimationCollection[index1, index2, index3].Add(num3);
                    }
                    for (int valid = (int) animationValue.Valid; valid < (int) animationValue.Total; ++valid)
                      sequence.AnimationCollection[index1, index2, index3].Add(num3);
                  }
                  input2.Position = position2;
                }
              }
            }
          }
          if (sequence._SequenceGroupIndex > 0)
          {
            binaryReader2.Close();
            input2.Close();
          }
        }
        input1.Position = (long) this._Header.Textures.Offset;
        for (int index = 0; index < this._Header.Textures.Count; ++index)
          this._TextureCollection.Add(new Texture(binaryReader1.ReadBytes(Texture.Length)));
        input1.Position = (long) this._Header.SkinsOffset;
        this._SkinCollection = new SkinCollection(binaryReader1.ReadBytes(this._Header.SkinFamilies * this._Header.SkinReferences * 2), this._Header.SkinFamilies, this._Header.SkinReferences);
        input1.Position = (long) this._Header.BodyGroups.Offset;
        for (int index = 0; index < this._Header.BodyGroups.Count; ++index)
          this._BodyGroupCollection.Add(new BodyGroupV11(binaryReader1.ReadBytes(BodyGroupV11.Length)));
        foreach (BodyGroupV11 bodyGroup in this._BodyGroupCollection)
        {
          input1.Position = (long) bodyGroup.BodyOffset;
          for (int index = 0; index < bodyGroup.ModelCount; ++index)
            this._BodyCollection.Add(new BodyV11(binaryReader1.ReadBytes(120)));
        }
        foreach (BodyV11 body in this._BodyCollection)
        {
          input1.Position = (long) body.LodBlendOffset;
          body.LodBlends = binaryReader1.ReadBytes(body.LodBlendCount);
          input1.Position = (long) body.LodPositionOffset;
          body.LodPositionCollection = new LodPosition[body.LodBlends.Length][];
          for (int index5 = 0; index5 < body.LodBlends.Length; ++index5)
          {
            body.LodPositionCollection[index5] = new LodPosition[(int) body.LodBlends[index5]];
            for (byte index6 = 0; (int) index6 < (int) body.LodBlends[index5]; ++index6)
              body.LodPositionCollection[index5][(int) index6] = new LodPosition(binaryReader1.ReadBytes(20));
          }
          input1.Position = (long) body.LodBoneOffset;
          body.LodBones = binaryReader1.ReadBytes(body.LodBoneCount);
          input1.Position = (long) body.LodNormOffset;
          for (int index = 0; index < body.LodBoneCount; ++index)
            body.LodNormCollection.Add(new Point3D(binaryReader1.ReadBytes(12)));
        }
        for (int index7 = 0; index7 < this._BodyCollection.Count; ++index7)
        {
          input1.Position = (long) this._BodyCollection[index7].LevelOfDetailOffset;
          for (int index8 = 0; index8 < this._BodyCollection[index7].LevelOfDetailCount; ++index8)
            this._BodyCollection[index7].LevelOfDetailCollection.Add(new LevelOfDetailV11(binaryReader1.ReadBytes(20)));
        }
        for (int index9 = 0; index9 < this._BodyCollection.Count; ++index9)
        {
          for (int index10 = 0; index10 < this._BodyCollection[index9].LevelOfDetailCount; ++index10)
          {
            input1.Position = (long) this._BodyCollection[index9].LevelOfDetailCollection[index10].LodVerticesOffset;
            short lodvertexcount;
            do
            {
              lodvertexcount = binaryReader1.ReadInt16();
              LodVertices lodVertices = new LodVertices(lodvertexcount);
              if (Math.Sign(lodvertexcount) == -1)
                lodvertexcount *= (short) -1;
              for (int index11 = 0; index11 < (int) lodvertexcount; ++index11)
                lodVertices.LodVertexCollection.Add(new LodVertex(binaryReader1.ReadBytes(LodVertex.Length)));
              if (lodvertexcount > (short) 0)
                this._BodyCollection[index9].LevelOfDetailCollection[index10].LodVerticesCollection.Add(lodVertices);
            }
            while (lodvertexcount > (short) 0);
          }
        }
        input1.Position = (long) this._Header.Attachments.Offset;
        for (int index = 0; index < this._Header.Attachments.Count; ++index)
          this._AttachmentCollection.Add(new Attachment(binaryReader1.ReadBytes(Attachment.Length)));
        input1.Position = (long) this._Header.SoundGroups.Offset;
        for (int index = 0; index < this._Header.SoundGroups.Count; ++index)
          this._SoundGroupCollection.Add(new SoundGroup(binaryReader1.ReadBytes(SoundGroup.Length)));
        long position = input1.Position;
        for (int index = 0; index < this._Header.SoundGroups.Count; ++index)
        {
          input1.Position = (long) this._SoundGroupCollection[index].Offset + position;
          this._SoundGroupCollection[index].Sounds = new Sounds(binaryReader1.ReadBytes(Sounds.Length));
        }
        this._ModelHeader = new ModelHeaderV11(binaryReader1.ReadBytes(ModelHeaderV11.Length));
        input1.Position = (long) this._ModelHeader.TriangleMapOffset;
        for (int index = 0; index < this._ModelHeader.TriangleCount; ++index)
          this._TriangleMapCollection.Add(new TriangleMap(binaryReader1.ReadBytes(TriangleMap.Length)));
        input1.Position = (long) this._ModelHeader.VerticesOffset;
        for (int index = 0; index < this._ModelHeader.VertexCount; ++index)
          this._VertexCollection.Add(new Vertex(binaryReader1.ReadBytes(Vertex.Length)));
        input1.Position = (long) this._ModelHeader.NormalOffset;
        for (int index = 0; index < this._ModelHeader.VertexCount; ++index)
          this._NormalCollection.Add(new Normal(binaryReader1.ReadBytes(Normal.Length)));
        input1.Position = (long) this._ModelHeader.TextureCoordsOffset;
        for (int index = 0; index < this._ModelHeader.VertexCount; ++index)
          this._TextureCoordinateCollection.Add(new TextureCoordinate(binaryReader1.ReadBytes(TextureCoordinate.Length)));
        input1.Position = (long) this._ModelHeader.BlendingScalesOffset;
        for (int index = 0; index < this._ModelHeader.VertexCount; ++index)
          this._BlendingScalesCollection.Add(new BlendingScales(binaryReader1.ReadBytes(BlendingScales.Length)));
        input1.Position = (long) this._ModelHeader.BlendingOffset;
        for (int index = 0; index < this._ModelHeader.VertexCount; ++index)
          this._BlendingCollection.Add(new Blending(binaryReader1.ReadBytes(Blending.Length)));
        input1.Position = (long) this._ModelHeader.BoneFixUpOffset;
        for (int index = 0; index < this._Header.Bones.Count; ++index)
          this._BoneFixUpCollection.Add(new BoneFixUp(binaryReader1.ReadBytes(BoneFixUp.Length)));
        for (int index = 0; index < this._ModelHeader.ModelCount; ++index)
        {
          input1.Position = (long) this._ModelHeader.ModelsOffset[index];
          this._ModelCollection.Add(new ModelV11(binaryReader1.ReadBytes(ModelV11.Length)));
        }
        binaryReader1.Close();
        input1.Close();
      }
      else
        Messages.ThrowException("Mdl.MdlFileV11.Read(string)", Messages.FILE_NOT_FOUND + file);
    }

    public void Write(string file)
    {
      FileStream output1 = new FileStream(file, FileMode.Create);
      BinaryWriter bw1 = new BinaryWriter((Stream) output1);
      HeaderV11 headerV11 = new HeaderV11();
      ModelHeaderV11 modelHeaderV11 = new ModelHeaderV11();
      bw1.Write(new byte[HeaderV11.Length]);
      headerV11.Bones = new CountOffsetPair(this._BoneCollection.Count, Convert.ToInt32(output1.Position));
      foreach (Bone bone in this._BoneCollection)
      {
        bw1.Write(bone.GetBytes());
        bw1.Flush();
      }
      StaticMethods.Buffer4(ref bw1);
      headerV11.BoneControllers = new CountOffsetPair(this._BoneControllerCollection.Count, Convert.ToInt32(output1.Position));
      foreach (BoneController boneController in this._BoneControllerCollection)
      {
        bw1.Write(boneController.GetBytes());
        bw1.Flush();
      }
      StaticMethods.Buffer4(ref bw1);
      headerV11.Attachments = new CountOffsetPair(this._AttachmentCollection.Count, Convert.ToInt32(output1.Position));
      foreach (Attachment attachment in this._AttachmentCollection)
      {
        bw1.Write(attachment.GetBytes());
        bw1.Flush();
      }
      StaticMethods.Buffer4(ref bw1);
      headerV11.HitBoxes = new CountOffsetPair(this._HitBoxCollection.Count, Convert.ToInt32(output1.Position));
      foreach (HitBox hitBox in this._HitBoxCollection)
      {
        bw1.Write(hitBox.GetBytes());
        bw1.Flush();
      }
      StaticMethods.Buffer4(ref bw1);
      for (int index1 = 0; index1 < this._SequenceGroupCollection.Count; ++index1)
      {
        FileStream output2 = output1;
        BinaryWriter bw2 = bw1;
        if (index1 > 0)
        {
          output2 = new FileInfo(file.Substring(0, file.LastIndexOf('\\')) + this._SequenceGroupCollection[index1].FileName.Substring(this._SequenceGroupCollection[index1].FileName.LastIndexOf('\\'))).Create();
          bw2 = new BinaryWriter((Stream) output2);
          bw2.Write(new byte[76]);
          bw2.Flush();
        }
        for (int index2 = 0; index2 < this._SequenceCollection.Count; ++index2)
        {
          if (index1 == this._SequenceCollection[index2]._SequenceGroupIndex)
          {
            this._SequenceCollection[index2].AnimationOffset = Convert.ToInt32(output2.Position);
            ushort uint16 = Convert.ToUInt16(StaticMethods.Buffer(this._SequenceCollection[index2].BlendCount * this._BoneCollection.Count * 12, 4) + this._SequenceCollection[index2].BlendCount * this._BoneCollection.Count * 12);
            ushort num = uint16;
            List<short> shortList = new List<short>();
            for (int index3 = 0; index3 < this._SequenceCollection[index2].BlendCount; ++index3)
            {
              for (int index4 = 0; index4 < this._BoneCollection.Count; ++index4)
              {
                for (int index5 = 0; index5 < 6; ++index5)
                {
                  if (this._SequenceCollection[index2].AnimationCollection[index3, index4, index5] == null)
                  {
                    bw2.Write((ushort) 0);
                  }
                  else
                  {
                    bw2.Write(Convert.ToUInt16((int) num + shortList.Count * 2 - ((int) num - (int) uint16)));
                    shortList.AddRange((IEnumerable<short>) MdlFileV14.CompressAnimations((List<short>) this._SequenceCollection[index2].AnimationCollection[index3, index4, index5]));
                  }
                  bw2.Flush();
                }
                uint16 -= (ushort) 12;
              }
            }
            StaticMethods.Buffer4(ref bw2);
            for (int index6 = 0; index6 < shortList.Count; ++index6)
            {
              bw2.Write(shortList[index6]);
              if (index6 % 2048 == 0)
                bw2.Flush();
            }
            StaticMethods.Buffer4(ref bw2);
          }
        }
        if (index1 > 0)
        {
          int int32 = Convert.ToInt32(output2.Position);
          output2.Position = 0L;
          bw2.Write(Encoding.ASCII.GetBytes("IDSQ"));
          bw2.Write(11);
          bw2.Write(Encoding.ASCII.GetBytes("models\\" + this._Header.Name.Insert(this._Header.Name.LastIndexOf('.'), index1.ToString().PadLeft(2, '0'))));
          output2.Position = 72L;
          bw2.Write(int32);
          bw2.Flush();
          bw2.Close();
          output2.Close();
        }
      }
      headerV11.Sequences = new CountOffsetPair(this._SequenceCollection.Count, Convert.ToInt32(output1.Position));
      bw1.Write(new byte[this._SequenceCollection.Count * Sequence.Length]);
      bw1.Flush();
      for (int index = 0; index < this._SequenceCollection.Count; ++index)
      {
        this._SequenceCollection[index].Events = new CountOffsetPair(this._SequenceCollection[index].EventCollection.Length, Convert.ToInt32(output1.Position));
        foreach (Event @event in this._SequenceCollection[index].EventCollection)
          bw1.Write(@event.GetBytes());
        if (this._SequenceCollection[index].PivotCollection.Length != 0)
        {
          this._SequenceCollection[index].Pivots = new CountOffsetPair(this._SequenceCollection[index].PivotCollection.Length, Convert.ToInt32(output1.Position));
          foreach (Pivot pivot in this._SequenceCollection[index].PivotCollection)
            bw1.Write(pivot.GetBytes());
          StaticMethods.Buffer4(ref bw1);
        }
        else
        {
          StaticMethods.Buffer4(ref bw1);
          this._SequenceCollection[index].Pivots = new CountOffsetPair(0, Convert.ToInt32(output1.Position));
        }
      }
      long position1 = output1.Position;
      output1.Position = (long) headerV11.Sequences.Offset;
      foreach (Sequence sequence in this._SequenceCollection)
      {
        bw1.Write(sequence.GetBytes());
        bw1.Flush();
      }
      output1.Position = position1;
      headerV11.SequenceGroups = new CountOffsetPair(this._SequenceGroupCollection.Count, Convert.ToInt32(output1.Position));
      foreach (SequenceGroup sequenceGroup in this._SequenceGroupCollection)
        bw1.Write(sequenceGroup.GetBytes());
      StaticMethods.Buffer4(ref bw1);
      foreach (BodyV11 body in this._BodyCollection)
      {
        body.LodPositionOffset = Convert.ToInt32(output1.Position);
        for (int index7 = 0; index7 < body.LodPositionCollection.Length; ++index7)
        {
          for (int index8 = 0; index8 < body.LodPositionCollection[index7].Length; ++index8)
            bw1.Write(body.LodPositionCollection[index7][index8].GetBytes());
        }
        StaticMethods.Buffer4(ref bw1);
      }
      foreach (BodyV11 body in this._BodyCollection)
      {
        body.LodBlendOffset = Convert.ToInt32(output1.Position);
        bw1.Write(body.LodBlends);
        StaticMethods.Buffer4(ref bw1);
      }
      headerV11.BodyGroups = new CountOffsetPair(this._BodyGroupCollection.Count, Convert.ToInt32(output1.Position));
      bw1.Write(new byte[this._BodyGroupCollection.Count * BodyGroupV11.Length]);
      StaticMethods.Buffer4(ref bw1);
      foreach (BodyGroupV11 bodyGroup in this.BodyGroupCollection)
      {
        bodyGroup.BodyOffset = Convert.ToInt32(output1.Position);
        bw1.Write(new byte[bodyGroup.ModelCount * BodyV11.Length]);
      }
      StaticMethods.Buffer4(ref bw1);
      foreach (BodyV11 body in this._BodyCollection)
      {
        body.LodBoneOffset = Convert.ToInt32(output1.Position);
        bw1.Write(body.LodBones);
        StaticMethods.Buffer4(ref bw1);
        body.LodNormOffset = Convert.ToInt32(output1.Position);
        body.UnknownOffset0 = body.LodNormOffset;
        foreach (Point3D lodNorm in body.LodNormCollection)
          bw1.Write(lodNorm.GetBytes());
        StaticMethods.Buffer4(ref bw1);
        body.LevelOfDetailOffset = Convert.ToInt32(output1.Position);
        bw1.Write(new byte[body.LevelOfDetailCount * LevelOfDetailV11.Length]);
        StaticMethods.Buffer4(ref bw1);
        foreach (LevelOfDetailV11 levelOfDetail in body.LevelOfDetailCollection)
        {
          levelOfDetail.LodVerticesOffset = Convert.ToInt32(output1.Position);
          foreach (LodVertices lodVertices in levelOfDetail.LodVerticesCollection)
            bw1.Write(lodVertices.GetBytes());
          bw1.Write((short) 0);
          StaticMethods.Buffer4(ref bw1);
        }
        long position2 = output1.Position;
        output1.Position = (long) body.LevelOfDetailOffset;
        foreach (LevelOfDetailV11 levelOfDetail in body.LevelOfDetailCollection)
          bw1.Write(levelOfDetail.GetBytes());
        StaticMethods.Buffer4(ref bw1);
        output1.Position = position2;
      }
      long position3 = output1.Position;
      output1.Position = (long) this.BodyGroupCollection[0].BodyOffset;
      foreach (BodyV11 body in this._BodyCollection)
        bw1.Write(body.GetBytes());
      StaticMethods.Buffer4(ref bw1);
      output1.Position = position3;
      headerV11.Textures = new CountOffsetPair(this._TextureCollection.Count, Convert.ToInt32(output1.Position));
      foreach (Texture texture in this._TextureCollection)
        bw1.Write(texture.GetBytes());
      StaticMethods.Buffer4(ref bw1);
      headerV11.SkinsOffset = Convert.ToInt32(output1.Position);
      bw1.Write(this._SkinCollection.GetBytes());
      StaticMethods.Buffer4(ref bw1);
      headerV11.TextureInfo = Convert.ToInt32(output1.Position);
      headerV11.SoundGroups = new CountOffsetPair(this._SoundGroupCollection.Count, Convert.ToInt32(output1.Position));
      for (int index = 0; index < this._SoundGroupCollection.Count; ++index)
      {
        this._SoundGroupCollection[index].Offset = index * Sounds.Length;
        bw1.Write(this._SoundGroupCollection[index].GetBytes());
      }
      bw1.Flush();
      headerV11.SoundsOffset = Convert.ToInt32(output1.Position);
      for (int index = 0; index < this._SoundGroupCollection.Count; ++index)
      {
        bw1.Write(this._SoundGroupCollection[index].Sounds.GetBytes());
        bw1.Flush();
      }
      StaticMethods.Buffer4(ref bw1);
      headerV11.ModelHeaderOffset = Convert.ToInt32(output1.Position);
      bw1.Write(new byte[ModelHeaderV11.Length]);
      modelHeaderV11.VerticesOffset = Convert.ToInt32(output1.Position);
      int num1 = 0;
      foreach (Vertex vertex in this._VertexCollection)
      {
        bw1.Write(vertex.GetBytes());
        ++num1;
        if (num1 % 256 == 0)
          bw1.Flush();
      }
      bw1.Flush();
      modelHeaderV11.BlendingOffset = Convert.ToInt32(output1.Position);
      int num2 = 0;
      foreach (Blending blending in this._BlendingCollection)
      {
        bw1.Write(blending.GetBytes());
        ++num2;
        if (num2 % 1024 == 0)
          bw1.Flush();
      }
      StaticMethods.Buffer4(ref bw1);
      modelHeaderV11.BlendingScalesOffset = Convert.ToInt32(output1.Position);
      int num3 = 0;
      foreach (BlendingScales blendingScales in this._BlendingScalesCollection)
      {
        bw1.Write(blendingScales.GetBytes());
        ++num3;
        if (num3 % 256 == 0)
          bw1.Flush();
      }
      bw1.Flush();
      modelHeaderV11.NormalOffset = Convert.ToInt32(output1.Position);
      int num4 = 0;
      foreach (Normal normal in this._NormalCollection)
      {
        bw1.Write(normal.GetBytes());
        ++num4;
        if (num4 % 256 == 0)
          bw1.Flush();
      }
      bw1.Flush();
      modelHeaderV11.TextureCoordsOffset = Convert.ToInt32(output1.Position);
      int num5 = 0;
      foreach (TextureCoordinate textureCoordinate in this._TextureCoordinateCollection)
      {
        bw1.Write(textureCoordinate.GetBytes());
        ++num5;
        if (num5 % 512 == 0)
          bw1.Flush();
      }
      StaticMethods.Buffer4(ref bw1);
      modelHeaderV11.TriangleMapOffset = Convert.ToInt32(output1.Position);
      int num6 = 0;
      foreach (TriangleMap triangleMap in this._TriangleMapCollection)
      {
        bw1.Write(triangleMap.GetBytes());
        ++num6;
        if (num6 % 2048 == 0)
          bw1.Flush();
      }
      StaticMethods.Buffer4(ref bw1);
      modelHeaderV11.BoneFixUpOffset = Convert.ToInt32(output1.Position);
      int num7 = 0;
      foreach (BoneFixUp boneFixUp in this._BoneFixUpCollection)
      {
        bw1.Write(boneFixUp.GetBytes());
        ++num7;
        if (num7 % 96 == 0)
          bw1.Flush();
      }
      bw1.Flush();
      for (int index = 0; index < this._ModelCollection.Count; ++index)
      {
        modelHeaderV11.ModelsOffset[index] = Convert.ToInt32(output1.Position);
        bw1.Write(this._ModelCollection[index].GetBytes());
        bw1.Flush();
      }
      headerV11.LevelOfDetailFlags = this._Header._LevelOfDetailFlags;
      headerV11.BoundingBoxMax = this._Header.BoundingBoxMax;
      headerV11.BoundingBoxMin = this._Header.BoundingBoxMin;
      headerV11.ClippingBoxMax = this._Header.ClippingBoxMax;
      headerV11.ClippingBoxMin = this._Header.ClippingBoxMin;
      headerV11.EyePosition = this._Header.EyePosition;
      headerV11.FileLength = Convert.ToInt32(output1.Position);
      headerV11.Identifier = "MDLZ";
      headerV11.Name = this._Header.Name;
      headerV11.SkinFamilies = this._SkinCollection.Families;
      headerV11.SkinReferences = this._SkinCollection.References;
      headerV11.TypeFlag = this._Header.TypeFlag;
      headerV11.Version = 11;
      headerV11.TransitionFlagsOffset = this._Header.TransitionFlagsOffset;
      headerV11.TransitionsCount = this._Header.TransitionsCount;
      headerV11.TransitionsOffset = this._Header.TransitionsOffset;
      output1.Position = 0L;
      bw1.Write(headerV11.GetBytes());
      modelHeaderV11.ModelCount = this._ModelCollection.Count;
      modelHeaderV11.TriangleCount = this._TriangleMapCollection.Count;
      modelHeaderV11.VertexCount = this._VertexCollection.Count;
      output1.Position = (long) headerV11.ModelHeaderOffset;
      bw1.Write(modelHeaderV11.GetBytes());
      int index9 = 0;
      output1.Position = (long) headerV11.BodyGroups.Offset;
      foreach (BodyGroupV11 bodyGroup in this._BodyGroupCollection)
      {
        bodyGroup.ModelOffset = modelHeaderV11.ModelsOffset[index9];
        bw1.Write(bodyGroup.GetBytes());
        index9 += bodyGroup.ModelCount;
      }
      bw1.Flush();
      bw1.Close();
      output1.Close();
    }

    public MdlFileV14 ToV14()
    {
      MdlFileV14 v14 = new MdlFileV14();
      v14.Clear();
      foreach (BodyGroupV11 bodyGroup in this._BodyGroupCollection)
        v14.BodyGroupCollection.Add(new BodyGroupV14(bodyGroup.Name, bodyGroup.ModelCount, bodyGroup.UnknownCount, bodyGroup.ModelOffset));
      v14.Header.LevelOfDetailFlags = this._Header.LevelOfDetailFlags;
      v14.Header.Attachments = this._Header.Attachments;
      v14.Header.BlendingOffset = this._ModelHeader.BlendingOffset;
      v14.Header.BodyGroups = this._Header.BodyGroups;
      v14.Header.BoneControllers = this._Header.BoneControllers;
      v14.Header.BoneFixUpOffset = this._ModelHeader.BoneFixUpOffset;
      v14.Header.Bones = this._Header.Bones;
      v14.Header.BoundingBoxMax = this._Header.BoundingBoxMax;
      v14.Header.BoundingBoxMin = this._Header.BoundingBoxMin;
      v14.Header.ClippingBoxMax = this._Header.ClippingBoxMax;
      v14.Header.ClippingBoxMin = this._Header.ClippingBoxMin;
      v14.Header.EyePosition = this._Header.EyePosition;
      v14.Header.FileLength = this._Header.FileLength;
      v14.Header.HitBoxes = this._Header.HitBoxes;
      v14.Header.ModelCount = this._ModelHeader.ModelCount;
      v14.Header.ModelsOffset = this._ModelHeader.ModelsOffset;
      v14.Header.Name = this._Header.Name;
      v14.Header.BlendingScalesOffset = this._ModelHeader.BlendingScalesOffset;
      v14.Header.SequenceGroups = this._Header.SequenceGroups;
      v14.Header.Sequences = this._Header.Sequences;
      v14.Header.SkinFamilies = this._Header.SkinFamilies;
      v14.Header.SkinReferences = this._Header.SkinReferences;
      v14.Header.SkinsOffset = this._Header.SkinsOffset;
      v14.Header.SoundGroups = this._Header.SoundGroups;
      v14.Header.SoundsOffset = this._Header.SoundsOffset;
      v14.Header.TextureCoordsOffset = this._ModelHeader.TextureCoordsOffset;
      v14.Header.TextureInfo = this._Header.TextureInfo;
      v14.Header.Textures = this._Header.Textures;
      v14.Header.TransitionFlagsOffset = this._Header.TransitionFlagsOffset;
      v14.Header.TransitionsCount = this._Header.TransitionsCount;
      v14.Header.TransitionsOffset = this._Header.TransitionsOffset;
      v14.Header.TriangleCount = this._ModelHeader.TriangleCount;
      v14.Header.TriangleMapOffset = this._ModelHeader.TriangleMapOffset;
      v14.Header.TypeFlag = this._Header.TypeFlag;
      v14.Header.NormalOffset = this._ModelHeader.NormalOffset;
      v14.Header.VertexCount = this._ModelHeader.VertexCount;
      v14.Header.VerticesOffset = this._ModelHeader.VerticesOffset;
      v14._BlendingScalesCollection = this._BlendingScalesCollection;
      v14.TextureCoordinateCollection = this._TextureCoordinateCollection;
      v14.NormalCollection = this._NormalCollection;
      v14.VertexCollection = this._VertexCollection;
      v14.BlendingCollection = this._BlendingCollection;
      foreach (BodyGroupV11 bodyGroup in this._BodyGroupCollection)
      {
        byte model = this.FindModel(bodyGroup.ModelOffset);
        for (byte index = model; (int) index < bodyGroup.ModelCount + (int) model; ++index)
        {
          ModelV14 modelV14 = new ModelV14(this._ModelCollection[(int) index].Name, this._ModelCollection[(int) index].Index);
          for (byte skin = 0; (int) skin < this._ModelCollection[(int) index].ModelInfoCollection.Length; ++skin)
          {
            if (this._ModelCollection[(int) index].ModelInfoCollection[(int) skin].MeshCollection.Count > 0)
            {
              ModelInfoV14 modelInfoV14 = new ModelInfoV14((int) skin);
              foreach (MeshV11 mesh in this._ModelCollection[(int) index].ModelInfoCollection[(int) skin].MeshCollection)
              {
                MeshV14 meshV14 = new MeshV14();
                mesh.BoneMappings.CopyTo((Array) meshV14.BoneMappings, 0);
                meshV14.TriangleCount = mesh.TriangleCount;
                meshV14.TriangleIndex = Convert.ToUInt16(v14.TriangleMapCollection.Count);
                for (int triangleIndex = (int) mesh.TriangleIndex; triangleIndex < (int) mesh.TriangleCount + (int) mesh.TriangleIndex; triangleIndex += 3)
                {
                  v14.TriangleMapCollection.Add(this._TriangleMapCollection[triangleIndex + 1]);
                  v14.TriangleMapCollection.Add(this._TriangleMapCollection[triangleIndex + 2]);
                  v14.TriangleMapCollection.Add(this._TriangleMapCollection[triangleIndex]);
                }
                modelInfoV14.MeshCollection.Add(meshV14);
              }
              modelV14.ModelInfoCollection.Add(modelInfoV14);
            }
          }
          v14.ModelCollection.Add(modelV14);
        }
      }
      foreach (Sequence sequence in this._SequenceCollection)
      {
        v14.SequenceCollection.Add(sequence);
        v14.SequenceCollection[v14.SequenceCollection.Count - 1].SequenceGroupIndex = 0;
      }
      v14.SequenceGroupCollection.Add(this._SequenceGroupCollection[0]);
      v14.BoneCollection = this._BoneCollection;
      v14.AttachmentCollection = this._AttachmentCollection;
      v14.BoneControllerCollection = this._BoneControllerCollection;
      v14.BoneFixUpCollection = this._BoneFixUpCollection;
      v14.HitBoxCollection = this._HitBoxCollection;
      v14.SkinCollection = this._SkinCollection;
      v14.SoundGroupCollection = this._SoundGroupCollection;
      v14.TextureCollection = this._TextureCollection;
      return v14;
    }

    private byte FindBody(string body)
    {
      for (byte index = 0; (int) index < this._BodyGroupCollection.Count; ++index)
      {
        if (this._BodyGroupCollection[(int) index].Name == body)
          return index;
      }
      Messages.ThrowException("Mdl.MdlFileV11.FindBody(string)", Messages.NOT_FOUND_IN_COLLECTION + body);
      return 0;
    }

    private byte FindModel(int offset)
    {
      for (byte model = 0; (int) model < this._ModelHeader.ModelCount; ++model)
      {
        if (this._ModelHeader.ModelsOffset[(int) model] == offset)
          return model;
      }
      Messages.ThrowException("Mdl.MdlFileV11.FindModel(int)", Messages.OFFSET_NOT_FOUND + (object) offset);
      return 0;
    }

    private byte FindModel(string model)
    {
      for (byte index = 0; (int) index < this._ModelCollection.Count; ++index)
      {
        if (this._ModelCollection[(int) index].Name == model)
          return index;
      }
      Messages.ThrowException("Mdl.MdlFileV11.FindModel(string)", Messages.NOT_FOUND_IN_COLLECTION + model);
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
      Messages.ThrowException("Mdl.MdlFileV11.FindSequence(string, byte)", Messages.NOT_FOUND_IN_COLLECTION + sequence);
      return 0;
    }
  }
}
