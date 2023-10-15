// Decompiled with JetBrains decompiler
// Type: JbnLib.Bsp.BspV42
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Lang;
using JbnLib.Map;
using JbnLib.Qc;
using JbnLib.Shared;
using JbnLib.Smd;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Drawing.Imaging;
using System.IO;
using System.Text;

namespace JbnLib.Bsp
{
  public class BspV42 : List<OffsetLengthPair>
  {
    public const int Version = 42;
    public const int Lumps = 18;
    private List<EntityV510> _EntityCollection;
    private List<PlaneV42> _PlaneCollection;
    private List<ShaderV42> _ShaderCollection;
    private List<FogV42> _FogCollection;
    private List<VertexV42> _VertexCollection;
    private List<ZeroVertexV42> _ZeroVertexCollection;
    private List<DrawIndexV42> _DrawIndexCollection;
    private List<VisibilityV42> _VisibilityCollection;
    private List<NodeV42> _NodeCollection;
    private List<SurfaceV42> _SurfaceCollection;
    private List<LeafV42> _LeafCollection;
    private List<LeafSurfaceV42> _LeafSurfaceCollection;
    private List<LeafBrushV42> _LeafBrushCollection;
    private List<ModelV42> _ModelCollection;
    private List<BrushV42> _BrushCollection;
    private List<BrushSideV42> _BrushSideCollection;
    private List<TextureMapV42> _TextureMapCollection;

    public List<EntityV510> EntityCollection
    {
      get => this._EntityCollection;
      set => this._EntityCollection = value;
    }

    public List<PlaneV42> PlaneCollection
    {
      get => this._PlaneCollection;
      set => this._PlaneCollection = value;
    }

    public List<ShaderV42> ShaderCollection
    {
      get => this._ShaderCollection;
      set => this._ShaderCollection = value;
    }

    public List<FogV42> FogCollection
    {
      get => this._FogCollection;
      set => this._FogCollection = value;
    }

    public List<VertexV42> VertexCollection
    {
      get => this._VertexCollection;
      set => this._VertexCollection = value;
    }

    public List<ZeroVertexV42> ZeroVertexCollection
    {
      get => this._ZeroVertexCollection;
      set => this._ZeroVertexCollection = value;
    }

    public List<DrawIndexV42> DrawIndexCollection
    {
      get => this._DrawIndexCollection;
      set => this._DrawIndexCollection = value;
    }

    public List<VisibilityV42> VisibilityCollection
    {
      get => this._VisibilityCollection;
      set => this._VisibilityCollection = value;
    }

    public List<NodeV42> NodeCollection
    {
      get => this._NodeCollection;
      set => this._NodeCollection = value;
    }

    public List<SurfaceV42> SurfaceCollection
    {
      get => this._SurfaceCollection;
      set => this._SurfaceCollection = value;
    }

    public List<LeafV42> LeafCollection
    {
      get => this._LeafCollection;
      set => this._LeafCollection = value;
    }

    public List<LeafSurfaceV42> LeafSurfaceCollection
    {
      get => this._LeafSurfaceCollection;
      set => this._LeafSurfaceCollection = value;
    }

    public List<LeafBrushV42> LeafBrushCollection
    {
      get => this._LeafBrushCollection;
      set => this._LeafBrushCollection = value;
    }

    public List<ModelV42> ModelCollection
    {
      get => this._ModelCollection;
      set => this._ModelCollection = value;
    }

    public List<BrushV42> BrushCollection
    {
      get => this._BrushCollection;
      set => this._BrushCollection = value;
    }

    public List<BrushSideV42> BrushSideCollection
    {
      get => this._BrushSideCollection;
      set => this._BrushSideCollection = value;
    }

    public List<TextureMapV42> TextureMapCollection
    {
      get => this._TextureMapCollection;
      set => this._TextureMapCollection = value;
    }

    public void Read(string path)
    {
      this.Clear();
      if (!File.Exists(path))
        Messages.ThrowException("Bsp.BspV42.Read(string)", Messages.FILE_NOT_FOUND + path);
      FileStream input = new FileStream(path, FileMode.Open, FileAccess.Read);
      BinaryReader binaryReader = new BinaryReader((Stream) input);
      int num1 = binaryReader.ReadInt32();
      if (num1 != 42)
        Messages.ThrowException("Bsp.BspV42.Read(string)", Messages.VERSION_MISMATCH + Messages.EXPECTING + "42 " + Messages.GOT + (object) num1);
      for (byte index = 0; index < (byte) 18; ++index)
        this.Add(new OffsetLengthPair(binaryReader.ReadInt32(), binaryReader.ReadInt32()));
      input.Seek((long) this[0].Offset, SeekOrigin.Begin);
      MapFileV510 mapFileV510 = new MapFileV510();
      mapFileV510.Read(binaryReader.ReadChars(this[0].Length));
      foreach (EntityV510 entityV510 in (List<EntityV510>) mapFileV510)
        this._EntityCollection.Add(entityV510);
      input.Seek((long) this[1].Offset, SeekOrigin.Begin);
      int int32_1 = Convert.ToInt32(this[1].Length / PlaneV42.Length);
      for (int index = 0; index < int32_1; ++index)
        this._PlaneCollection.Add(new PlaneV42(binaryReader.ReadBytes(PlaneV42.Length)));
      input.Seek((long) this[2].Offset, SeekOrigin.Begin);
      int int32_2 = Convert.ToInt32(this[2].Length / ShaderV42.Length);
      for (int index = 0; index < int32_2; ++index)
        this._ShaderCollection.Add(new ShaderV42(binaryReader.ReadBytes(ShaderV42.Length)));
      input.Seek((long) this[3].Offset, SeekOrigin.Begin);
      int int32_3 = Convert.ToInt32(this[3].Length / FogV42.Length);
      for (int index = 0; index < int32_3; ++index)
        this._FogCollection.Add(new FogV42(binaryReader.ReadBytes(FogV42.Length)));
      input.Seek((long) this[4].Offset, SeekOrigin.Begin);
      int int32_4 = Convert.ToInt32(this[4].Length / VertexV42.Length);
      for (int index = 0; index < int32_4; ++index)
        this._VertexCollection.Add(new VertexV42(binaryReader.ReadBytes(VertexV42.Length)));
      input.Seek((long) this[5].Offset, SeekOrigin.Begin);
      int int32_5 = Convert.ToInt32(this[5].Length / ZeroVertexV42.Length);
      for (int index = 0; index < int32_5; ++index)
        this._ZeroVertexCollection.Add(new ZeroVertexV42(binaryReader.ReadBytes(ZeroVertexV42.Length)));
      input.Seek((long) this[6].Offset, SeekOrigin.Begin);
      int int32_6 = Convert.ToInt32(this[6].Length / DrawIndexV42.Length);
      for (int index = 0; index < int32_6; ++index)
        this._DrawIndexCollection.Add(new DrawIndexV42(binaryReader.ReadBytes(DrawIndexV42.Length)));
      input.Seek((long) this[7].Offset, SeekOrigin.Begin);
      this._VisibilityCollection.Add(new VisibilityV42(binaryReader.ReadBytes(this[7].Length)));
      input.Seek((long) this[8].Offset, SeekOrigin.Begin);
      int int32_7 = Convert.ToInt32(this[8].Length / NodeV42.Length);
      for (int index = 0; index < int32_7; ++index)
        this._NodeCollection.Add(new NodeV42(binaryReader.ReadBytes(NodeV42.Length)));
      input.Seek((long) this[9].Offset, SeekOrigin.Begin);
      int int32_8 = Convert.ToInt32(this[9].Length / SurfaceV42.Length);
      for (int index = 0; index < int32_8; ++index)
        this._SurfaceCollection.Add(new SurfaceV42(binaryReader.ReadBytes(SurfaceV42.Length)));
      long num2 = (long) (this[10].Offset + this[10].Length);
      for (int index1 = this._SurfaceCollection.Count - 1; index1 >= 0; --index1)
      {
        if (this._SurfaceCollection[index1].LightingOffset != -1)
        {
          input.Seek((long) (this._SurfaceCollection[index1].LightingOffset + this[10].Offset), SeekOrigin.Begin);
          long num3 = (num2 - (long) this._SurfaceCollection[index1].LightingOffset - (long) this[10].Offset) / (long) LumelV42.Length;
          num2 = input.Position;
          for (int index2 = 0; (long) index2 < num3; ++index2)
            this._SurfaceCollection[index1].LumelCollection.Add(new LumelV42(binaryReader.ReadBytes(LumelV42.Length)));
        }
      }
      input.Seek((long) this[11].Offset, SeekOrigin.Begin);
      int int32_9 = Convert.ToInt32(this[11].Length / LeafV42.Length);
      for (int index = 0; index < int32_9; ++index)
        this._LeafCollection.Add(new LeafV42(binaryReader.ReadBytes(LeafV42.Length)));
      input.Seek((long) this[12].Offset, SeekOrigin.Begin);
      int int32_10 = Convert.ToInt32(this[12].Length / LeafSurfaceV42.Length);
      for (int index = 0; index < int32_10; ++index)
        this._LeafSurfaceCollection.Add(new LeafSurfaceV42(binaryReader.ReadBytes(LeafSurfaceV42.Length)));
      input.Seek((long) this[13].Offset, SeekOrigin.Begin);
      int int32_11 = Convert.ToInt32(this[13].Length / LeafBrushV42.Length);
      for (int index = 0; index < int32_11; ++index)
        this._LeafBrushCollection.Add(new LeafBrushV42(binaryReader.ReadBytes(LeafBrushV42.Length)));
      input.Seek((long) this[14].Offset, SeekOrigin.Begin);
      int int32_12 = Convert.ToInt32(this[14].Length / ModelV42.Length);
      for (int index = 0; index < int32_12; ++index)
        this._ModelCollection.Add(new ModelV42(binaryReader.ReadBytes(ModelV42.Length)));
      input.Seek((long) this[15].Offset, SeekOrigin.Begin);
      int int32_13 = Convert.ToInt32(this[15].Length / BrushV42.Length);
      for (int index = 0; index < int32_13; ++index)
        this._BrushCollection.Add(new BrushV42(binaryReader.ReadBytes(BrushV42.Length)));
      input.Seek((long) this[16].Offset, SeekOrigin.Begin);
      int int32_14 = Convert.ToInt32(this[16].Length / BrushSideV42.Length);
      for (int index = 0; index < int32_14; ++index)
        this._BrushSideCollection.Add(new BrushSideV42(binaryReader.ReadBytes(BrushSideV42.Length)));
      input.Seek((long) this[17].Offset, SeekOrigin.Begin);
      int int32_15 = Convert.ToInt32(this[17].Length / TextureMapV42.Length);
      for (int index = 0; index < int32_15; ++index)
        this._TextureMapCollection.Add(new TextureMapV42(binaryReader.ReadBytes(TextureMapV42.Length)));
      binaryReader.Close();
      input.Close();
    }

    public void Write(string path)
    {
      FileStream output = new FileStream(path, FileMode.Create, FileAccess.Write);
      BinaryWriter bw = new BinaryWriter((Stream) output);
      output.Seek(148L, SeekOrigin.Begin);
      this[4] = new OffsetLengthPair(Convert.ToInt32(output.Position), this._VertexCollection.Count * VertexV42.Length);
      foreach (VertexV42 vertex in this._VertexCollection)
        bw.Write(vertex.GetBytes());
      bw.Flush();
      this[5] = new OffsetLengthPair(Convert.ToInt32(output.Position), this._ZeroVertexCollection.Count * ZeroVertexV42.Length);
      foreach (ZeroVertexV42 zeroVertex in this._ZeroVertexCollection)
        bw.Write(zeroVertex.GetBytes());
      bw.Flush();
      this[6] = new OffsetLengthPair(Convert.ToInt32(output.Position), this._DrawIndexCollection.Count * DrawIndexV42.Length);
      foreach (DrawIndexV42 drawIndex in this._DrawIndexCollection)
        bw.Write(drawIndex.GetBytes());
      bw.Flush();
      MapFileV510 mapFileV510 = new MapFileV510();
      foreach (EntityV510 entity in this._EntityCollection)
        mapFileV510.Add(entity);
      byte[] bytesNoComments = mapFileV510.GetBytesNoComments();
      this[0] = new OffsetLengthPair(Convert.ToInt32(output.Position), bytesNoComments.Length + 1);
      bw.Write(bytesNoComments);
      bw.Write((byte) 0);
      StaticMethods.Buffer4(ref bw);
      this[2] = new OffsetLengthPair(Convert.ToInt32(output.Position), this._ShaderCollection.Count * ShaderV42.Length);
      foreach (ShaderV42 shader in this._ShaderCollection)
        bw.Write(shader.GetBytes());
      bw.Flush();
      this[3] = new OffsetLengthPair(Convert.ToInt32(output.Position), this._FogCollection.Count * FogV42.Length);
      foreach (FogV42 fog in this._FogCollection)
        bw.Write(fog.GetBytes());
      bw.Flush();
      this[17] = new OffsetLengthPair(Convert.ToInt32(output.Position), this._TextureMapCollection.Count * TextureMapV42.Length);
      foreach (TextureMapV42 textureMap in this._TextureMapCollection)
        bw.Write(textureMap.GetBytes());
      bw.Flush();
      int int32 = Convert.ToInt32(output.Position);
      foreach (SurfaceV42 surface in this._SurfaceCollection)
      {
        surface.LightingOffset = surface.LumelCollection.Count != 0 ? Convert.ToInt32(output.Position - (long) int32) : -1;
        foreach (LumelV42 lumel in surface.LumelCollection)
          bw.Write(lumel.GetBytes());
        bw.Flush();
      }
      this[10] = new OffsetLengthPair(int32, Convert.ToInt32(output.Position - (long) int32));
      StaticMethods.Buffer4(ref bw);
      this[1] = new OffsetLengthPair(Convert.ToInt32(output.Position), this._PlaneCollection.Count * PlaneV42.Length);
      foreach (PlaneV42 plane in this._PlaneCollection)
        bw.Write(plane.GetBytes());
      bw.Flush();
      this[9] = new OffsetLengthPair(Convert.ToInt32(output.Position), this._SurfaceCollection.Count * SurfaceV42.Length);
      foreach (SurfaceV42 surface in this._SurfaceCollection)
        bw.Write(surface.GetBytes());
      bw.Flush();
      this[16] = new OffsetLengthPair(Convert.ToInt32(output.Position), this._BrushSideCollection.Count * BrushSideV42.Length);
      foreach (BrushSideV42 brushSide in this._BrushSideCollection)
        bw.Write(brushSide.GetBytes());
      bw.Flush();
      this[15] = new OffsetLengthPair(Convert.ToInt32(output.Position), this._BrushCollection.Count * BrushV42.Length);
      foreach (BrushV42 brush in this._BrushCollection)
        bw.Write(brush.GetBytes());
      bw.Flush();
      this[12] = new OffsetLengthPair(Convert.ToInt32(output.Position), this._LeafSurfaceCollection.Count * LeafSurfaceV42.Length);
      foreach (LeafSurfaceV42 leafSurface in this._LeafSurfaceCollection)
        bw.Write(leafSurface.GetBytes());
      bw.Flush();
      this[13] = new OffsetLengthPair(Convert.ToInt32(output.Position), this._LeafBrushCollection.Count * LeafBrushV42.Length);
      foreach (LeafBrushV42 leafBrush in this._LeafBrushCollection)
        bw.Write(leafBrush.GetBytes());
      bw.Flush();
      this[7] = new OffsetLengthPair(Convert.ToInt32(output.Position), this._VisibilityCollection[0].Data.Length);
      bw.Write(this._VisibilityCollection[0].Data);
      bw.Flush();
      this[11] = new OffsetLengthPair(Convert.ToInt32(output.Position), this._LeafCollection.Count * LeafV42.Length);
      foreach (LeafV42 leaf in this._LeafCollection)
        bw.Write(leaf.GetBytes());
      bw.Flush();
      this[8] = new OffsetLengthPair(Convert.ToInt32(output.Position), this._NodeCollection.Count * NodeV42.Length);
      foreach (NodeV42 node in this._NodeCollection)
        bw.Write(node.GetBytes());
      bw.Flush();
      this[14] = new OffsetLengthPair(Convert.ToInt32(output.Position), this._ModelCollection.Count * ModelV42.Length);
      foreach (ModelV42 model in this._ModelCollection)
        bw.Write(model.GetBytes());
      bw.Flush();
      output.Seek(0L, SeekOrigin.Begin);
      bw.Write(42);
      for (int index = 0; index < 18; ++index)
      {
        bw.Write(this[index].Offset);
        bw.Write(this[index].Length);
      }
      bw.Flush();
      bw.Close();
      output.Close();
    }

    public new void Clear()
    {
      base.Clear();
      this._BrushCollection = new List<BrushV42>();
      this._BrushSideCollection = new List<BrushSideV42>();
      this._DrawIndexCollection = new List<DrawIndexV42>();
      this._EntityCollection = new List<EntityV510>();
      this._FogCollection = new List<FogV42>();
      this._LeafBrushCollection = new List<LeafBrushV42>();
      this._LeafCollection = new List<LeafV42>();
      this._LeafSurfaceCollection = new List<LeafSurfaceV42>();
      this._ModelCollection = new List<ModelV42>();
      this._NodeCollection = new List<NodeV42>();
      this._PlaneCollection = new List<PlaneV42>();
      this._ShaderCollection = new List<ShaderV42>();
      this._SurfaceCollection = new List<SurfaceV42>();
      this._TextureMapCollection = new List<TextureMapV42>();
      this._VertexCollection = new List<VertexV42>();
      this._VisibilityCollection = new List<VisibilityV42>();
      this._ZeroVertexCollection = new List<ZeroVertexV42>();
    }

    public static void Separate(
      string openfile,
      string savedirectory,
      byte[] header,
      string[] lumps)
    {
      FileStream input = new FileStream(openfile, FileMode.Open, FileAccess.Read);
      BinaryReader binaryReader = new BinaryReader((Stream) input);
      byte[] numArray1 = binaryReader.ReadBytes(header.Length);
      if (StaticMethods.Compare(numArray1, header) != 0)
        Messages.ThrowException("Bsp.Lumps.Separate(string, string, byte[], string[])", Messages.IDENTIFIER_MISMATCH + Messages.EXPECTING + Encoding.ASCII.GetString(header) + " " + Messages.GOT + Encoding.ASCII.GetString(numArray1));
      int[,] numArray2 = new int[lumps.Length, 2];
      for (uint index = 0; (long) index < (long) lumps.Length; ++index)
      {
        numArray2[(int) index, 0] = binaryReader.ReadInt32();
        numArray2[(int) index, 1] = binaryReader.ReadInt32();
      }
      for (uint index = 0; (long) index < (long) lumps.Length; ++index)
      {
        if (!Directory.Exists(savedirectory))
          Directory.CreateDirectory(savedirectory);
        FileStream output = new FileStream(savedirectory + "\\" + lumps[(int) index], FileMode.Create, FileAccess.Write);
        BinaryWriter binaryWriter = new BinaryWriter((Stream) output);
        input.Seek((long) numArray2[(int) index, 0], SeekOrigin.Begin);
        byte[] buffer = new byte[4096];
        int num = numArray2[(int) index, 1];
        while (num > 0)
        {
          int count = num >= buffer.Length ? buffer.Length : num;
          buffer = binaryReader.ReadBytes(count);
          binaryWriter.Write(buffer, 0, count);
          num -= count;
          binaryWriter.Flush();
        }
        binaryWriter.Close();
        output.Close();
      }
      binaryReader.Close();
      input.Close();
    }

    public static void Combine(
      string opendirectory,
      string savefile,
      byte[] header,
      string[] lumps)
    {
      FileStream output = new FileStream(savefile, FileMode.Create, FileAccess.Write);
      BinaryWriter binaryWriter = new BinaryWriter((Stream) output);
      binaryWriter.Write(header);
      for (int index = 0; index < lumps.Length * 2; ++index)
        binaryWriter.Write(0U);
      int[,] numArray = new int[lumps.Length, 2];
      for (uint index = 0; (long) index < (long) lumps.Length; ++index)
      {
        FileInfo fileInfo = new FileInfo(opendirectory + "\\" + lumps[(int) index]);
        FileStream input = new FileStream(fileInfo.FullName, FileMode.Open, FileAccess.Read);
        BinaryReader binaryReader = new BinaryReader((Stream) input);
        numArray[(int) index, 0] = Convert.ToInt32(output.Position);
        numArray[(int) index, 1] = Convert.ToInt32(fileInfo.Length);
        byte[] buffer = new byte[4096];
        int num = numArray[(int) index, 1];
        while (num > 0)
        {
          int count = num >= buffer.Length ? buffer.Length : num;
          buffer = binaryReader.ReadBytes(count);
          binaryWriter.Write(buffer, 0, count);
          num -= count;
          binaryWriter.Flush();
        }
        binaryReader.Close();
        input.Close();
      }
      output.Seek((long) header.Length, SeekOrigin.Begin);
      for (byte index = 0; (int) index < lumps.Length; ++index)
      {
        binaryWriter.Write(numArray[(int) index, 0]);
        binaryWriter.Write(numArray[(int) index, 1]);
      }
      binaryWriter.Flush();
      binaryWriter.Close();
      output.Close();
    }

    public bool DumpQcV10(string qcname, string textureopen) => this.DumpQcV10(qcname, textureopen, "", ImageFormat.Bmp);

    public bool DumpQcV10(string qcname, string textureopen, string texturesave) => this.DumpQcV10(qcname, textureopen, texturesave, ImageFormat.Png);

    public bool DumpQcV10(
      string qcname,
      string textureopen,
      string texturesave,
      ImageFormat format)
    {
      bool flag = false;
      QcFileV10 qcFileV10 = new QcFileV10();
      qcFileV10.ModelName = new CommandModelName(qcname.Split('\\')[qcname.Split('\\').Length - 1].Remove(qcname.Split('\\')[qcname.Split('\\').Length - 1].Length - 2, 2) + "mdl");
      SmdFileV10 smdV10 = this.ToSmdV10(textureopen, texturesave, format);
      SmdFileV10 smdFileV10 = new SmdFileV10(SmdType.Animation);
      smdFileV10._NodeCollection.Add(new JbnLib.Smd.Node((sbyte) 0, "root", (sbyte) -1));
      smdFileV10._TimeCollection.Add(new Frame(0, new List<Time>()
      {
        new Time(0, new Point3D(0.0f, 0.0f, 0.0f), new Point3D(0.0f, 0.0f, 0.0f))
      }));
      qcFileV10._BodyGroupCollection.Add(new CommandBodyGroup("studio")
      {
        BodyCollection = {
          new BodyGroupItem("studio", (qcname.Remove(qcname.Length - 3) + "_ref").Substring(qcname.LastIndexOf('\\') + 1))
        }
      });
      qcFileV10.SequenceCollection.Add(new CommandSequenceV10()
      {
        Name = "idle1",
        FileCollection = new List<string>()
        {
          (qcname.Remove(qcname.Length - 3) + "_seq").Substring(qcname.LastIndexOf('\\') + 1)
        }
      });
      try
      {
        if (!Directory.Exists(qcname.Remove(qcname.LastIndexOf('\\'), qcname.Length - qcname.LastIndexOf('\\'))))
          Directory.CreateDirectory(qcname.Remove(qcname.LastIndexOf('\\'), qcname.Length - qcname.LastIndexOf('\\')));
        if (File.Exists(qcname))
          File.Delete(qcname);
        qcFileV10.Write(qcname);
        if (File.Exists(qcname.Remove(qcname.Length - 3) + "_ref.smd"))
          File.Delete(qcname.Remove(qcname.Length - 3) + "_ref.smd");
        smdV10.Write(qcname.Remove(qcname.Length - 3) + "_ref.smd");
        if (File.Exists(qcname.Remove(qcname.Length - 3) + "_seq.smd"))
          File.Delete(qcname.Remove(qcname.Length - 3) + "_seq.smd");
        smdFileV10.Write(qcname.Remove(qcname.Length - 3) + "_seq.smd");
        flag = true;
      }
      catch (Exception ex)
      {
        Messages.ThrowException("BspV42.DumpQcV10(string, string, string, ImageFormat)", ex.Message);
      }
      return flag;
    }

    public bool DumpQcV11(string qcname, string textureopen) => this.DumpQcV11(qcname, textureopen, "", ImageFormat.Bmp);

    public bool DumpQcV11(string qcname, string textureopen, string texturesave) => this.DumpQcV11(qcname, textureopen, texturesave, ImageFormat.Png);

    public bool DumpQcV11(
      string qcname,
      string textureopen,
      string texturesave,
      ImageFormat format)
    {
      bool flag = false;
      QcFileV11 qcFileV11 = new QcFileV11();
      qcFileV11.ModelName = new CommandModelName(qcname.Split('\\')[qcname.Split('\\').Length - 1].Remove(qcname.Split('\\')[qcname.Split('\\').Length - 1].Length - 2, 2) + "mdl");
      SmdFileV11 v11 = this.ToSmdV10(textureopen, texturesave, format).ToV11();
      SmdFileV11 smdFileV11 = new SmdFileV11(SmdType.Animation);
      smdFileV11._NodeCollection.Add(new JbnLib.Smd.Node((sbyte) 0, "root", (sbyte) -1));
      smdFileV11._TimeCollection.Add(new Frame(0, new List<Time>()
      {
        new Time(0, new Point3D(0.0f, 0.0f, 0.0f), new Point3D(0.0f, 0.0f, 0.0f))
      }));
      int[][] numArray = new int[this._ShaderCollection.Count][];
      for (int index1 = 0; index1 < this._ShaderCollection.Count; ++index1)
      {
        for (int index2 = 0; index2 < this._FogCollection.Count; ++index2)
        {
          numArray[index1] = new int[this._FogCollection.Count];
          numArray[index1].SetValue((object) 0, index2);
        }
      }
      foreach (SurfaceV42 surface in this._SurfaceCollection)
        ++numArray[surface.ShaderIndex][surface.FogIndex];
      for (int index3 = 0; index3 < this._ShaderCollection.Count; ++index3)
      {
        if (!this._ShaderCollection[index3].Texture.ToLower().StartsWith("special/"))
        {
          int index4 = 0;
          int num = numArray[index3][0];
          for (int index5 = 1; index5 < this._FogCollection.Count; ++index5)
          {
            if (numArray[index3][index5] > num)
            {
              index4 = index5;
              num = numArray[index3][index5];
            }
          }
          string texture = this._ShaderCollection[index3].Texture.Substring(this._ShaderCollection[index3].Texture.LastIndexOf('/') + 1);
          qcFileV11.TextureCollection.Add(new CommandTexture(texture, StaticMethods.ResolveWorldToModelMaterial(this._FogCollection[index4].Material)));
        }
      }
      qcFileV11._BodyGroupCollection.Add(new CommandBodyGroup("studio")
      {
        BodyCollection = {
          new BodyGroupItem("studio", (qcname.Remove(qcname.Length - 3) + "_ref").Substring(qcname.LastIndexOf('\\') + 1))
        }
      });
      qcFileV11.SequenceCollection.Add(new CommandSequenceV11()
      {
        Name = "idle1",
        FileCollection = new List<string>()
        {
          (qcname.Remove(qcname.Length - 3) + "_seq").Substring(qcname.LastIndexOf('\\') + 1)
        }
      });
      try
      {
        if (!Directory.Exists(qcname.Remove(qcname.LastIndexOf('\\'), qcname.Length - qcname.LastIndexOf('\\'))))
          Directory.CreateDirectory(qcname.Remove(qcname.LastIndexOf('\\'), qcname.Length - qcname.LastIndexOf('\\')));
        if (File.Exists(qcname))
          File.Delete(qcname);
        qcFileV11.Write(qcname);
        if (File.Exists(qcname.Remove(qcname.Length - 3) + "_ref.smd"))
          File.Delete(qcname.Remove(qcname.Length - 3) + "_ref.smd");
        v11.Write(qcname.Remove(qcname.Length - 3) + "_ref.smd");
        if (File.Exists(qcname.Remove(qcname.Length - 3) + "_seq.smd"))
          File.Delete(qcname.Remove(qcname.Length - 3) + "_seq.smd");
        smdFileV11.Write(qcname.Remove(qcname.Length - 3) + "_seq.smd");
        flag = true;
      }
      catch (Exception ex)
      {
        Messages.ThrowException("BspV42.DumpQcV11(string, string, string, ImageFormat)", ex.Message);
      }
      return flag;
    }

    public bool DumpQcV44(string qcname, string textureopen) => this.DumpQcV44(qcname, textureopen, "", ImageFormat.Bmp);

    public bool DumpQcV44(string qcname, string textureopen, string texturesave) => this.DumpQcV44(qcname, textureopen, texturesave, ImageFormat.Png);

    public bool DumpQcV44(
      string qcname,
      string textureopen,
      string texturesave,
      ImageFormat format)
    {
      bool flag = false;
      QcFileV44 qcFileV44 = new QcFileV44();
      qcFileV44.ModelName = new CommandModelName(qcname.Split('\\')[qcname.Split('\\').Length - 1].Remove(qcname.Split('\\')[qcname.Split('\\').Length - 1].Length - 2, 2) + "mdl");
      SmdFileV44 v44 = this.ToSmdV10(textureopen, texturesave, format).ToV44();
      SmdFileV44 smdFileV44 = new SmdFileV44(SmdType.Animation);
      smdFileV44._NodeCollection.Add(new JbnLib.Smd.Node((sbyte) 0, "root", (sbyte) -1));
      smdFileV44._TimeCollection.Add(new Frame(0, new List<Time>()
      {
        new Time(0, new Point3D(0.0f, 0.0f, 0.0f), new Point3D(0.0f, 0.0f, 0.0f))
      }));
      qcFileV44._BodyGroupCollection.Add(new CommandBodyGroup("studio")
      {
        BodyCollection = {
          new BodyGroupItem("studio", (qcname.Remove(qcname.Length - 3) + "_ref").Substring(qcname.LastIndexOf('\\') + 1))
        }
      });
      qcFileV44.SequenceCollection.Add(new CommandSequenceV44()
      {
        Name = "idle1",
        FileCollection = new List<string>()
        {
          (qcname.Remove(qcname.Length - 3) + "_seq").Substring(qcname.LastIndexOf('\\') + 1)
        }
      });
      try
      {
        if (!Directory.Exists(qcname.Remove(qcname.LastIndexOf('\\'), qcname.Length - qcname.LastIndexOf('\\'))))
          Directory.CreateDirectory(qcname.Remove(qcname.LastIndexOf('\\'), qcname.Length - qcname.LastIndexOf('\\')));
        if (File.Exists(qcname))
          File.Delete(qcname);
        qcFileV44.Write(qcname);
        if (File.Exists(qcname.Remove(qcname.Length - 3) + "_ref.smd"))
          File.Delete(qcname.Remove(qcname.Length - 3) + "_ref.smd");
        v44.Write(qcname.Remove(qcname.Length - 3) + "_ref.smd");
        if (File.Exists(qcname.Remove(qcname.Length - 3) + "_seq.smd"))
          File.Delete(qcname.Remove(qcname.Length - 3) + "_seq.smd");
        smdFileV44.Write(qcname.Remove(qcname.Length - 3) + "_seq.smd");
        flag = true;
      }
      catch (Exception ex)
      {
        Messages.ThrowException("BspV42.DumpQcV44(string, string, string, ImageFormat)", ex.Message);
      }
      return flag;
    }

    public SmdFileV10 ToSmdV10(string textureopen) => this.ToSmdV10(textureopen, "", ImageFormat.Png);

    public SmdFileV10 ToSmdV10(string textureopen, string texturesave) => this.ToSmdV10(textureopen, texturesave, ImageFormat.Png);

    public SmdFileV10 ToSmdV10(string textureopen, string texturesave, ImageFormat format)
    {
      string str1 = StaticMethods.CheckImgExtension(format);
      SmdFileV10 smdV10 = new SmdFileV10(SmdType.Reference);
      smdV10.NodeCollection.Add(new JbnLib.Smd.Node((sbyte) 0, "root", (sbyte) -1));
      List<Time> bones1 = new List<Time>()
      {
        new Time(0, new Point3D(), new Point3D())
      };
      textureopen = textureopen.TrimEnd('\\') + "\\";
      texturesave = texturesave.TrimEnd('\\') + "\\";
      string[] strArray = new string[this._ShaderCollection.Count];
      Size[] sizeArray = new Size[this._ShaderCollection.Count];
      for (int index = 0; index < this._ShaderCollection.Count; ++index)
      {
        if (!this._ShaderCollection[index].Texture.StartsWith("special/"))
        {
          if (File.Exists(textureopen + this._ShaderCollection[index].Texture.Replace('/', '\\') + ".png"))
          {
            Image image = Image.FromFile(textureopen + this._ShaderCollection[index].Texture.Replace('/', '\\') + ".png");
            strArray[index] = StaticMethods.GoodFileName(this._ShaderCollection[index].Texture);
            sizeArray[index] = image.Size;
            if (texturesave != "\\")
            {
              if (File.Exists(texturesave + strArray[index] + str1))
                File.Delete(texturesave + strArray[index] + str1);
              image.Save(texturesave + strArray[index] + str1, format);
            }
          }
          else
            Messages.ThrowException("BspV42.ToSmdV10(string, string, ImageFormat)", "Could not find texture " + textureopen + this._ShaderCollection[index].Texture.Replace('/', '\\') + ".png");
        }
      }
      List<ModelV42> modelV42List = new List<ModelV42>();
      List<Point3D> point3DList = new List<Point3D>();
      modelV42List.Add(this._ModelCollection[0]);
      point3DList.Add(new Point3D());
      foreach (EntityV510 entity in this._EntityCollection)
      {
        string str2 = "";
        if (entity.TryGetValue("model", out str2) && str2[0] == '*')
        {
          modelV42List.Add(this._ModelCollection[Convert.ToInt32(entity["model"].Substring(1))]);
          if (entity.TryGetValue("origin", out str2))
          {
            if (str2.Split(' ').Length > 2)
            {
              point3DList.Add(new Point3D((float) Convert.ToInt32(entity["origin"].Split(' ')[0]), (float) Convert.ToInt32(entity["origin"].Split(' ')[1]), (float) Convert.ToInt32(entity["origin"].Split(' ')[2])));
              smdV10.NodeCollection.Add(new JbnLib.Smd.Node(Convert.ToSByte(smdV10.NodeCollection.Count), "model" + (object) point3DList.Count, (sbyte) 0));
              bones1.Add(new Time((int) Convert.ToSByte(smdV10.NodeCollection.Count - 1), point3DList[point3DList.Count - 1], new Point3D()));
              continue;
            }
          }
          point3DList.Add(new Point3D());
        }
      }
      for (int index1 = 0; index1 < modelV42List.Count; ++index1)
      {
        int num = index1;
        if (point3DList[index1] == new Point3D())
        {
          num = 0;
        }
        else
        {
          for (int index2 = 1; index2 < index1; ++index2)
            --num;
        }
        for (int surfaceIndex = modelV42List[index1].SurfaceIndex; surfaceIndex < modelV42List[index1].SurfaceIndex + modelV42List[index1].SurfaceCount; ++surfaceIndex)
        {
          SurfaceV42 surface = this._SurfaceCollection[surfaceIndex];
          if (!this._ShaderCollection[surface.ShaderIndex].Texture.StartsWith("special/"))
          {
            int index3 = 0;
            Point3D[] corners = new Point3D[surface.VertexCount];
            for (int vertexIndex = surface.VertexIndex; vertexIndex < surface.VertexCount + surface.VertexIndex; ++vertexIndex)
            {
              corners[index3] = this._VertexCollection[vertexIndex].Position + point3DList[index1];
              ++index3;
            }
            sbyte[] bones2 = new sbyte[corners.Length];
            for (int index4 = 0; index4 < bones2.Length; ++index4)
              bones2[index4] = Convert.ToSByte(num);
            VertexV10[] uvVerts = StaticMethods.StFaceToUvVerts(bones2, corners, this._PlaneCollection[surface.PlaneIndex].Normal, this._TextureMapCollection[surface.TextureMapShaderIndex], sizeArray[surface.ShaderIndex]);
            for (int drawIndex = surface.DrawIndex; drawIndex < surface.DrawCount + surface.DrawIndex; drawIndex += 3)
              smdV10.TriangleCollection.Add(new Triangle<VertexV10>()
              {
                Texture = strArray[surface.ShaderIndex],
                Vertices = {
                  uvVerts[this._DrawIndexCollection[drawIndex + 2].VertexIndex],
                  uvVerts[this._DrawIndexCollection[drawIndex + 1].VertexIndex],
                  uvVerts[this._DrawIndexCollection[drawIndex].VertexIndex]
                }
              });
          }
        }
      }
      smdV10.TimeCollection.Add(new Frame(0, bones1));
      return smdV10;
    }
  }
}
