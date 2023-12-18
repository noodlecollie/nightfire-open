// Decompiled with JetBrains decompiler
// Type: JbnLib.Map.MapFileV220
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Bsp;
using JbnLib.Lang;
using JbnLib.Qc;
using JbnLib.Shared;
using JbnLib.Smd;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Drawing.Imaging;
using System.IO;

namespace JbnLib.Map
{
  public class MapFileV220 : List<EntityV220>, IMapFile
  {
    public void Read(string path)
    {
      EntityV220 entityV220 = (EntityV220) null;
      BrushV220 brushV220 = (BrushV220) null;
      FileTokenizer fileTokenizer = new FileTokenizer(path);
      while (!fileTokenizer.GetToken())
      {
        switch (fileTokenizer.Token)
        {
          case "{":
            if (entityV220 == null)
            {
              entityV220 = new EntityV220();
              continue;
            }
            brushV220 = new BrushV220();
            continue;
          case "}":
            if (brushV220 != null)
            {
              brushV220.Id = entityV220.BrushCollection.Count;
              entityV220.BrushCollection.Add(brushV220);
              brushV220 = (BrushV220) null;
              continue;
            }
            entityV220.Id = this.Count;
            this.Add(entityV220);
            entityV220 = (EntityV220) null;
            continue;
          case "(":
            SideV220 sideV220 = new SideV220();
            fileTokenizer.GetToken();
            sideV220.Plane[0][0] = Convert.ToSingle(fileTokenizer.Token);
            fileTokenizer.GetToken();
            sideV220.Plane[0][1] = Convert.ToSingle(fileTokenizer.Token);
            fileTokenizer.GetToken();
            sideV220.Plane[0][2] = Convert.ToSingle(fileTokenizer.Token);
            fileTokenizer.GetToken();
            fileTokenizer.GetToken();
            fileTokenizer.GetToken();
            sideV220.Plane[1][0] = Convert.ToSingle(fileTokenizer.Token);
            fileTokenizer.GetToken();
            sideV220.Plane[1][1] = Convert.ToSingle(fileTokenizer.Token);
            fileTokenizer.GetToken();
            sideV220.Plane[1][2] = Convert.ToSingle(fileTokenizer.Token);
            fileTokenizer.GetToken();
            fileTokenizer.GetToken();
            fileTokenizer.GetToken();
            sideV220.Plane[2][0] = Convert.ToSingle(fileTokenizer.Token);
            fileTokenizer.GetToken();
            sideV220.Plane[2][1] = Convert.ToSingle(fileTokenizer.Token);
            fileTokenizer.GetToken();
            sideV220.Plane[2][2] = Convert.ToSingle(fileTokenizer.Token);
            fileTokenizer.GetToken();
            fileTokenizer.GetToken();
            sideV220.Texture = fileTokenizer.Token;
            fileTokenizer.GetToken();
            fileTokenizer.GetToken();
            sideV220.S[0] = Convert.ToSingle(fileTokenizer.Token);
            fileTokenizer.GetToken();
            sideV220.S[1] = Convert.ToSingle(fileTokenizer.Token);
            fileTokenizer.GetToken();
            sideV220.S[2] = Convert.ToSingle(fileTokenizer.Token);
            fileTokenizer.GetToken();
            sideV220.S[3] = Convert.ToSingle(fileTokenizer.Token);
            fileTokenizer.GetToken();
            fileTokenizer.GetToken();
            fileTokenizer.GetToken();
            sideV220.T[0] = Convert.ToSingle(fileTokenizer.Token);
            fileTokenizer.GetToken();
            sideV220.T[1] = Convert.ToSingle(fileTokenizer.Token);
            fileTokenizer.GetToken();
            sideV220.T[2] = Convert.ToSingle(fileTokenizer.Token);
            fileTokenizer.GetToken();
            sideV220.T[3] = Convert.ToSingle(fileTokenizer.Token);
            fileTokenizer.GetToken();
            fileTokenizer.GetToken();
            sideV220.Rotation = Convert.ToSingle(fileTokenizer.Token);
            fileTokenizer.GetToken();
            sideV220.Scale[0] = Convert.ToSingle(fileTokenizer.Token);
            fileTokenizer.GetToken();
            sideV220.Scale[1] = Convert.ToSingle(fileTokenizer.Token);
            brushV220.Add(sideV220);
            continue;
          default:
            string token = fileTokenizer.Token;
            fileTokenizer.GetToken();
            if (entityV220.ContainsKey(token))
            {
              entityV220[token] = fileTokenizer.Token;
              continue;
            }
            entityV220.Add(token, fileTokenizer.Token);
            continue;
        }
      }
    }

    public void Write(string path)
    {
      StreamWriter streamWriter = new StreamWriter(path);
      foreach (EntityV220 entityV220 in (List<EntityV220>) this)
      {
        streamWriter.Write(entityV220.ToString());
        streamWriter.Flush();
      }
      streamWriter.Close();
    }

    public bool DumpQcV10(string qcname, string textureopen) => this.DumpQcV10(qcname, textureopen, "", ImageFormat.Bmp);

    public bool DumpQcV10(string qcname, string textureopen, string texturesave) => this.DumpQcV10(qcname, textureopen, texturesave, ImageFormat.Bmp);

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
        Messages.ThrowException("MapFileV220.DumpQcV10(string, string, string, ImageFormat)", ex.Message);
      }
      return flag;
    }

    public bool DumpQcV11(string qcname, string textureopen) => this.DumpQcV11(qcname, textureopen, "", ImageFormat.Bmp);

    public bool DumpQcV11(string qcname, string textureopen, string texturesave) => this.DumpQcV11(qcname, textureopen, texturesave, ImageFormat.Bmp);

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
        Messages.ThrowException("MapFileV220.DumpQcV11(string, string, string, ImageFormat)", ex.Message);
      }
      return flag;
    }

    public bool DumpQcV44(string qcname, string textureopen) => this.DumpQcV44(qcname, textureopen, "", ImageFormat.Bmp);

    public bool DumpQcV44(string qcname, string textureopen, string texturesave) => this.DumpQcV44(qcname, textureopen, texturesave, ImageFormat.Bmp);

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
        Messages.ThrowException("MapFileV220.DumpQcV44(string, string, string, ImageFormat)", ex.Message);
      }
      return flag;
    }

    public SmdFileV10 ToSmdV10(string textureopen) => this.ToSmdV10(textureopen, "", ImageFormat.Bmp);

    public SmdFileV10 ToSmdV10(string textureopen, string texturesave) => this.ToSmdV10(textureopen, texturesave, ImageFormat.Bmp);

    public SmdFileV10 ToSmdV10(string textureopen, string texturesave, ImageFormat format)
    {
      string str = StaticMethods.CheckImgExtension(format);
      SmdFileV10 smdV10 = new SmdFileV10(SmdType.Reference);
      smdV10.NodeCollection.Add(new JbnLib.Smd.Node((sbyte) 0, "root", (sbyte) -1));
      List<Time> bones1 = new List<Time>()
      {
        new Time(0, new Point3D(), new Point3D())
      };
      textureopen = textureopen.TrimEnd('\\') + "\\";
      texturesave = texturesave.TrimEnd('\\') + "\\";
      Dictionary<string, Size> dictionary = new Dictionary<string, Size>();
      foreach (EntityV220 entityV220 in (List<EntityV220>) this)
      {
        sbyte num = 0;
        Point3D[] point3DArray1 = new Point3D[entityV220.BrushCollection.Count];
        Point3D[][] point3DArray2 = new Point3D[entityV220.BrushCollection.Count][];
label_23:
        for (int index1 = 0; index1 < entityV220.BrushCollection.Count; ++index1)
        {
          BrushV220 brush = entityV220.BrushCollection[index1];
          List<Point3D> point3DList = new List<Point3D>();
          for (int index2 = 0; index2 < brush.Count; ++index2)
          {
            foreach (Point3D point3D in (List<Point3D>) brush[index2].Plane)
            {
              if (!point3DList.Contains(point3D))
                point3DList.Add(point3D);
            }
          }
          Point3D point3D1 = point3DList[0];
          for (int index3 = 1; index3 < point3DList.Count; ++index3)
            point3D1 += point3DList[index3];
          Point3D point3D2 = point3D1 / (double) point3DList.Count;
          foreach (SideV220 sideV220 in (List<SideV220>) brush)
          {
            if (sideV220.Texture.ToUpper().EndsWith(MapFileV220.SpecialTextures.ORIGIN.ToString()) && num == (sbyte) 0)
            {
              smdV10.NodeCollection.Add(new JbnLib.Smd.Node(Convert.ToSByte(smdV10.NodeCollection.Count), "model" + (object) smdV10.NodeCollection.Count, (sbyte) 0));
              num = Convert.ToSByte(smdV10.NodeCollection.Count - 1);
              bones1.Add(new Time(smdV10.NodeCollection.Count - 1, new Point3D(Math.Round((double) point3D2[0], 0), Math.Round((double) point3D2[1], 0), Math.Round((double) point3D2[2], 0)), new Point3D()));
              entityV220.BrushCollection.RemoveAt(index1);
              --index1;
              goto label_23;
            }
          }
          point3DArray2[index1] = point3DList.ToArray();
          point3DArray1[index1] = point3D2;
        }
        for (int index4 = 0; index4 < entityV220.BrushCollection.Count; ++index4)
        {
          BrushV220 brush = entityV220.BrushCollection[index4];
          for (int index5 = 0; index5 < brush.Count; ++index5)
          {
            SideV220 sideV220 = brush[index5];
            if (!this.IsSpecialTexture(brush[index5].Texture))
            {
              string key = StaticMethods.GoodFileName(sideV220.Texture);
              if (!dictionary.ContainsKey(key))
              {
                if (File.Exists(textureopen + sideV220.Texture.Replace('/', '\\') + ".png"))
                {
                  Image image = Image.FromFile(textureopen + sideV220.Texture.Replace('/', '\\') + ".png");
                  dictionary.Add(key, image.Size);
                  if (texturesave != "\\" && !this.IsSpecialTexture(sideV220.Texture))
                  {
                    if (File.Exists(texturesave + key + str))
                      File.Delete(texturesave + key + str);
                    image.Save(texturesave + key + str, format);
                  }
                }
                else
                  Messages.ThrowException("MapFileV220.ToSmdV10(string, string, ImageFormat)", "Could not find texture " + textureopen + sideV220.Texture.Replace('/', '\\') + ".png");
              }
              List<Point3D> startlist = new List<Point3D>();
              foreach (Point3D point in point3DArray2[index4])
              {
                if (StaticMethods.CloseEnough(sideV220.Plane.DistanceFrom(point), 0.0, 1))
                  startlist.Add(point);
              }
              Point3D[] corners = StaticMethods.OrderTrianglePoint3Ds(startlist);
              TextureAxis u = new TextureAxis(sideV220.S.X / sideV220.Scale.X, sideV220.S.Y / sideV220.Scale.X, sideV220.S.Z / sideV220.Scale.X, sideV220.S.Shift);
              TextureAxis v = new TextureAxis(sideV220.T.X / sideV220.Scale.Y, sideV220.T.Y / sideV220.Scale.Y, sideV220.T.Z / sideV220.Scale.Y, sideV220.T.Shift);
              sbyte[] bones2 = new sbyte[corners.Length];
              for (int index6 = 0; index6 < bones2.Length; ++index6)
                bones2[index6] = num;
              VertexV10[] uvVerts = StaticMethods.StFaceToUvVerts(bones2, corners, -sideV220.Plane.Normal, new TextureMapV42(u, v), dictionary[key]);
              for (int index7 = 2; index7 < corners.Length; ++index7)
              {
                Triangle<VertexV10> triangle = new Triangle<VertexV10>();
                triangle.Texture = key;
                triangle.Vertices.Add(uvVerts[0]);
                triangle.Vertices.Add(uvVerts[index7 - 1]);
                triangle.Vertices.Add(uvVerts[index7]);
                if (new Plane3D(corners[0], corners[index7 - 1], corners[index7]).DistanceFrom(point3DArray1[index4]) >= 0.0)
                  triangle.Flip();
                smdV10.TriangleCollection.Add(triangle);
              }
            }
          }
        }
      }
      smdV10.TimeCollection.Add(new Frame(0, bones1));
      return smdV10;
    }

    public MapFileV510 ToV510()
    {
      MapFileV510 v510 = new MapFileV510();
      foreach (EntityV220 entityV220 in (List<EntityV220>) this)
        v510.Add(entityV220.ToV510());
      return v510;
    }

    public bool IsSpecialTexture(string texture) => texture.ToUpper().EndsWith(MapFileV220.SpecialTextures.BEVEL.ToString()) | texture.ToUpper().EndsWith(MapFileV220.SpecialTextures.CLIP.ToString()) | texture.ToUpper().EndsWith(MapFileV220.SpecialTextures.HINT.ToString()) | texture.ToUpper().EndsWith(MapFileV220.SpecialTextures.NULL.ToString()) | texture.ToUpper().EndsWith(MapFileV220.SpecialTextures.ORIGIN.ToString()) | texture.ToUpper().EndsWith(MapFileV220.SpecialTextures.SKIP.ToString()) | texture.ToUpper().EndsWith(MapFileV220.SpecialTextures.SKY.ToString()) | texture.ToUpper().EndsWith(MapFileV220.SpecialTextures.TRIGGER.ToString());

    public enum SpecialTextures : byte
    {
      BEVEL,
      CLIP,
      HINT,
      NULL,
      ORIGIN,
      SKIP,
      SKY,
      TRIGGER,
    }
  }
}
