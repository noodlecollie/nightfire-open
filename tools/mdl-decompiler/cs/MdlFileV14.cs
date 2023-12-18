using JbnLib.Lang;
using JbnLib.Qc;
using JbnLib.Smd;
using JbnLib.Shared;
using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO;
using System.Threading;

namespace JbnLib.Mdl
{
    public class MdlFileV14 : MdlFile
    {
        #region " Collections "

        private HeaderV14 _Header;
        public HeaderV14 Header
        {
            get { return _Header; }
        }

        private List<BodyGroupV14> _BodyGroupCollection;
        public List<BodyGroupV14> BodyGroupCollection
        {
            get { return _BodyGroupCollection; }
            set { _BodyGroupCollection = value; }
        }

        private List<ModelV14> _ModelCollection;
        public List<ModelV14> ModelCollection
        {
            get { return _ModelCollection; }
            set { _ModelCollection = value; }
        }
        #endregion

        #region " Constructors "
        public MdlFileV14()
        {
            Thread.CurrentThread.CurrentUICulture = new CultureInfo("en");
            Thread.CurrentThread.CurrentCulture = new CultureInfo("en-US");
        }
        #endregion

        #region " Methods "
        public void Clear()
        {
            _Header = new HeaderV14();
            _BoneCollection = new List<Bone>();
            _BoneControllerCollection = new List<BoneController>();
            _HitBoxCollection = new List<HitBox>();
            _SequenceCollection = new List<Sequence>();
            _SequenceGroupCollection = new List<SequenceGroup>();
            _TextureCollection = new List<Texture>();
            _SkinCollection = new SkinCollection();
            _BodyGroupCollection = new List<BodyGroupV14>();
            _AttachmentCollection = new List<Attachment>();
            _SoundGroupCollection = new List<SoundGroup>();
            _TriangleMapCollection = new List<TriangleMap>();
            _VertexCollection = new List<Vertex>();
            _NormalCollection = new List<Normal>();
            _TextureCoordinateCollection = new List<TextureCoordinate>();
            _BlendingScalesCollection = new List<BlendingScales>();
            _BlendingCollection = new List<Blending>();
            _BoneFixUpCollection = new List<BoneFixUp>();
            _ModelCollection = new List<ModelV14>();
            _LevelOfDetailCollection = new List<LevelOfDetail>();

        }
        public void Read(string file)
        {
            FileInfo fi = new FileInfo(file);
            if (fi.Exists)
            {
                Clear();

                FileStream fs = fi.OpenRead();
                BinaryReader br = new BinaryReader(fs);

                #region " Header "
                _Header = new HeaderV14(br.ReadBytes(HeaderV14.Length));
                if (_Header.Identifier != "MDLZ")
                    Messages.ThrowException("Mdl.MdlFileV14.Read(string)", Messages.IDENTIFIER_MISMATCH + Messages.EXPECTING + "MDLZ " + Messages.GOT + _Header.Identifier);
                if (_Header.Version != 14)
                    Messages.ThrowException("Mdl.MdlFileV14.Read(string)", Messages.VERSION_MISMATCH + Messages.EXPECTING + "14 " + Messages.GOT + _Header.Version);
                #endregion

                #region " Bones "
                fs.Position = _Header.Bones.Offset;
                for (int i = 0; i < _Header.Bones.Count; i++)
                    _BoneCollection.Add(new Bone(br.ReadBytes(Bone.Length)));
                #endregion

                #region " Bone Controllers "
                fs.Position = _Header.BoneControllers.Offset;
                for (int i = 0; i < _Header.BoneControllers.Count; i++)
                    _BoneControllerCollection.Add(new BoneController(br.ReadBytes(BoneController.Length)));
                #endregion

                #region " Hit Boxes "
                fs.Position = _Header.HitBoxes.Offset;
                for (int i = 0; i < _Header.HitBoxes.Count; i++)
                    _HitBoxCollection.Add(new HitBox(br.ReadBytes(HitBox.Length)));
                #endregion

                #region " Sequences "
                fs.Position = _Header.Sequences.Offset;
                for (int i = 0; i < _Header.Sequences.Count; i++)
                    _SequenceCollection.Add(new Sequence(br.ReadBytes(Sequence.Length)));
                #endregion

                #region " Events "
                foreach (Sequence seq in _SequenceCollection)
                {
                    fs.Position = seq.Events.Offset;
                    for (int i = 0; i < seq.Events.Count; i++)
                        seq.EventCollection[i] = new Event(br.ReadBytes(Event.Length));
                }
                #endregion

                #region " Pivots "
                foreach (Sequence seq in _SequenceCollection)
                {
                    fs.Position = seq.Pivots.Offset;
                    for (int i = 0; i < seq.Pivots.Count; i++)
                        seq.PivotCollection[i] = new Pivot(br.ReadBytes(Pivot.Length));
                }
                #endregion

                #region " Animations "
                foreach (Sequence seq in _SequenceCollection)
                {
                    fs.Position = seq.AnimationOffset;

                    seq.AnimationCollection = new Animation[seq.BlendCount, _Header.Bones.Count, 6];
                    for (int bl = 0; bl < seq.BlendCount; bl++)
                    {
                        for (int b = 0; b < _Header.Bones.Count; b++)
                        {
                            long rowoffset = fs.Position;
                            for (int t = 0; t < 6; t++)
                            {
                                ushort read = br.ReadUInt16();
                                if (read > 0)
                                {
                                    seq.AnimationCollection[bl, b, t] = new Animation();

                                    long columnoffset = fs.Position;

                                    fs.Position = rowoffset + read;
                                    int handled = 0;
                                    while (handled < seq.FrameCount)
                                    {
                                        AnimationValue value = new AnimationValue(br.ReadInt16());
                                        handled += value.Total;
                                        short read2 = 0;
                                        for (int v = 0; v < value.Valid; v++)
                                        {
                                            read2 = br.ReadInt16();
                                            seq.AnimationCollection[bl, b, t].Add(read2);
                                        }
                                        for (int v = value.Valid; v < value.Total; v++)
                                        {
                                            seq.AnimationCollection[bl, b, t].Add(read2);
                                        }
                                    }

                                    fs.Position = columnoffset;
                                }
                            }
                        }
                    }
                    fs.Position += StaticMethods.Buffer(_Header.Bones.Count * 12, 16);

                }
                #endregion

                #region " Sequence Groups "
                fs.Position = _Header.SequenceGroups.Offset;
                for (int i = 0; i < _Header.SequenceGroups.Count; i++)
                    _SequenceGroupCollection.Add(new SequenceGroup(br.ReadBytes(SequenceGroup.Length)));
                #endregion

                #region " Level of Detail "
                if (_Header.LevelOfDetailFlags != LevelOfDetailFlags.None)
                {
                    fs.Position += StaticMethods.Buffer(Convert.ToInt32(fs.Position), 16);
                    for (byte i = 0; i < Enumerators.LodFlagsToInt(_Header.LevelOfDetailFlags); i++)
                        _LevelOfDetailCollection.Add(new LevelOfDetail(br.ReadBytes(LevelOfDetail.Length)));
                }
                #endregion

                #region " Textures "
                fs.Position = _Header.Textures.Offset;
                for (int i = 0; i < _Header.Textures.Count; i++)
                    _TextureCollection.Add(new Texture(br.ReadBytes(Texture.Length)));
                #endregion

                #region " Skin "
                fs.Position = _Header.SkinsOffset;
                _SkinCollection = new SkinCollection(br.ReadBytes(_Header.SkinFamilies * _Header.SkinReferences * 2), _Header.SkinFamilies, _Header.SkinReferences);
                #endregion

                #region " Body Groups "
                fs.Position = _Header.BodyGroups.Offset;
                for (int i = 0; i < _Header.BodyGroups.Count; i++)
                    _BodyGroupCollection.Add(new BodyGroupV14(br.ReadBytes(BodyGroupV14.Length)));
                #endregion

                #region " Attachments "
                fs.Position = _Header.Attachments.Offset;
                for (int i = 0; i < _Header.Attachments.Count; i++)
                    _AttachmentCollection.Add(new Attachment(br.ReadBytes(Attachment.Length)));
                #endregion

                #region " Sound Groups "
                fs.Position = _Header.SoundGroups.Offset;
                for (int i = 0; i < _Header.SoundGroups.Count; i++)
                {
                    _SoundGroupCollection.Add(new SoundGroup(br.ReadBytes(SoundGroup.Length)));
                }

                long tempoffset = fs.Position;
                for (int i = 0; i < _Header.SoundGroups.Count; i++)
                {
                    fs.Position = _SoundGroupCollection[i].Offset + tempoffset;
                    _SoundGroupCollection[i].Sounds = new Sounds(br.ReadBytes(Sounds.Length));
                }


                #endregion

                #region " Triangle Map "
                fs.Position = _Header.TriangleMapOffset;
                for (int i = 0; i < _Header.TriangleCount; i++)
                    _TriangleMapCollection.Add(new TriangleMap(br.ReadBytes(TriangleMap.Length)));
                #endregion

                #region " Vertices "
                fs.Position = _Header.VerticesOffset;
                for (int i = 0; i < _Header.VertexCount; i++)
                    _VertexCollection.Add(new Vertex(br.ReadBytes(Vertex.Length)));
                #endregion

                #region " Normals "
                fs.Position = _Header.NormalOffset;
                for (int i = 0; i < _Header.VertexCount; i++)
                    _NormalCollection.Add(new Normal(br.ReadBytes(Normal.Length)));
                #endregion

                #region " Texture Coordinates "
                fs.Position = _Header.TextureCoordsOffset;
                for (int i = 0; i < _Header.VertexCount; i++)
                    _TextureCoordinateCollection.Add(new TextureCoordinate(br.ReadBytes(TextureCoordinate.Length)));
                #endregion

                #region " Blending Scales "
                fs.Position = _Header.BlendingScalesOffset;
                for (int i = 0; i < _Header.VertexCount; i++)
                    _BlendingScalesCollection.Add(new BlendingScales(br.ReadBytes(BlendingScales.Length)));
                #endregion

                #region " Bone Reference "
                fs.Position = _Header.BlendingOffset;
                for (int i = 0; i < _Header.VertexCount; i++)
                    _BlendingCollection.Add(new Blending(br.ReadBytes(Blending.Length)));
                #endregion

                #region " Bone Fix Up "
                fs.Position = _Header.BoneFixUpOffset;
                for (int i = 0; i < _Header.Bones.Count; i++)
                    _BoneFixUpCollection.Add(new BoneFixUp(br.ReadBytes(BoneFixUp.Length)));
                #endregion

                #region " Models "
                for (int i = 0; i < _Header.ModelCount; i++)
                {
                    fs.Position = _Header.ModelsOffset[i];
                    _ModelCollection.Add(new ModelV14(br.ReadBytes(ModelV14.Length)));
                }
                #endregion

                #region " Model Info and Meshes "
                foreach (ModelV14 temp in _ModelCollection)
                {
                    // Model Info
                    foreach (int temp2 in temp.ModelInfoOffset)
                    {
                        if (temp2 != 0)
                        {
                            fs.Position = temp2;
                            temp.ModelInfoCollection.Add(new ModelInfoV14(br.ReadBytes(ModelInfoV14.Length)));
                        }
                    }

                    // Meshes
                    foreach (ModelInfoV14 temp2 in temp.ModelInfoCollection)
                    {
                        fs.Position = temp2.Mesh.Offset;
                        for (int i = 0; i < temp2.Mesh.Count; i++)
                            temp2.MeshCollection.Add(new MeshV14(br.ReadBytes(MeshV14.Length)));
                    }
                }
                #endregion

                br.Close();
                fs.Close();
            }
            else
                Messages.ThrowException("Mdl.MdlFileV14.Read(string)", Messages.FILE_NOT_FOUND + file);
        }
        public void Write(string file)
        {
            FileStream fs = new FileStream(file, FileMode.Create);
            BinaryWriter bw = new BinaryWriter(fs);

            // Create Header to fill with offsets/counts.
            HeaderV14 header = new HeaderV14();

            // Make room for the header.
            bw.Write(new byte[HeaderV14.Length]);

            #region " Bones "
            header.Bones = new CountOffsetPair(_BoneCollection.Count, Convert.ToInt32(fs.Position));
            foreach (Bone bone in _BoneCollection)
            {
                bw.Write(bone.GetBytes());
                bw.Flush();
            }
            StaticMethods.Buffer16(ref bw);
            #endregion

            #region " Bone Controllers "
            header.BoneControllers = new CountOffsetPair(_BoneControllerCollection.Count, Convert.ToInt32(fs.Position));
            foreach (BoneController controller in _BoneControllerCollection)
            {
                bw.Write(controller.GetBytes());
                bw.Flush();
            }
            StaticMethods.Buffer16(ref bw);
            #endregion

            #region " Attachments "
            header.Attachments = new CountOffsetPair(_AttachmentCollection.Count, Convert.ToInt32(fs.Position));
            foreach (Attachment attachment in _AttachmentCollection)
            {
                bw.Write(attachment.GetBytes());
                bw.Flush();
            }
            StaticMethods.Buffer16(ref bw);
            #endregion

            #region " Hit Boxes "
            header.HitBoxes = new CountOffsetPair(_HitBoxCollection.Count, Convert.ToInt32(fs.Position));
            foreach (HitBox hitbox in _HitBoxCollection)
            {
                bw.Write(hitbox.GetBytes());
                bw.Flush();
            }
            StaticMethods.Buffer16(ref bw);
            #endregion

            #region " Animations "
            for (int s = 0; s < _SequenceCollection.Count; s++)
            {
                // Update animation offset.
                _SequenceCollection[s].AnimationOffset = Convert.ToInt32(fs.Position);

                // Write animation headers.
                ushort rowoffset = Convert.ToUInt16(StaticMethods.Buffer(_SequenceCollection[s].BlendCount * _BoneCollection.Count * 12, 16) + _SequenceCollection[s].BlendCount * _BoneCollection.Count * 12);
                ushort valuesoffset = rowoffset;
                List<short> values = new List<short>();

                for (int bl = 0; bl < _SequenceCollection[s].BlendCount; bl++)
                {
                    for (int b = 0; b < _BoneCollection.Count; b++)
                    {
                        for (int t = 0; t < 6; t++)
                        {
                            if (_SequenceCollection[s].AnimationCollection[bl, b, t] == null)
                                bw.Write((ushort)0);
                            else
                            {
                                bw.Write(Convert.ToUInt16(valuesoffset + (values.Count * 2) - (valuesoffset - rowoffset)));

                                values.AddRange(CompressAnimations(_SequenceCollection[s].AnimationCollection[bl, b, t]));
                            }
                            bw.Flush();
                        }
                        rowoffset -= 12;
                    }
                }
                StaticMethods.Buffer16(ref bw);

                // Write animation values.
                for (int i = 0; i < values.Count; i++)
                {
                    bw.Write(values[i]);
                    if (i % 2048 == 0)
                        bw.Flush();
                }
                StaticMethods.Buffer16(ref bw);
            }
            #endregion

            long temp;

            #region " Sequences BUFFER"
            header.Sequences = new CountOffsetPair(_SequenceCollection.Count, Convert.ToInt32(fs.Position));

            // Reserve space
            bw.Write(new byte[_SequenceCollection.Count * Sequence.Length]);
            bw.Flush();
            #endregion

            #region " Events "
            for (int i = 0; i < _SequenceCollection.Count; i++)
            {
                _SequenceCollection[i].Events = new CountOffsetPair(_SequenceCollection[i].EventCollection.Length, Convert.ToInt32(fs.Position));
                foreach (Event evt in _SequenceCollection[i].EventCollection)
                {
                    bw.Write(evt.GetBytes());
                }
                StaticMethods.Buffer16(ref bw);

                if (_SequenceCollection[i].PivotCollection.Length > 0)
                {
                    _SequenceCollection[i].Pivots = new CountOffsetPair(_SequenceCollection[i].PivotCollection.Length, Convert.ToInt32(fs.Position));
                    foreach (Pivot pvt in _SequenceCollection[i].PivotCollection)
                    {
                        bw.Write(pvt.GetBytes());
                    }
                    StaticMethods.Buffer16(ref bw);
                }
                else
                {
                    StaticMethods.Buffer16(ref bw);
                    _SequenceCollection[i].Pivots = new CountOffsetPair(0, Convert.ToInt32(fs.Position));
                }
            }
            #endregion

            #region " Sequences "
            // Store offset so we may go back to it
            temp = fs.Position;

            // Go to sequences and write them
            fs.Position = header.Sequences.Offset;
            foreach (Sequence seq in _SequenceCollection)
            {
                bw.Write(seq.GetBytes());
                bw.Flush();
            }

            // Go back to where we were.
            fs.Position = temp;
            #endregion

            #region " Sequence Groups "
            header.SequenceGroups = new CountOffsetPair(_SequenceGroupCollection.Count, Convert.ToInt32(fs.Position));
            foreach (SequenceGroup seqgroup in _SequenceGroupCollection)
            {
                bw.Write(seqgroup.GetBytes());
            }
            StaticMethods.Buffer16(ref bw);
            #endregion

            #region " Level of Detail "
            foreach (LevelOfDetail lod in _LevelOfDetailCollection)
            {
                bw.Write(lod.GetBytes());
            }
            StaticMethods.Buffer16(ref bw);
            #endregion

            #region " Body Groups BUFFER"
            header.BodyGroups = new CountOffsetPair(_BodyGroupCollection.Count, Convert.ToInt32(fs.Position));

            // Reserve space
            bw.Write(new byte[_BodyGroupCollection.Count * BodyGroupV14.Length]);
            StaticMethods.Buffer16(ref bw);
            #endregion

            #region " Textures "
            header.Textures = new CountOffsetPair(_TextureCollection.Count, Convert.ToInt32(fs.Position));
            foreach (Texture tex in _TextureCollection)
            {
                bw.Write(tex.GetBytes());
            }
            StaticMethods.Buffer16(ref bw);
            #endregion

            #region " Skins "
            header.SkinsOffset = Convert.ToInt32(fs.Position);
            bw.Write(_SkinCollection.GetBytes());
            StaticMethods.Buffer16(ref bw);
            #endregion

            #region " Texture Info "
            header.TextureInfo = Convert.ToInt32(fs.Position);
            #endregion

            #region " Sound Groups "
            // Write header
            header.SoundGroups = new CountOffsetPair(_SoundGroupCollection.Count, Convert.ToInt32(fs.Position));
            for (int i = 0; i < _SoundGroupCollection.Count; i++)
            {
                _SoundGroupCollection[i].Offset = (i * Sounds.Length);
                bw.Write(_SoundGroupCollection[i].GetBytes());
            }
            bw.Flush();

            // Write sounds
            header.SoundsOffset = Convert.ToInt32(fs.Position);
            for (int i = 0; i < _SoundGroupCollection.Count; i++)
            {
                bw.Write(_SoundGroupCollection[i].Sounds.GetBytes());
                bw.Flush();
            }
            StaticMethods.Buffer16(ref bw);
            #endregion

            int flush;

            #region " Vertices "
            header.VerticesOffset = Convert.ToInt32(fs.Position);
            flush = 0;
            foreach (Vertex vert in _VertexCollection)
            {
                bw.Write(vert.GetBytes());
                flush++;

                if (flush % 256 == 0)
                    bw.Flush();
            }
            bw.Flush();
            #endregion

            #region " Blending "
            header.BlendingOffset = Convert.ToInt32(fs.Position);
            flush = 0;
            foreach (Blending blend in _BlendingCollection)
            {
                bw.Write(blend.GetBytes());
                flush++;

                if (flush % 1024 == 0)
                    bw.Flush();
            }
            StaticMethods.Buffer16(ref bw);
            #endregion

            #region " Blending Scales "
            header.BlendingScalesOffset = Convert.ToInt32(fs.Position);
            flush = 0;
            foreach (BlendingScales norm in _BlendingScalesCollection)
            {
                bw.Write(norm.GetBytes());
                flush++;

                if (flush % 256 == 0)
                    bw.Flush();
            }
            bw.Flush();
            #endregion

            #region " Normals "
            header.NormalOffset = Convert.ToInt32(fs.Position);
            flush = 0;
            foreach (Normal norm in _NormalCollection)
            {
                bw.Write(norm.GetBytes());
                flush++;

                if (flush % 256 == 0)
                    bw.Flush();
            }
            bw.Flush();
            #endregion

            #region " Texture Coordinates "
            header.TextureCoordsOffset = Convert.ToInt32(fs.Position);
            flush = 0;
            foreach (TextureCoordinate texcoord in _TextureCoordinateCollection)
            {
                bw.Write(texcoord.GetBytes());
                flush++;

                if (flush % 512 == 0)
                    bw.Flush();
            }
            StaticMethods.Buffer16(ref bw);
            #endregion

            #region " Triangle Map "
            header.TriangleMapOffset = Convert.ToInt32(fs.Position);
            flush = 0;
            foreach (TriangleMap tri in _TriangleMapCollection)
            {
                bw.Write(tri.GetBytes());
                flush++;

                if (flush % 2048 == 0)
                    bw.Flush();
            }
            StaticMethods.Buffer16(ref bw);
            #endregion

            #region " Bone Fix Up "
            header.BoneFixUpOffset = Convert.ToInt32(fs.Position);
            flush = 0;
            foreach (BoneFixUp boneman in _BoneFixUpCollection)
            {
                bw.Write(boneman.GetBytes());
                flush++;

                if (flush % 96 == 0)
                    bw.Flush();
            }
            bw.Flush();
            #endregion

            #region " Mesh "
            foreach (ModelV14 model in _ModelCollection)
            {
                foreach (ModelInfoV14 info in model.ModelInfoCollection)
                {
                    info.Mesh = new CountOffsetPair(info.MeshCollection.Count, Convert.ToInt32(fs.Position));
                    foreach (MeshV14 mesh in info.MeshCollection)
                    {
                        // Calculate index from/to.
                        ushort lowest = UInt16.MaxValue;
                        ushort highest = UInt16.MinValue;
                        for (int i = mesh.TriangleIndex; i < mesh.TriangleIndex + mesh.TriangleCount; i++)
                        {
                            if (_TriangleMapCollection[i].VertexIndex < lowest)
                                lowest = _TriangleMapCollection[i].VertexIndex;
                            if (_TriangleMapCollection[i].VertexIndex > highest)
                                highest = _TriangleMapCollection[i].VertexIndex;
                        }
                        mesh.VertexIndexFrom = lowest;
                        mesh.VertexIndexTo = highest;
                        bw.Write(mesh.GetBytes());
                    }
                    bw.Flush();
                }
            }
            #endregion

            #region " Model Info "
            foreach (ModelV14 model in _ModelCollection)
            {
                foreach (ModelInfoV14 info in model.ModelInfoCollection)
                {
                    model.ModelInfoOffset[info.SkinReference] = Convert.ToInt32(fs.Position);
                    bw.Write(info.GetBytes());
                }
                bw.Flush();
            }
            StaticMethods.Buffer16(ref bw);
            #endregion

            #region " Model "
            for (int m = 0; m < _ModelCollection.Count; m++)
            {
                header.ModelsOffset[m] = Convert.ToInt32(fs.Position);
                bw.Write(_ModelCollection[m].GetBytes());
                bw.Flush();
            }
            StaticMethods.Buffer16(ref bw);
            #endregion

            #region " Header "
            header.LevelOfDetailFlags = _Header._LevelOfDetailFlags;
            header.BoundingBoxMax = _Header.BoundingBoxMax;
            header.BoundingBoxMin = _Header.BoundingBoxMin;
            header.ClippingBoxMax = _Header.ClippingBoxMax;
            header.ClippingBoxMin = _Header.ClippingBoxMin;
            header.EyePosition = _Header.EyePosition;
            header.FileLength = Convert.ToInt32(fs.Position);
            header.Identifier = "MDLZ";
            header.ModelCount = _ModelCollection.Count;
            header.Name = _Header.Name;
            header.SkinFamilies = _SkinCollection.Families;
            header.SkinReferences = _SkinCollection.References;
            header.TransitionFlagsOffset = _Header.TransitionFlagsOffset;
            header.TransitionsCount = _Header.TransitionsCount;
            header.TransitionsOffset = _Header.TransitionsOffset;
            header.TriangleCount = _TriangleMapCollection.Count;
            header.TypeFlag = _Header.TypeFlag;
            header.UselessOffset = _Header.UselessOffset;
            header.Version = 14;
            header.VertexCount = _VertexCollection.Count;

            fs.Position = 0;
            bw.Write(header.GetBytes());
            #endregion

            int model2 = 0;

            #region " Body Groups "
            fs.Position = header.BodyGroups.Offset;
            foreach (BodyGroupV14 body in _BodyGroupCollection)
            {
                body.ModelOffset = header.ModelsOffset[model2];
                bw.Write(body.GetBytes());
                model2 += body.ModelCount;
            }
            bw.Flush();
            #endregion

            // Close streams
            bw.Close();
            fs.Close();

        }

        public void ReadQce(string file)
        {
            Qce qce = new Qce();
            qce.Read(file);

            // Make sure we have a good version
            if ((qce.QceVersion.Game != QceGame.JamesBond007Nightfire) || (qce.QceVersion.QcVersion != 11) || (qce.QceVersion.QceVersionMajor != 1))
                Messages.ThrowException("Mdl.MdlFileV14.ReadQce(string)", Messages.QCE_VERSION_MISMATCH);

            // Update sequence activities.
            byte blend;
            foreach (CommandReplaceActivity cra in qce.ReplaceActivityCollection)
                _SequenceCollection[FindSequence(cra.SequenceName, out blend)].Activity = (int)Enum.Parse(typeof(ActivityV14), cra.Activity);

            // Handle LOD
            if (qce.LodFlags.Value != 0)
            {
                _Header.LevelOfDetailFlags = (LevelOfDetailFlags)qce.LodFlags.Value;
                _LevelOfDetailCollection = new List<LevelOfDetail>();
                for (int i = 0; i < Enumerators.LodFlagsToInt(_Header.LevelOfDetailFlags); i++)
                    _LevelOfDetailCollection.Add(new LevelOfDetail());
                for (int i = 0; i < qce.LodTempCollection.Count; i++)
                {
                    int[] temp = new int[4];
                    qce.LodTempCollection[i].Distances.CopyTo(temp);
                    _LevelOfDetailCollection[qce.LodTempCollection[i].Index] = new LevelOfDetail(qce.LodTempCollection[i].Distances.Count, temp);
                }
            }
        }

        public static List<short> CompressAnimations(List<short> frames)
        {
            List<short> values = new List<short>();

            int handled = 0;
            while (handled < frames.Count)
            {
                byte total = 0;
                if (frames.Count - handled > 255)
                    total = 255;
                else
                    total = Convert.ToByte(frames.Count - handled);

                List<ValueCountPair> temp = new List<ValueCountPair>();
                temp.Add(new ValueCountPair(frames[handled]));

                int index = 0;
                for (int i = 1; i < total; i++)
                {
                    if (temp[index].Value == frames[i + handled])
                        temp[index].Count++;
                    else
                    {
                        temp.Add(new ValueCountPair(frames[i + handled]));
                        index++;
                    }
                }

                values.Add(0);
                index = values.Count - 1;
                byte valid = 0;
                byte total2 = 0;
                for (int i = 0; i < temp.Count; i++)
                {
                    if ((temp[i].Count > 2) || (temp[i].Count + valid == total))
                    {
                        valid++;
                        values.Add(temp[i].Value);

                        values[index] = new AnimationValue(valid, Convert.ToByte(valid + temp[i].Count - 1)).Value;

                        if (temp[i].Count - 1 + valid < total)
                        {
                            total2 += Convert.ToByte(temp[i].Count + valid - 1);
                            valid = 0;
                            if (total2 != total)
                            {
                                values.Add(0);
                                index = values.Count - 1;
                            }
                        }
                        else
                            total2 = 0;
                    }
                    else
                    {
                        for (int j = 0; j < temp[i].Count; j++)
                        {
                            if (temp[i].Count - 1 + total2 + valid != total)
                            {
                                valid++;
                                values.Add(temp[i].Value);
                            }
                            else
                                values[index] = new AnimationValue(valid, Convert.ToByte(valid + temp[i].Count - 1)).Value;
                        }
                        if (total2 + valid == total)
                            values[index] = new AnimationValue(valid, valid).Value;
                    }
                }


                handled += total;
            }

            return values;
        }
        #endregion

        #region " V10 "
        /// <summary>
        /// Dumps QC and all referenced SMD files.
        /// </summary>
        /// <param name="file">The path to the QC file.</param>
        /// <returns>A list of textures referenced.</returns>
        public string[] DumpV10Qc(string file)
        {
            return DumpV10Qc(file, _Header.Name);
        }
        /// <summary>
        /// Dumps QC and all referenced SMD files.
        /// </summary>
        /// <param name="file">The path to the QC file.</param>
        /// <param name="modelname">Override the $modelname.</param>
        /// <returns>A list of textures referenced.</returns>
        public string[] DumpV10Qc(string file, string modelname)
        {
            List<string> reftext = new List<string>();

            FileInfo fi = new FileInfo(file);
            Directory.CreateDirectory(fi.DirectoryName);
            string name = _Header.Name.Replace(".mdl", "");

            QcFileV10 qc = new QcFileV10();
            Qce qce = new Qce();
            qce.QceVersion = new CommandQceVersion(QceGame.HalfLife, 10, 1, 0);

            #region " Attachment "
            for (int i = 0; i < _AttachmentCollection.Count; i++)
                qc.AttachmentCollection.Add(new CommandAttachment(_AttachmentCollection[i].Name, _BoneCollection[_AttachmentCollection[i].Bone].Name, _AttachmentCollection[i].Position));
            #endregion

            #region " BBox "
            qc.BBox = new CommandBBox(_Header.BoundingBoxMin, _Header.BoundingBoxMax);
            #endregion

            #region " Body Groups "
            foreach (BodyGroupV14 body in _BodyGroupCollection)
            {
                CommandBodyGroup bodygroup = new CommandBodyGroup();
                bodygroup.Name = body.Name;
                for (int i = 0; i < body.ModelCount; i++)
                {
                    byte imodel = FindModel(body.ModelOffset + (i * ModelV14.Length));
                    if (_ModelCollection[imodel].Name == "blank")
                        bodygroup.BodyCollection.Add(new BodyGroupItem("blank"));
                    else
                    {
                        string[] tmptext;
                        bodygroup.BodyCollection.Add(new BodyGroupItem("studio", DumpV10SmdReference(fi.DirectoryName, _ModelCollection[imodel].Name, out tmptext)));
                        foreach (string text in tmptext)
                            reftext.Add(text);
                    }
                }

                qc.BodyGroupCollection.Add(bodygroup);
            }
            #endregion

            #region " CBox "
            qc.CBox = new CommandCBox(_Header.ClippingBoxMin, _Header.ClippingBoxMax);
            #endregion

            #region " CD "
            qc.Cd = new CommandCd(fi.DirectoryName);
            #endregion

            #region " Controller "
            foreach (BoneController control in _BoneControllerCollection)
                qc.ControllerCollection.Add(new CommandController(control.Index, _BoneCollection[control.Bone].Name, control.Type, control.Start, control.End));
            #endregion

            #region " Eye Position "
            qc.EyePosition = new CommandEyePosition(_Header.EyePosition);
            #endregion

            #region " Flags "
            qc.Flags = new CommandFlags(_Header.TypeFlag);
            #endregion

            #region " HBox "
            foreach (HitBox hbox in _HitBoxCollection)
            {
                //if (_BoneCollection[hbox.Bone].VertCount >= 100)
                qc.HBoxCollection.Add(new CommandHBox(hbox.Group, _BoneCollection[hbox.Bone].Name, hbox.Min, hbox.Max));
            }
            #endregion

            #region " Model Name "
            qc.ModelName = new CommandModelName(modelname);
            #endregion

            #region " Origin "
            qc.Origin = new CommandOrigin(new Point3D());
            #endregion

            #region " Root "
            foreach (Bone bone in _BoneCollection)
            {
                if (bone.Parent == -1)
                {
                    qc.Root = new CommandRoot(bone.Name);
                    break;
                }
            }
            #endregion

            #region " Sequence "
            if (_BoneCollection.Count > 0)
            {
                foreach (Sequence seq in _SequenceCollection)
                {
                    CommandSequenceV10 sequence = new CommandSequenceV10();

                    #region " Activity "
                    ActivityV14 act = (ActivityV14)seq.Activity;
                    if (act != 0)
                    {
                        sequence.Activity = new OptionActivityV10((ActivityV10)Enumerators.ConvertActivity(typeof(ActivityV14), typeof(ActivityV10), act), seq.ActivityWeight);
                        if (!sequence.Activity.Activity.ToString().StartsWith("ACT_"))
                        {
                            qce.ReplaceActivityCollection.Add(new CommandReplaceActivity(seq.Name, act.ToString()));
                            sequence.Activity.Activity = ActivityV10.ACT_INVALID;
                        }
                    }
                    #endregion

                    #region " Blend "
                    for (int i = 0; i < seq.BlendCount; i++)
                    {
                        if (seq.BlendType[i] != MotionFlags.None)
                            sequence.BlendCollection.Add(new OptionBlend(seq.BlendType[i], seq.BlendStart[i], seq.BlendEnd[i]));
                    }
                    #endregion

                    #region " Control "
                    sequence.Control.Flags = seq.MotionType;
                    #endregion

                    #region " Events "
                    foreach (Event @event in seq.EventCollection)
                        sequence.EventCollection.Add(new OptionEvent(@event.EventValue, @event.Frame, @event.Options));
                    #endregion

                    #region " Files "
                    sequence.FileCollection.Add(DumpV10SmdAnimation(fi.DirectoryName, seq.Name));
                    for (int i = 1; i < seq.BlendCount; i++)
                        sequence.FileCollection.Add(DumpV10SmdAnimation(fi.DirectoryName, seq.Name + "_blend" + i));
                    #endregion

                    #region " FPS "
                    if (seq.FramesPerSecond != 30)
                        sequence.Fps = new OptionFps(seq.FramesPerSecond);
                    #endregion

                    #region " Loop "
                    if (seq.Flags == SequenceFlags.LOOPING)
                        sequence.Loop = new OptionLoop(true);
                    #endregion

                    #region " Name "
                    sequence.Name = seq.Name;
                    #endregion

                    #region " Node "
                    if (seq.NodeEntry != 0)
                        sequence.Node = new OptionNodeV10(Convert.ToSByte(seq.NodeEntry));
                    #endregion

                    #region " Pivots "
                    for (int i = 0; i < seq.PivotCollection.Length; i++)
                        sequence.PivotCollection.Add(new OptionPivot(i, seq.PivotCollection[i].Start, seq.PivotCollection[i].End));
                    #endregion

                    #region " Transitions "
                    if (seq.NodeExit != 0)
                    {
                        if (seq.NodeFlags == NodeFlags.REVERSE)
                            sequence.RTransition = new OptionRTransition(seq.NodeEntry, seq.NodeExit);
                        else
                            sequence.Transition = new OptionTransition(seq.NodeEntry, seq.NodeExit);
                    }
                    #endregion

                    qc.SequenceCollection.Add(sequence);
                }
            }
            #endregion

            #region " Texture Group "
            if (_SkinCollection.Families > 1)
            {
                CommandTextureGroup texturegroup = new CommandTextureGroup();
                texturegroup.Name = "Skins";
                for (int f = 0; f < _SkinCollection.Families; f++)
                {
                    List<string> skins = new List<string>();
                    for (int r = 0; r < _SkinCollection.References; r++)
                        skins.Add(_TextureCollection[_SkinCollection.Skins[f, r]].TextureName);
                    texturegroup.SkinCollection.Add(skins);
                }
            }
            #endregion

            qc.Write(file);
            qce.Write(fi.DirectoryName + "\\" + name + ".qce");

            return StaticMethods.EliminateDuplicates(reftext.ToArray());
        }
        /// <summary>
        /// Dumps a reference SMD found in this V14 model.
        /// </summary>
        /// <param name="path">The directory to save the file in.</param>
        /// <param name="model">The model to dump.  Do not use an extension here.</param>
        /// <param name="textures">Returns an array of texture names referenced by the SMD.</param>
        /// <returns>The name of the file created that is QC friendly (no extension).</returns>
        public string DumpV10SmdReference(string path, string model, out string[] textures)
        {
            SmdFileV10 smd = new SmdFileV10(SmdType.Reference);

            byte imodel = FindModel(model);
            foreach (ModelInfoV14 mi in _ModelCollection[imodel].ModelInfoCollection)
            {
                foreach (MeshV14 mesh in mi.MeshCollection)
                {
                    for (int j = 0; j < mesh.TriangleCount; j += 3)
                    {
                        List<VertexV10> vertices = new List<VertexV10>();

                        VertexV10 vert = new VertexV10();
                        ushort ivertex = _TriangleMapCollection[mesh.TriangleIndex + j + 1].VertexIndex;
                        vert.Position = _VertexCollection[ivertex].Position;
                        vert.Normal = _NormalCollection[ivertex].Position;
                        vert.Bone = BlendsToBones(mesh.BoneMappings, ivertex)[0];
                        vert.TextureCoordinate = new TextureCoordinate(_TextureCoordinateCollection[ivertex].U, _TextureCoordinateCollection[ivertex].V * -1);
                        vertices.Add(vert);

                        vert = new VertexV10();
                        ivertex = _TriangleMapCollection[mesh.TriangleIndex + j].VertexIndex;
                        vert.Position = _VertexCollection[ivertex].Position;
                        vert.Normal = _NormalCollection[ivertex].Position;
                        vert.Bone = BlendsToBones(mesh.BoneMappings, ivertex)[0];
                        vert.TextureCoordinate = new TextureCoordinate(_TextureCoordinateCollection[ivertex].U, _TextureCoordinateCollection[ivertex].V * -1);
                        vertices.Add(vert);

                        vert = new VertexV10();
                        ivertex = _TriangleMapCollection[mesh.TriangleIndex + j + 2].VertexIndex;
                        vert.Position = _VertexCollection[ivertex].Position;
                        vert.Normal = _NormalCollection[ivertex].Position;
                        vert.Bone = BlendsToBones(mesh.BoneMappings, ivertex)[0];
                        vert.TextureCoordinate = new TextureCoordinate(_TextureCoordinateCollection[ivertex].U, _TextureCoordinateCollection[ivertex].V * -1);
                        vertices.Add(vert);

                        smd.TriangleCollection.Add(new Triangle<VertexV10>(_TextureCollection[mi.SkinReference].TextureName, vertices));
                    }
                }
            }

            List<Time> times = new List<Time>();
            for (int i = 0; i < _BoneCollection.Count; i++)
            {
                smd.NodeCollection.Add(new Node(Convert.ToSByte(i), _BoneCollection[i].Name, Convert.ToSByte(_BoneCollection[i].Parent)));
                times.Add(new Time(i, _BoneCollection[i].Position, _BoneCollection[i].Rotation));
            }
            smd.TimeCollection.Add(new Frame(0, times));

            textures = smd.GetTextures();

            smd.Write(path.TrimEnd('\\') + " \\" + StaticMethods.GoodFileName(model) + STRING_REF + ".smd");
            return StaticMethods.GoodFileName(model) + STRING_REF;
        }
        /// <summary>
        /// Dumps a animation SMD found in this V14 model.
        /// </summary>
        /// <param name="path">The directory to save the file in.</param>
        /// <param name="sequence">The sequence to dump.  Do not use an extension here.</param>
        /// <returns>The name of the file created that is QC friendly (no extension).</returns>
        public string DumpV10SmdAnimation(string path, string sequence)
        {
            SmdFileV10 smd = new SmdFileV10(SmdType.Animation);

            // Add nodes
            List<Time> @default = new List<Time>();
            for (int i = 0; i < _BoneCollection.Count; i++)
            {
                smd.NodeCollection.Add(new Node(Convert.ToSByte(i), _BoneCollection[i].Name, Convert.ToSByte(_BoneCollection[i].Parent)));
                @default.Add(new Time(i, _BoneCollection[i].Position, _BoneCollection[i].Rotation));
            }

            byte blend;
            int iSequence = FindSequence(sequence, out blend);

            for (int f = 0; f < _SequenceCollection[iSequence].FrameCount; f++)
            {
                List<Time> times = new List<Time>();
                for (int b = 0; b < _Header.Bones.Count; b++)
                {
                    Point3D Pos = new Point3D(_BoneCollection[b].Position);
                    Point3D Rot = new Point3D(_BoneCollection[b].Rotation);


                    for (int t = 0; t < 6; t++)
                    {
                        if (_SequenceCollection[iSequence].AnimationCollection[blend, b, t] != null)
                        {
                            switch (t)
                            {
                                case 0:
                                    Pos.X += _SequenceCollection[iSequence].AnimationCollection[blend, b, t][f] * _BoneCollection[b].ScalePosition.X;
                                    break;
                                case 1:
                                    Pos.Y += _SequenceCollection[iSequence].AnimationCollection[blend, b, t][f] * _BoneCollection[b].ScalePosition.Y;
                                    break;
                                case 2:
                                    Pos.Z += _SequenceCollection[iSequence].AnimationCollection[blend, b, t][f] * _BoneCollection[b].ScalePosition.Z;
                                    break;
                                case 3:
                                    Rot.X += _SequenceCollection[iSequence].AnimationCollection[blend, b, t][f] * _BoneCollection[b].ScaleRotation.X;
                                    break;
                                case 4:
                                    Rot.Y += _SequenceCollection[iSequence].AnimationCollection[blend, b, t][f] * _BoneCollection[b].ScaleRotation.Y;
                                    break;
                                case 5:
                                    Rot.Z += _SequenceCollection[iSequence].AnimationCollection[blend, b, t][f] * _BoneCollection[b].ScaleRotation.Z;
                                    break;
                            }
                        }
                    }

                    // Force linear movement...
                    if ((_BoneCollection[b].Parent == -1) && (_SequenceCollection[iSequence].FrameCount > 1))
                    {
                        Pos.X += f * _SequenceCollection[iSequence].LinearMovement[0] / (_SequenceCollection[iSequence].FrameCount - 1);
                        Pos.Y += f * _SequenceCollection[iSequence].LinearMovement[1] / (_SequenceCollection[iSequence].FrameCount - 1);
                        Pos.Z += f * _SequenceCollection[iSequence].LinearMovement[2] / (_SequenceCollection[iSequence].FrameCount - 1);
                    }

                    times.Add(new Time(b, Pos, Rot));
                }
                smd.TimeCollection.Add(new Frame(f, times));
            }

            if (smd.TimeCollection.Count == 0)
                smd.TimeCollection.Add(new Frame(0, @default));

            smd.Rotate(-90);
            smd.Write(path.TrimEnd('\\') + " \\" + StaticMethods.GoodFileName(sequence) + STRING_ANI + ".smd");
            return StaticMethods.GoodFileName(sequence) + STRING_ANI;
        }
        #endregion

        #region " V11 "
        /// <summary>
        /// Dumps QC and all referenced SMD files.
        /// </summary>
        /// <param name="file">The path to the QC file.</param>
        /// <returns>A list of textures referenced.</returns>
        public string[] DumpV11Qc(string file)
        {
            return DumpV11Qc(file, _Header.Name);
        }
        /// <summary>
        /// Dumps QC and all referenced SMD files.
        /// </summary>
        /// <param name="file">The path to the QC file.</param>
        /// <param name="modelname">Override the $modelname.</param>
        /// <returns>A list of textures referenced.</returns>
        public string[] DumpV11Qc(string file, string modelname)
        {
            List<string> reftext = new List<string>();

            FileInfo fi = new FileInfo(file);
            Directory.CreateDirectory(fi.DirectoryName);
            string name = _Header.Name.Replace(".mdl", "");

            QcFileV11 qc = new QcFileV11();
            Qce qce = new Qce();
            /*
             * 1.0  New
             * 1.1  Added $lodflags and $lodtemp
             */
            qce.QceVersion = new CommandQceVersion(QceGame.JamesBond007Nightfire, 11, 1, 1);

            #region " Attachment "
            for (int i = 0; i < _AttachmentCollection.Count; i++)
                qc.AttachmentCollection.Add(new CommandAttachment(_AttachmentCollection[i].Name, _BoneCollection[_AttachmentCollection[i].Bone].Name, _AttachmentCollection[i].Position));
            #endregion

            #region " BBox "
            qc.BBox = new CommandBBox(_Header.BoundingBoxMin, _Header.BoundingBoxMax);
            #endregion

            #region " Body Groups "
            // To "repair" airfield_radar2.mdl and missile_container.mdl
            if (_BodyGroupCollection.Count == 0)
            {
                _BodyGroupCollection.Add(new BodyGroupV14("studio", 1, _Header.ModelsOffset[0]));
            }

            foreach (BodyGroupV14 body in _BodyGroupCollection)
            {
                CommandBodyGroup bodygroup = new CommandBodyGroup();
                bodygroup.Name = body.Name;
                for (int i = 0; i < body.ModelCount; i++)
                {
                    byte imodel = FindModel(body.ModelOffset + (i * ModelV14.Length));
                    if (_ModelCollection[imodel].Name == "blank")
                        bodygroup.BodyCollection.Add(new BodyGroupItem("blank"));
                    else
                    {
                        string[] tmptext;
                        bodygroup.BodyCollection.Add(new BodyGroupItem("studio", DumpV11SmdReference(fi.DirectoryName, _ModelCollection[imodel].Name, out tmptext)));
                        foreach (string text in tmptext)
                            reftext.Add(text);
                    }
                }

                qc.BodyGroupCollection.Add(bodygroup);
            }
            #endregion

            #region " CBox "
            qc.CBox = new CommandCBox(_Header.ClippingBoxMin, _Header.ClippingBoxMax);
            #endregion

            #region " Controller "
            foreach (BoneController control in _BoneControllerCollection)
                qc.ControllerCollection.Add(new CommandController(control.Index, _BoneCollection[control.Bone].Name, control.Type, control.Start, control.End));
            #endregion

            #region " Eye Position "
            qc.EyePosition = new CommandEyePosition(_Header.EyePosition);
            #endregion

            #region " Flags "
            qc.Flags = new CommandFlags(_Header.TypeFlag);
            #endregion

            #region " HBox "
            foreach (HitBox hbox in _HitBoxCollection)
            {
                //if (_BoneCollection[hbox.Bone].VertCount >= 100)
                qc.HBoxCollection.Add(new CommandHBox(hbox.Group, _BoneCollection[hbox.Bone].Name, hbox.Min, hbox.Max));
            }
            #endregion

            #region " Level of Detail "
            qce.LodFlags = new CommandLodFlags((int)_Header.LevelOfDetailFlags);

            for (int i = 0; i < _LevelOfDetailCollection.Count; i++)
            {
                if (_LevelOfDetailCollection[i].Levels != 0)
                {
                    CommandLodTemp lodtemp = new CommandLodTemp();
                    lodtemp.Index = i;
                    for (int j = 0; j < _LevelOfDetailCollection[i].Levels; j++)
                        lodtemp.Distances.Add(_LevelOfDetailCollection[i].Distance[j]);
                    qce.LodTempCollection.Add(lodtemp);
                }
            }
            #endregion

            #region " Model Name "
            qc.ModelName = new CommandModelName(modelname);
            #endregion

            #region " Origin "
            qc.Origin = new CommandOrigin(new Point3D());
            #endregion

            #region " Root "
            foreach (Bone bone in _BoneCollection)
            {
                if (bone.Parent == -1)
                {
                    qc.Root = new CommandRoot(bone.Name);
                    break;
                }
            }
            #endregion

            #region " Sequence "
            if (_BoneCollection.Count > 0)
            {
                foreach (Sequence seq in _SequenceCollection)
                {
                    CommandSequenceV11 sequence = new CommandSequenceV11();

                    #region " Activity "
                    ActivityV14 act = (ActivityV14)seq.Activity;
                    if (act != 0)
                    {
                        sequence.Activity = new OptionActivityV11((ActivityV11)Enumerators.ConvertActivity(typeof(ActivityV14), typeof(ActivityV11), act), seq.ActivityWeight);
                        if (!sequence.Activity.Activity.ToString().StartsWith("ACT_"))
                        {
                            qce.ReplaceActivityCollection.Add(new CommandReplaceActivity(seq.Name, act.ToString()));
                            sequence.Activity.Activity = ActivityV11.ACT_IDLE;
                        }
                    }
                    #endregion

                    #region " Blend "
                    for (int i = 0; i < seq.BlendCount; i++)
                    {
                        if (seq.BlendType[i] != MotionFlags.None)
                            sequence.BlendCollection.Add(new OptionBlend(seq.BlendType[i], seq.BlendStart[i], seq.BlendEnd[i]));
                    }
                    #endregion

                    #region " Control "
                    sequence.Control.Flags = seq.MotionType;
                    #endregion

                    #region " Events "
                    foreach (Event @event in seq.EventCollection)
                        sequence.EventCollection.Add(new OptionEvent(@event.EventValue, @event.Frame, @event.Options));
                    #endregion

                    #region " Files "
                    sequence.FileCollection.Add(DumpV11SmdAnimation(fi.DirectoryName, seq.Name));
                    for (int i = 1; i < seq.BlendCount; i++)
                        sequence.FileCollection.Add(DumpV11SmdAnimation(fi.DirectoryName, seq.Name + "_blend" + i));
                    #endregion

                    #region " FPS "
                    if (seq.FramesPerSecond != 30)
                        sequence.Fps = new OptionFps(seq.FramesPerSecond);
                    #endregion

                    #region " Loop "
                    if (seq.Flags == SequenceFlags.LOOPING)
                        sequence.Loop = new OptionLoop(true);
                    #endregion

                    #region " Name "
                    sequence.Name = seq.Name;
                    #endregion

                    #region " Node "
                    if (seq.NodeEntry != 0)
                        sequence.Node = new OptionNodeV10(Convert.ToSByte(seq.NodeEntry));
                    #endregion

                    #region " Pivots "
                    for (int i = 0; i < seq.PivotCollection.Length; i++)
                        sequence.PivotCollection.Add(new OptionPivot(i, seq.PivotCollection[i].Start, seq.PivotCollection[i].End));
                    #endregion

                    #region " Transitions "
                    if (seq.NodeExit != 0)
                    {
                        if (seq.NodeFlags == NodeFlags.REVERSE)
                            sequence.RTransition = new OptionRTransition(seq.NodeEntry, seq.NodeExit);
                        else
                            sequence.Transition = new OptionTransition(seq.NodeEntry, seq.NodeExit);
                    }
                    #endregion

                    qc.SequenceCollection.Add(sequence);
                }
            }
            #endregion

            #region " Sound Group "
            foreach (SoundGroup group in _SoundGroupCollection)
            {
                CommandSoundGroup soundgroup = new CommandSoundGroup();
                soundgroup.Name = group.Name;
                foreach (string sound in group.Sounds.Names)
                    soundgroup.Sounds.Add(sound);
                qc.SoundGroupCollection.Add(soundgroup);
            }
            #endregion

            #region " Texture "
            foreach (Texture tex in _TextureCollection)
                qc.TextureCollection.Add(new CommandTexture(tex.TextureName, tex.MaterialName));
            #endregion

            #region " Texture Group "
            if (_SkinCollection.Families > 1)
            {
                CommandTextureGroup texturegroup = new CommandTextureGroup();
                texturegroup.Name = "Skins";
                for (int f = 0; f < _SkinCollection.Families; f++)
                {
                    List<string> skins = new List<string>();
                    for (int r = 0; r < _SkinCollection.References; r++)
                        skins.Add(_TextureCollection[_SkinCollection.Skins[f, r]].TextureName);
                    texturegroup.SkinCollection.Add(skins);
                }
            }
            #endregion

            qc.Write(file);
            qce.Write(fi.DirectoryName + "\\" + name + ".qce");

            return StaticMethods.EliminateDuplicates(reftext.ToArray());
        }
        /// <summary>
        /// Dumps a reference SMD found in this V14 model.
        /// </summary>
        /// <param name="path">The directory to save the file in.</param>
        /// <param name="model">The model to dump.  Do not use an extension here.</param>
        /// <param name="textures">Returns an array of texture names referenced by the SMD.</param>
        /// <returns>The name of the file created that is QC friendly (no extension).</returns>
        public string DumpV11SmdReference(string path, string model, out string[] textures)
        {
            SmdFileV11 smd = new SmdFileV11(SmdType.Reference);

            byte imodel = FindModel(model);
            foreach (ModelInfoV14 mi in _ModelCollection[imodel].ModelInfoCollection)
            {
                foreach (MeshV14 mesh in mi.MeshCollection)
                {
                    for (int j = 0; j < mesh.TriangleCount; j += 3)
                    {
                        List<VertexV11> vertices = new List<VertexV11>();

                        VertexV11 vert = new VertexV11();
                        ushort ivertex = _TriangleMapCollection[mesh.TriangleIndex + j + 1].VertexIndex;
                        vert.Blends = BlendsToBlending(mesh.BoneMappings, ivertex);
                        vert.Position = _VertexCollection[ivertex].Position;
                        vert.Normal = _NormalCollection[ivertex].Position;
                        vert.TextureCoordinate = new TextureCoordinate(_TextureCoordinateCollection[ivertex].U, _TextureCoordinateCollection[ivertex].V * -1);
                        vertices.Add(vert);

                        vert = new VertexV11();
                        ivertex = _TriangleMapCollection[mesh.TriangleIndex + j].VertexIndex;
                        vert.Blends = BlendsToBlending(mesh.BoneMappings, ivertex);
                        vert.Position = _VertexCollection[ivertex].Position;
                        vert.Normal = _NormalCollection[ivertex].Position;
                        vert.TextureCoordinate = new TextureCoordinate(_TextureCoordinateCollection[ivertex].U, _TextureCoordinateCollection[ivertex].V * -1);
                        vertices.Add(vert);

                        vert = new VertexV11();
                        ivertex = _TriangleMapCollection[mesh.TriangleIndex + j + 2].VertexIndex;
                        vert.Blends = BlendsToBlending(mesh.BoneMappings, ivertex);
                        vert.Position = _VertexCollection[ivertex].Position;
                        vert.Normal = _NormalCollection[ivertex].Position;
                        vert.TextureCoordinate = new TextureCoordinate(_TextureCoordinateCollection[ivertex].U, _TextureCoordinateCollection[ivertex].V * -1);
                        vertices.Add(vert);

                        smd.TriangleCollection.Add(new Triangle<VertexV11>(_TextureCollection[mi.SkinReference].TextureName, vertices));
                    }
                }
            }

            List<Time> times = new List<Time>();
            for (int i = 0; i < _BoneCollection.Count; i++)
            {
                smd.NodeCollection.Add(new Node(Convert.ToSByte(i), _BoneCollection[i].Name, Convert.ToSByte(_BoneCollection[i].Parent)));
                times.Add(new Time(i, _BoneCollection[i].Position, _BoneCollection[i].Rotation));
            }
            smd.TimeCollection.Add(new Frame(0, times));

            textures = smd.GetTextures();

            smd.Write(path.TrimEnd('\\') + " \\" + StaticMethods.GoodFileName(model) + STRING_REF + ".smd");
            return StaticMethods.GoodFileName(model) + STRING_REF;
        }
        /// <summary>
        /// Dumps a animation SMD found in this V14 model.
        /// </summary>
        /// <param name="path">The directory to save the file in.</param>
        /// <param name="sequence">The sequence to dump.  Do not use an extension here.</param>
        /// <returns>The name of the file created that is QC friendly (no extension).</returns>
        public string DumpV11SmdAnimation(string path, string sequence)
        {
            SmdFileV11 smd = new SmdFileV11(SmdType.Animation);

            // Add nodes
            List<Time> @default = new List<Time>();
            for (int i = 0; i < _BoneCollection.Count; i++)
            {
                smd.NodeCollection.Add(new Node(Convert.ToSByte(i), _BoneCollection[i].Name, Convert.ToSByte(_BoneCollection[i].Parent)));
                @default.Add(new Time(i, _BoneCollection[i].Position, _BoneCollection[i].Rotation));
            }

            byte blend;
            int iSequence = FindSequence(sequence, out blend);

            for (int f = 0; f < _SequenceCollection[iSequence].FrameCount; f++)
            {
                List<Time> times = new List<Time>();
                for (int b = 0; b < _Header.Bones.Count; b++)
                {
                    Point3D Pos = new Point3D(_BoneCollection[b].Position);
                    Point3D Rot = new Point3D(_BoneCollection[b].Rotation);


                    for (int t = 0; t < 6; t++)
                    {
                        if (_SequenceCollection[iSequence].AnimationCollection[blend, b, t] != null)
                        {
                            switch (t)
                            {
                                case 0:
                                    Pos.X += _SequenceCollection[iSequence].AnimationCollection[blend, b, t][f] * _BoneCollection[b].ScalePosition.X;
                                    break;
                                case 1:
                                    Pos.Y += _SequenceCollection[iSequence].AnimationCollection[blend, b, t][f] * _BoneCollection[b].ScalePosition.Y;
                                    break;
                                case 2:
                                    Pos.Z += _SequenceCollection[iSequence].AnimationCollection[blend, b, t][f] * _BoneCollection[b].ScalePosition.Z;
                                    break;
                                case 3:
                                    Rot.X += _SequenceCollection[iSequence].AnimationCollection[blend, b, t][f] * _BoneCollection[b].ScaleRotation.X;
                                    break;
                                case 4:
                                    Rot.Y += _SequenceCollection[iSequence].AnimationCollection[blend, b, t][f] * _BoneCollection[b].ScaleRotation.Y;
                                    break;
                                case 5:
                                    Rot.Z += _SequenceCollection[iSequence].AnimationCollection[blend, b, t][f] * _BoneCollection[b].ScaleRotation.Z;
                                    break;
                            }
                        }
                    }

                    // Force linear movement...
                    if ((_BoneCollection[b].Parent == -1) && (_SequenceCollection[iSequence].FrameCount > 1))
                    {
                        Pos.X += f * _SequenceCollection[iSequence].LinearMovement[0] / (_SequenceCollection[iSequence].FrameCount - 1);
                        Pos.Y += f * _SequenceCollection[iSequence].LinearMovement[1] / (_SequenceCollection[iSequence].FrameCount - 1);
                        Pos.Z += f * _SequenceCollection[iSequence].LinearMovement[2] / (_SequenceCollection[iSequence].FrameCount - 1);
                    }

                    times.Add(new Time(b, Pos, Rot));
                }
                smd.TimeCollection.Add(new Frame(f, times));
            }

            if (smd.TimeCollection.Count == 0)
                smd.TimeCollection.Add(new Frame(0, @default));

            smd.Rotate(-90);
            smd.Write(path.TrimEnd('\\') + " \\" + StaticMethods.GoodFileName(sequence) + STRING_ANI + ".smd");
            return StaticMethods.GoodFileName(sequence) + STRING_ANI;
        }
        #endregion

        #region " V44 "
        /// <summary>
        /// Dumps QC and all referenced SMD files.
        /// </summary>
        /// <param name="file">The path to the QC file.</param>
        /// <returns>A list of textures referenced.</returns>
        public string[] DumpV44Qc(string file)
        {
            return DumpV44Qc(file, _Header.Name);
        }
        /// <summary>
        /// Dumps QC and all referenced SMD files.
        /// </summary>
        /// <param name="file">The path to the QC file.</param>
        /// <param name="modelname">Override the $modelname.</param>
        /// <returns>A list of textures referenced.</returns>
        public string[] DumpV44Qc(string file, string modelname)
        {
            List<string> reftext = new List<string>();

            FileInfo fi = new FileInfo(file);
            Directory.CreateDirectory(fi.DirectoryName);
            string name = _Header.Name.Replace(".mdl", "");

            QcFileV44 qc = new QcFileV44();
            Qce qce = new Qce();
            /*
             * 1.0  New
             */
            qce.QceVersion = new CommandQceVersion(QceGame.HalfLife2, 44, 1, 0);

            #region " Attachment "
            for (int i = 0; i < _AttachmentCollection.Count; i++)
                qc.AttachmentCollection.Add(new CommandAttachment(_AttachmentCollection[i].Name, _BoneCollection[_AttachmentCollection[i].Bone].Name, _AttachmentCollection[i].Position));
            #endregion

            #region " BBox "
            qc.BBox = new CommandBBox(_Header.BoundingBoxMin, _Header.BoundingBoxMax);
            #endregion

            #region " Body Groups "
            // To "repair" airfield_radar2.mdl and missile_container.mdl
            if (_BodyGroupCollection.Count == 0)
            {
                _BodyGroupCollection.Add(new BodyGroupV14("studio", 1, _Header.ModelsOffset[0]));
            }

            foreach (BodyGroupV14 body in _BodyGroupCollection)
            {

                CommandBodyGroup bodygroup = new CommandBodyGroup();
                bodygroup.Name = body.Name;
                for (int i = 0; i < body.ModelCount; i++)
                {
                    byte imodel = FindModel(body.ModelOffset + (i * ModelV14.Length));
                    if (_ModelCollection[imodel].Name == "blank")
                        bodygroup.BodyCollection.Add(new BodyGroupItem("blank"));
                    else
                    {
                        string[] tmptext;
                        bodygroup.BodyCollection.Add(new BodyGroupItem("studio", DumpV44SmdReference(fi.DirectoryName, _ModelCollection[imodel].Name, out tmptext)));
                        foreach (string text in tmptext)
                            reftext.Add(text);
                    }
                }

                qc.BodyGroupCollection.Add(bodygroup);
            }
            #endregion

            #region " CBox "
            qc.CBox = new CommandCBox(_Header.ClippingBoxMin, _Header.ClippingBoxMax);
            #endregion

            #region " Controller "
            foreach (BoneController control in _BoneControllerCollection)
                qc.ControllerCollection.Add(new CommandController(control.Index, _BoneCollection[control.Bone].Name, control.Type, control.Start, control.End));
            #endregion

            #region " Eye Position "
            qc.EyePosition = new CommandEyePosition(_Header.EyePosition);
            #endregion

            #region " Flags "
            qc.Flags = new CommandFlags(_Header.TypeFlag);
            #endregion

            #region " HBox "
            foreach (HitBox hbox in _HitBoxCollection)
            {
                //if (_BoneCollection[hbox.Bone].VertCount >= 100)
                qc.HBoxCollection.Add(new CommandHBox(hbox.Group, _BoneCollection[hbox.Bone].Name, hbox.Min, hbox.Max));
            }
            #endregion

            #region " Level of Detail "
            qce.LodFlags = new CommandLodFlags((int)_Header.LevelOfDetailFlags);

            for (int i = 0; i < _LevelOfDetailCollection.Count; i++)
            {
                if (_LevelOfDetailCollection[i].Levels != 0)
                {
                    CommandLodTemp lodtemp = new CommandLodTemp();
                    lodtemp.Index = i;
                    for (int j = 0; j < _LevelOfDetailCollection[i].Levels; j++)
                        lodtemp.Distances.Add(_LevelOfDetailCollection[i].Distance[j]);
                    qce.LodTempCollection.Add(lodtemp);
                }
            }
            #endregion

            #region " Model Name "
            qc.ModelName = new CommandModelName(modelname);
            #endregion

            #region " Origin "
            qc.Origin = new CommandOrigin(new Point3D());
            #endregion

            #region " Root "
            foreach (Bone bone in _BoneCollection)
            {
                if (bone.Parent == -1)
                {
                    qc.Root = new CommandRoot(bone.Name);
                    break;
                }
            }
            #endregion

            #region " Sequence "
            if (_BoneCollection.Count > 0)
            {
                foreach (Sequence seq in _SequenceCollection)
                {
                    CommandSequenceV44 sequence = new CommandSequenceV44();

                    #region " Activity "
                    ActivityV14 act = (ActivityV14)seq.Activity;
                    if (act != 0)
                    {
                        sequence.Activity = new OptionActivityV44((ActivityV44)Enumerators.ConvertActivity(typeof(ActivityV14), typeof(ActivityV44), act), seq.ActivityWeight);
                        if (!sequence.Activity.Activity.ToString().StartsWith("ACT_"))
                        {
                            qce.ReplaceActivityCollection.Add(new CommandReplaceActivity(seq.Name, act.ToString()));
                            sequence.Activity.Activity = ActivityV44.ACT_IDLE;
                        }
                    }
                    #endregion

                    #region " Blend "
                    for (int i = 0; i < seq.BlendCount; i++)
                    {
                        if (seq.BlendType[i] != MotionFlags.None)
                            sequence.BlendCollection.Add(new OptionBlend(seq.BlendType[i], seq.BlendStart[i], seq.BlendEnd[i]));
                    }
                    #endregion

                    #region " Control "
                    sequence.Control.Flags = seq.MotionType;
                    #endregion

                    #region " Events "
                    foreach (Event @event in seq.EventCollection)
                        sequence.EventCollection.Add(new OptionEvent(@event.EventValue, @event.Frame, @event.Options));
                    #endregion

                    #region " Files "
                    sequence.FileCollection.Add(DumpV11SmdAnimation(fi.DirectoryName, seq.Name));
                    for (int i = 1; i < seq.BlendCount; i++)
                        sequence.FileCollection.Add(DumpV44SmdAnimation(fi.DirectoryName, seq.Name + "_blend" + i));
                    #endregion

                    #region " FPS "
                    if (seq.FramesPerSecond != 30)
                        sequence.Fps = new OptionFps(seq.FramesPerSecond);
                    #endregion

                    #region " Loop "
                    if (seq.Flags == SequenceFlags.LOOPING)
                        sequence.Loop = new OptionLoop(true);
                    #endregion

                    #region " Name "
                    sequence.Name = seq.Name;
                    #endregion

                    #region " Node "
                    if (seq.NodeEntry != 0)
                        sequence.Node = new OptionNodeV44(SmdFile.GetNode(StaticMethods.SmdFile, Convert.ToSByte(seq.NodeEntry)));
                    #endregion

                    #region " Pivots "
                    for (int i = 0; i < seq.PivotCollection.Length; i++)
                        sequence.PivotCollection.Add(new OptionPivot(i, seq.PivotCollection[i].Start, seq.PivotCollection[i].End));
                    #endregion

                    #region " Transitions "
                    if (seq.NodeExit != 0)
                    {
                        if (seq.NodeFlags == NodeFlags.REVERSE)
                            sequence.RTransition = new OptionRTransition(seq.NodeEntry, seq.NodeExit);
                        else
                            sequence.Transition = new OptionTransition(seq.NodeEntry, seq.NodeExit);
                    }
                    #endregion

                    qc.SequenceCollection.Add(sequence);
                }
            }
            #endregion

            #region " Texture Group "
            if (_SkinCollection.Families > 1)
            {
                CommandTextureGroup texturegroup = new CommandTextureGroup();
                texturegroup.Name = "Skins";
                for (int f = 0; f < _SkinCollection.Families; f++)
                {
                    List<string> skins = new List<string>();
                    for (int r = 0; r < _SkinCollection.References; r++)
                        skins.Add(_TextureCollection[_SkinCollection.Skins[f, r]].TextureName);
                    texturegroup.SkinCollection.Add(skins);
                }
            }
            #endregion

            qc.Write(file);
            qce.Write(fi.DirectoryName + "\\" + name + ".qce");

            return StaticMethods.EliminateDuplicates(reftext.ToArray());
        }
        /// <summary>
        /// Dumps a reference SMD found in this V14 model.
        /// </summary>
        /// <param name="path">The directory to save the file in.</param>
        /// <param name="model">The model to dump.  Do not use an extension here.</param>
        /// <param name="textures">Returns an array of texture names referenced by the SMD.</param>
        /// <returns>The name of the file created that is QC friendly (no extension).</returns>
        public string DumpV44SmdReference(string path, string model, out string[] textures)
        {
            SmdFileV44 smd = new SmdFileV44(SmdType.Reference);

            byte imodel = FindModel(model);
            foreach (ModelInfoV14 mi in _ModelCollection[imodel].ModelInfoCollection)
            {
                foreach (MeshV14 mesh in mi.MeshCollection)
                {
                    for (int j = 0; j < mesh.TriangleCount; j += 3)
                    {
                        List<VertexV44> vertices = new List<VertexV44>();

                        VertexV44 vert = new VertexV44();
                        ushort ivertex = _TriangleMapCollection[mesh.TriangleIndex + j + 1].VertexIndex;
                        vert.LinkCollection = BlendsToLinks(mesh.BoneMappings, ivertex);
                        vert.Position = _VertexCollection[ivertex].Position;
                        vert.Normal = _NormalCollection[ivertex].Position;
                        vert.TextureCoordinate = new TextureCoordinate(_TextureCoordinateCollection[ivertex].U, _TextureCoordinateCollection[ivertex].V * -1);
                        vertices.Add(vert);

                        vert = new VertexV44();
                        ivertex = _TriangleMapCollection[mesh.TriangleIndex + j].VertexIndex;
                        vert.LinkCollection = BlendsToLinks(mesh.BoneMappings, ivertex);
                        vert.Position = _VertexCollection[ivertex].Position;
                        vert.Normal = _NormalCollection[ivertex].Position;
                        vert.TextureCoordinate = new TextureCoordinate(_TextureCoordinateCollection[ivertex].U, _TextureCoordinateCollection[ivertex].V * -1);
                        vertices.Add(vert);

                        vert = new VertexV44();
                        ivertex = _TriangleMapCollection[mesh.TriangleIndex + j + 2].VertexIndex;
                        vert.LinkCollection = BlendsToLinks(mesh.BoneMappings, ivertex);
                        vert.Position = _VertexCollection[ivertex].Position;
                        vert.Normal = _NormalCollection[ivertex].Position;
                        vert.TextureCoordinate = new TextureCoordinate(_TextureCoordinateCollection[ivertex].U, _TextureCoordinateCollection[ivertex].V * -1);
                        vertices.Add(vert);

                        smd.TriangleCollection.Add(new Triangle<VertexV44>(_TextureCollection[mi.SkinReference].TextureName, vertices));
                    }
                }
            }

            List<Time> times = new List<Time>();
            for (int i = 0; i < _BoneCollection.Count; i++)
            {
                smd.NodeCollection.Add(new Node(Convert.ToSByte(i), _BoneCollection[i].Name, Convert.ToSByte(_BoneCollection[i].Parent)));
                times.Add(new Time(i, _BoneCollection[i].Position, _BoneCollection[i].Rotation));
            }
            smd.TimeCollection.Add(new Frame(0, times));

            textures = smd.GetTextures();

            smd.Write(path.TrimEnd('\\') + " \\" + StaticMethods.GoodFileName(model) + STRING_REF + ".smd");

            // Make sure we can find one if needed.
            StaticMethods.SmdFile = path.TrimEnd('\\') + " \\" + StaticMethods.GoodFileName(model) + STRING_REF + ".smd";

            return StaticMethods.GoodFileName(model) + STRING_REF;
        }
        /// <summary>
        /// Dumps a animation SMD found in this V14 model.
        /// </summary>
        /// <param name="path">The directory to save the file in.</param>
        /// <param name="sequence">The sequence to dump.  Do not use an extension here.</param>
        /// <returns>The name of the file created that is QC friendly (no extension).</returns>
        public string DumpV44SmdAnimation(string path, string sequence)
        {
            SmdFileV44 smd = new SmdFileV44(SmdType.Animation);

            // Add nodes
            List<Time> @default = new List<Time>();
            for (int i = 0; i < _BoneCollection.Count; i++)
            {
                smd.NodeCollection.Add(new Node(Convert.ToSByte(i), _BoneCollection[i].Name, Convert.ToSByte(_BoneCollection[i].Parent)));
                @default.Add(new Time(i, _BoneCollection[i].Position, _BoneCollection[i].Rotation));
            }

            byte blend;
            int iSequence = FindSequence(sequence, out blend);

            for (int f = 0; f < _SequenceCollection[iSequence].FrameCount; f++)
            {
                List<Time> times = new List<Time>();
                for (int b = 0; b < _Header.Bones.Count; b++)
                {
                    Point3D Pos = new Point3D(_BoneCollection[b].Position);
                    Point3D Rot = new Point3D(_BoneCollection[b].Rotation);


                    for (int t = 0; t < 6; t++)
                    {
                        if (_SequenceCollection[iSequence].AnimationCollection[blend, b, t] != null)
                        {
                            switch (t)
                            {
                                case 0:
                                    Pos.X += _SequenceCollection[iSequence].AnimationCollection[blend, b, t][f] * _BoneCollection[b].ScalePosition.X;
                                    break;
                                case 1:
                                    Pos.Y += _SequenceCollection[iSequence].AnimationCollection[blend, b, t][f] * _BoneCollection[b].ScalePosition.Y;
                                    break;
                                case 2:
                                    Pos.Z += _SequenceCollection[iSequence].AnimationCollection[blend, b, t][f] * _BoneCollection[b].ScalePosition.Z;
                                    break;
                                case 3:
                                    Rot.X += _SequenceCollection[iSequence].AnimationCollection[blend, b, t][f] * _BoneCollection[b].ScaleRotation.X;
                                    break;
                                case 4:
                                    Rot.Y += _SequenceCollection[iSequence].AnimationCollection[blend, b, t][f] * _BoneCollection[b].ScaleRotation.Y;
                                    break;
                                case 5:
                                    Rot.Z += _SequenceCollection[iSequence].AnimationCollection[blend, b, t][f] * _BoneCollection[b].ScaleRotation.Z;
                                    break;
                            }
                        }
                    }

                    // Force linear movement...
                    if ((_BoneCollection[b].Parent == -1) && (_SequenceCollection[iSequence].FrameCount > 1))
                    {
                        Pos.X += f * _SequenceCollection[iSequence].LinearMovement[0] / (_SequenceCollection[iSequence].FrameCount - 1);
                        Pos.Y += f * _SequenceCollection[iSequence].LinearMovement[1] / (_SequenceCollection[iSequence].FrameCount - 1);
                        Pos.Z += f * _SequenceCollection[iSequence].LinearMovement[2] / (_SequenceCollection[iSequence].FrameCount - 1);
                    }

                    times.Add(new Time(b, Pos, Rot));
                }
                smd.TimeCollection.Add(new Frame(f, times));
            }

            if (smd.TimeCollection.Count == 0)
                smd.TimeCollection.Add(new Frame(0, @default));

            smd.Rotate(-90);
            smd.Write(path.TrimEnd('\\') + " \\" + StaticMethods.GoodFileName(sequence) + STRING_ANI + ".smd");

            // Make sure we can find one if needed.
            StaticMethods.SmdFile = path.TrimEnd('\\') + " \\" + StaticMethods.GoodFileName(sequence) + STRING_ANI + ".smd";

            return StaticMethods.GoodFileName(sequence) + STRING_ANI;
        }
        #endregion

        #region " Shared "
        private byte FindBody(string body)
        {
            for (byte i = 0; i < _BodyGroupCollection.Count; i++)
            {
                if (_BodyGroupCollection[i].Name == body)
                    return i;
            }
            Messages.ThrowException("Mdl.MdlFileV14.FindBody(string)", Messages.NOT_FOUND_IN_COLLECTION + body);
            return 0;
        }
        private byte FindModel(int offset)
        {
            for (byte i = 0; i < _Header.ModelCount; i++)
            {
                if (_Header.ModelsOffset[i] == offset)
                    return i;
            }
            Messages.ThrowException("Mdl.MdlFileV14.FindModel(int)", Messages.OFFSET_NOT_FOUND + offset);
            return 0;
        }
        private byte FindModel(string model)
        {
            for (byte i = 0; i < _ModelCollection.Count; i++)
            {
                if (_ModelCollection[i].Name == model)
                    return i;
            }
            Messages.ThrowException("Mdl.MdlFileV14.FindModel(string)", Messages.NOT_FOUND_IN_COLLECTION + model);
            return 0;
        }
        private byte FindSequence(string sequence, out byte blend)
        {
            blend = 0;
            if (sequence.EndsWith("_blend1"))
            {
                blend = 1;
                sequence = sequence.Substring(0, sequence.Length - 7);
            }
            if (sequence.EndsWith("_blend2"))
            {
                blend = 2;
                sequence = sequence.Substring(0, sequence.Length - 7);
            }
            if (sequence.EndsWith("_blend3"))
            {
                blend = 3;
                sequence = sequence.Substring(0, sequence.Length - 7);
            }

            for (byte i = 0; i < _SequenceCollection.Count; i++)
            {
                if (_SequenceCollection[i].Name == sequence)
                    return i;
            }
            Messages.ThrowException("Mdl.MdlFileV14.FindSequence(string)", Messages.NOT_FOUND_IN_COLLECTION + sequence);
            return 0;
        }
        private sbyte[] BlendsToBones(sbyte[] bonemappings, int vertex)
        {
            sbyte[] output = new sbyte[4];
            for (byte i = 0; i < _BlendingCollection[vertex].Value.Length; i++)
            {
                if (_BlendingCollection[vertex].Value[i] == -1)
                    output[i] = -1;
                else
                    output[i] = bonemappings[_BlendingCollection[vertex].Value[i]];
            }
            return output;
        }
        private BlendV11[] BlendsToBlending(sbyte[] bonemappings, int vertex)
        {
            BlendV11[] output = new BlendV11[4];
            for (byte i = 0; i < _BlendingCollection[vertex].Value.Length; i++)
            {
                sbyte bone;
                if (_BlendingCollection[vertex].Value[i] == -1)
                    bone = -1;
                else
                    bone = bonemappings[_BlendingCollection[vertex].Value[i]];
                output[i] = new BlendV11(bone, _BlendingScalesCollection[vertex].Value[i]);
            }
            return output;
        }
        private List<LinkV44> BlendsToLinks(sbyte[] bonemappings, int vertex)
        {
            List<LinkV44> output = new List<LinkV44>();
            for (byte i = 0; i < _BlendingCollection[vertex].Value.Length; i++)
            {
                if (_BlendingCollection[vertex].Value[i] != -1)
                    output.Add(new LinkV44(bonemappings[_BlendingCollection[vertex].Value[i]], _BlendingScalesCollection[vertex].Value[i]));
            }
            return output;
        }
        #endregion
    }
}
