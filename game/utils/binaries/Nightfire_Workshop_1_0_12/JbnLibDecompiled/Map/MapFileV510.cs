// Decompiled with JetBrains decompiler
// Type: JbnLib.Map.MapFileV510
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
using System.Text;

namespace JbnLib.Map
{
  public class MapFileV510 : List<EntityV510>, IMapFile
  {
    public void Read(string path)
    {
      EntityV510 entityV510 = (EntityV510) null;
      BrushV510 brushV510 = (BrushV510) null;
      FileTokenizer fileTokenizer = new FileTokenizer(path);
      int num = 0;
      while (!fileTokenizer.GetToken())
      {
        try
        {
          switch (fileTokenizer.Token)
          {
            case "{":
              if (entityV510 == null)
              {
                entityV510 = new EntityV510();
                continue;
              }
              brushV510 = new BrushV510();
              continue;
            case "}":
              if (brushV510 != null)
              {
                brushV510.Id = entityV510.BrushCollection.Count;
                entityV510.BrushCollection.Add(brushV510);
                brushV510 = (BrushV510) null;
                continue;
              }
              entityV510.Id = this.Count;
              this.Add(entityV510);
              entityV510 = (EntityV510) null;
              continue;
            case "(":
              SideV510 sideV510 = new SideV510();
              fileTokenizer.GetToken();
              sideV510.Plane[0][0] = Convert.ToSingle(fileTokenizer.Token);
              fileTokenizer.GetToken();
              sideV510.Plane[0][1] = Convert.ToSingle(fileTokenizer.Token);
              fileTokenizer.GetToken();
              sideV510.Plane[0][2] = Convert.ToSingle(fileTokenizer.Token);
              fileTokenizer.GetToken();
              fileTokenizer.GetToken();
              fileTokenizer.GetToken();
              sideV510.Plane[1][0] = Convert.ToSingle(fileTokenizer.Token);
              fileTokenizer.GetToken();
              sideV510.Plane[1][1] = Convert.ToSingle(fileTokenizer.Token);
              fileTokenizer.GetToken();
              sideV510.Plane[1][2] = Convert.ToSingle(fileTokenizer.Token);
              fileTokenizer.GetToken();
              fileTokenizer.GetToken();
              fileTokenizer.GetToken();
              sideV510.Plane[2][0] = Convert.ToSingle(fileTokenizer.Token);
              fileTokenizer.GetToken();
              sideV510.Plane[2][1] = Convert.ToSingle(fileTokenizer.Token);
              fileTokenizer.GetToken();
              sideV510.Plane[2][2] = Convert.ToSingle(fileTokenizer.Token);
              fileTokenizer.GetToken();
              fileTokenizer.GetToken();
              sideV510.Texture = fileTokenizer.Token;
              fileTokenizer.GetToken();
              fileTokenizer.GetToken();
              sideV510.S[0] = Convert.ToSingle(fileTokenizer.Token);
              fileTokenizer.GetToken();
              sideV510.S[1] = Convert.ToSingle(fileTokenizer.Token);
              fileTokenizer.GetToken();
              sideV510.S[2] = Convert.ToSingle(fileTokenizer.Token);
              fileTokenizer.GetToken();
              sideV510.S[3] = Convert.ToSingle(fileTokenizer.Token);
              fileTokenizer.GetToken();
              fileTokenizer.GetToken();
              fileTokenizer.GetToken();
              sideV510.T[0] = Convert.ToSingle(fileTokenizer.Token);
              fileTokenizer.GetToken();
              sideV510.T[1] = Convert.ToSingle(fileTokenizer.Token);
              fileTokenizer.GetToken();
              sideV510.T[2] = Convert.ToSingle(fileTokenizer.Token);
              fileTokenizer.GetToken();
              sideV510.T[3] = Convert.ToSingle(fileTokenizer.Token);
              fileTokenizer.GetToken();
              fileTokenizer.GetToken();
              sideV510.Rotation = Convert.ToSingle(fileTokenizer.Token);
              fileTokenizer.GetToken();
              sideV510.Scale[0] = Convert.ToSingle(fileTokenizer.Token);
              fileTokenizer.GetToken();
              sideV510.Scale[1] = Convert.ToSingle(fileTokenizer.Token);
              fileTokenizer.GetToken();
              sideV510.SurfaceFlags = (SurfaceFlags) Convert.ToInt32(fileTokenizer.Token);
              fileTokenizer.GetToken();
              sideV510.Material = (WorldMaterial) Enum.Parse(typeof (WorldMaterial), fileTokenizer.Token, true);
              fileTokenizer.GetToken();
              fileTokenizer.GetToken();
              sideV510.Lightmap[0] = Convert.ToInt32(fileTokenizer.Token);
              fileTokenizer.GetToken();
              sideV510.Lightmap[1] = Convert.ToInt32(fileTokenizer.Token);
              fileTokenizer.GetToken();
              brushV510.Add(sideV510);
              continue;
            default:
              string token = fileTokenizer.Token;
              fileTokenizer.GetToken();
              if (entityV510.ContainsKey(token))
                token += num.ToString();
              ++num;
              entityV510.Add(token, fileTokenizer.Token);
              continue;
          }
        }
        catch
        {
        }
      }
    }

    public void Read(char[] data)
    {
      EntityV510 entityV510 = (EntityV510) null;
      BrushV510 brushV510 = (BrushV510) null;
      MemoryTokenizer memoryTokenizer = new MemoryTokenizer(data);
      while (!memoryTokenizer.GetToken())
      {
        switch (memoryTokenizer.Token)
        {
          case "{":
            if (entityV510 == null)
            {
              entityV510 = new EntityV510();
              continue;
            }
            brushV510 = new BrushV510();
            continue;
          case "}":
            if (brushV510 != null)
            {
              brushV510.Id = entityV510.BrushCollection.Count;
              entityV510.BrushCollection.Add(brushV510);
              brushV510 = (BrushV510) null;
              continue;
            }
            entityV510.Id = this.Count;
            this.Add(entityV510);
            entityV510 = (EntityV510) null;
            continue;
          case "(":
            SideV510 sideV510 = new SideV510();
            memoryTokenizer.GetToken();
            sideV510.Plane[0][0] = Convert.ToSingle(memoryTokenizer.Token);
            memoryTokenizer.GetToken();
            sideV510.Plane[0][1] = Convert.ToSingle(memoryTokenizer.Token);
            memoryTokenizer.GetToken();
            sideV510.Plane[0][2] = Convert.ToSingle(memoryTokenizer.Token);
            memoryTokenizer.GetToken();
            memoryTokenizer.GetToken();
            memoryTokenizer.GetToken();
            sideV510.Plane[1][0] = Convert.ToSingle(memoryTokenizer.Token);
            memoryTokenizer.GetToken();
            sideV510.Plane[1][1] = Convert.ToSingle(memoryTokenizer.Token);
            memoryTokenizer.GetToken();
            sideV510.Plane[1][2] = Convert.ToSingle(memoryTokenizer.Token);
            memoryTokenizer.GetToken();
            memoryTokenizer.GetToken();
            memoryTokenizer.GetToken();
            sideV510.Plane[2][0] = Convert.ToSingle(memoryTokenizer.Token);
            memoryTokenizer.GetToken();
            sideV510.Plane[2][1] = Convert.ToSingle(memoryTokenizer.Token);
            memoryTokenizer.GetToken();
            sideV510.Plane[2][2] = Convert.ToSingle(memoryTokenizer.Token);
            memoryTokenizer.GetToken();
            memoryTokenizer.GetToken();
            sideV510.Texture = memoryTokenizer.Token;
            memoryTokenizer.GetToken();
            memoryTokenizer.GetToken();
            sideV510.S[0] = Convert.ToSingle(memoryTokenizer.Token);
            memoryTokenizer.GetToken();
            sideV510.S[1] = Convert.ToSingle(memoryTokenizer.Token);
            memoryTokenizer.GetToken();
            sideV510.S[2] = Convert.ToSingle(memoryTokenizer.Token);
            memoryTokenizer.GetToken();
            sideV510.S[3] = Convert.ToSingle(memoryTokenizer.Token);
            memoryTokenizer.GetToken();
            memoryTokenizer.GetToken();
            memoryTokenizer.GetToken();
            sideV510.T[0] = Convert.ToSingle(memoryTokenizer.Token);
            memoryTokenizer.GetToken();
            sideV510.T[1] = Convert.ToSingle(memoryTokenizer.Token);
            memoryTokenizer.GetToken();
            sideV510.T[2] = Convert.ToSingle(memoryTokenizer.Token);
            memoryTokenizer.GetToken();
            sideV510.T[3] = Convert.ToSingle(memoryTokenizer.Token);
            memoryTokenizer.GetToken();
            memoryTokenizer.GetToken();
            sideV510.Rotation = Convert.ToSingle(memoryTokenizer.Token);
            memoryTokenizer.GetToken();
            sideV510.Scale[0] = Convert.ToSingle(memoryTokenizer.Token);
            memoryTokenizer.GetToken();
            sideV510.Scale[1] = Convert.ToSingle(memoryTokenizer.Token);
            memoryTokenizer.GetToken();
            sideV510.SurfaceFlags = (SurfaceFlags) Convert.ToInt32(memoryTokenizer.Token);
            memoryTokenizer.GetToken();
            sideV510.Material = (WorldMaterial) Enum.Parse(typeof (WorldMaterial), memoryTokenizer.Token, true);
            memoryTokenizer.GetToken();
            memoryTokenizer.GetToken();
            sideV510.Lightmap[0] = Convert.ToInt32(memoryTokenizer.Token);
            memoryTokenizer.GetToken();
            sideV510.Lightmap[1] = Convert.ToInt32(memoryTokenizer.Token);
            memoryTokenizer.GetToken();
            brushV510.Add(sideV510);
            continue;
          default:
            string token = memoryTokenizer.Token;
            memoryTokenizer.GetToken();
            if (entityV510.ContainsKey(token))
            {
              entityV510[token] = memoryTokenizer.Token;
              continue;
            }
            entityV510.Add(token, memoryTokenizer.Token);
            continue;
        }
      }
    }

    public void Write(string path)
    {
      StreamWriter streamWriter = new StreamWriter(path);
      foreach (EntityV510 entityV510 in (List<EntityV510>) this)
      {
        streamWriter.Write(entityV510.ToString());
        streamWriter.Flush();
      }
      streamWriter.Close();
    }

    public char[] GetChars()
    {
      List<char[]> chArrayList = new List<char[]>();
      foreach (EntityV510 entityV510 in (List<EntityV510>) this)
        chArrayList.Add(entityV510.ToString().ToCharArray());
      int length = 0;
      foreach (char[] chArray in chArrayList)
        length += chArray.Length;
      char[] chars = new char[length];
      int index = 0;
      foreach (char[] chArray in chArrayList)
      {
        chArray.CopyTo((Array) chars, index);
        index += chArray.Length;
      }
      return chars;
    }

    public byte[] GetBytes() => Encoding.ASCII.GetBytes(this.GetChars());

    public char[] GetCharsNoComments()
    {
      List<char[]> chArrayList = new List<char[]>();
      foreach (EntityV510 entityV510 in (List<EntityV510>) this)
        chArrayList.Add(entityV510.ToStringNoComments().ToCharArray());
      int length = 0;
      foreach (char[] chArray in chArrayList)
        length += chArray.Length;
      char[] charsNoComments = new char[length];
      int index = 0;
      foreach (char[] chArray in chArrayList)
      {
        chArray.CopyTo((Array) charsNoComments, index);
        index += chArray.Length;
      }
      return charsNoComments;
    }

    public byte[] GetBytesNoComments() => Encoding.ASCII.GetBytes(this.GetCharsNoComments());

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
        Messages.ThrowException("MapFileV510.DumpQcV10(string, string, string, ImageFormat)", ex.Message);
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
      List<string> stringList1 = new List<string>();
      List<string> stringList2 = new List<string>();
      foreach (EntityV510 entityV510 in (List<EntityV510>) this)
      {
        foreach (List<SideV510> brush in entityV510.BrushCollection)
        {
          foreach (SideV510 sideV510 in brush)
          {
            if (!stringList1.Contains(sideV510.Texture))
              stringList1.Add(sideV510.Texture);
            List<string> stringList3 = stringList2;
            WorldMaterial material = sideV510.Material;
            string str1 = material.ToString();
            if (!stringList3.Contains(str1))
            {
              List<string> stringList4 = stringList2;
              material = sideV510.Material;
              string str2 = material.ToString();
              stringList4.Add(str2);
            }
          }
        }
      }
      int[][] numArray = new int[stringList1.Count][];
      for (int index1 = 0; index1 < stringList1.Count; ++index1)
      {
        for (int index2 = 0; index2 < stringList2.Count; ++index2)
        {
          numArray[index1] = new int[stringList1.Count];
          numArray[index1].SetValue((object) 0, index2);
        }
      }
      foreach (EntityV510 entityV510 in (List<EntityV510>) this)
      {
        foreach (List<SideV510> brush in entityV510.BrushCollection)
        {
          foreach (SideV510 sideV510 in brush)
          {
            for (int index3 = 0; index3 < stringList1.Count; ++index3)
            {
              if (stringList1[index3] == sideV510.Texture)
              {
                for (int index4 = 0; index4 < stringList2.Count; ++index4)
                {
                  if (stringList2[index4] == sideV510.Material.ToString())
                    ++numArray[index3][index4];
                }
              }
            }
          }
        }
      }
      for (int index5 = 0; index5 < stringList1.Count; ++index5)
      {
        if (!stringList1[index5].ToLower().StartsWith("special/"))
        {
          int index6 = 0;
          int num = numArray[index5][0];
          for (int index7 = 1; index7 < stringList2.Count; ++index7)
          {
            if (numArray[index5][index7] > num)
            {
              index6 = index7;
              num = numArray[index5][index7];
            }
          }
          string texture = stringList1[index5].Substring(stringList1[index5].LastIndexOf('/') + 1);
          qcFileV11.TextureCollection.Add(new CommandTexture(texture, StaticMethods.ResolveWorldToModelMaterial(stringList1[index6])));
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
        Messages.ThrowException("MapFileV510.DumpQcV11(string, string, string, ImageFormat)", ex.Message);
      }
      return flag;
    }

    public QcFileV11 GenerateQcV11(string modelname)
    {
      QcFileV11 qcV11 = new QcFileV11();
      qcV11.ModelName = new CommandModelName(modelname.ToLower().TrimEnd(".mdl".ToCharArray()) + ".mdl");
      try
      {
        List<string> stringList1 = new List<string>();
        List<string> stringList2 = new List<string>();
        foreach (EntityV510 entityV510 in (List<EntityV510>) this)
        {
          foreach (List<SideV510> brush in entityV510.BrushCollection)
          {
            foreach (SideV510 sideV510 in brush)
            {
              if (!stringList1.Contains(sideV510.Texture))
                stringList1.Add(sideV510.Texture);
              List<string> stringList3 = stringList2;
              WorldMaterial material = sideV510.Material;
              string str1 = material.ToString();
              if (!stringList3.Contains(str1))
              {
                List<string> stringList4 = stringList2;
                material = sideV510.Material;
                string str2 = material.ToString();
                stringList4.Add(str2);
              }
            }
          }
        }
        int[][] numArray = new int[stringList1.Count][];
        for (int index1 = 0; index1 < stringList1.Count; ++index1)
        {
          for (int index2 = 0; index2 < stringList2.Count; ++index2)
          {
            numArray[index1] = new int[stringList1.Count];
            numArray[index1].SetValue((object) 0, index2);
          }
        }
        foreach (EntityV510 entityV510 in (List<EntityV510>) this)
        {
          foreach (List<SideV510> brush in entityV510.BrushCollection)
          {
            foreach (SideV510 sideV510 in brush)
            {
              for (int index3 = 0; index3 < stringList1.Count; ++index3)
              {
                if (stringList1[index3] == sideV510.Texture)
                {
                  for (int index4 = 0; index4 < stringList2.Count; ++index4)
                  {
                    if (stringList2[index4] == sideV510.Material.ToString())
                      ++numArray[index3][index4];
                  }
                }
              }
            }
          }
        }
        for (int index5 = 0; index5 < stringList1.Count; ++index5)
        {
          if (!stringList1[index5].ToLower().StartsWith("special/"))
          {
            int index6 = 0;
            int num = numArray[index5][0];
            for (int index7 = 1; index7 < stringList2.Count; ++index7)
            {
              if (numArray[index5][index7] > num)
              {
                index6 = index7;
                num = numArray[index5][index7];
              }
            }
            string texture = stringList1[index5].Substring(stringList1[index5].LastIndexOf('/') + 1);
            qcV11.TextureCollection.Add(new CommandTexture(texture, StaticMethods.ResolveWorldToModelMaterial(stringList1[index6])));
          }
        }
      }
      catch (Exception ex)
      {
        Messages.ThrowException("BspV42.DumpQcV11(string, string, string, ImageFormat)", ex.Message);
      }
      return qcV11;
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
        Messages.ThrowException("MapFileV510.DumpQcV44(string, string, string, ImageFormat)", ex.Message);
      }
      return flag;
    }

    public SmdFileV10 ToSmdV10(string textureopen) => this.ToSmdV10(textureopen, "", ImageFormat.Png);

    public SmdFileV10 ToSmdV10(string textureopen, string texturesave) => this.ToSmdV10(textureopen, texturesave, ImageFormat.Png);

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
      foreach (EntityV510 entityV510 in (List<EntityV510>) this)
      {
        sbyte num = 0;
        Point3D[] point3DArray1 = new Point3D[entityV510.BrushCollection.Count];
        Point3D[][] point3DArray2 = new Point3D[entityV510.BrushCollection.Count][];
label_23:
        for (int index1 = 0; index1 < entityV510.BrushCollection.Count; ++index1)
        {
          BrushV510 brush = entityV510.BrushCollection[index1];
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
          foreach (SideV220 sideV220 in (List<SideV510>) brush)
          {
            if (sideV220.Texture.Replace('/', '\\').ToLower() == "special\\" + MapFileV510.SpecialTextures.origin.ToString() && num == (sbyte) 0)
            {
              smdV10.NodeCollection.Add(new JbnLib.Smd.Node(Convert.ToSByte(smdV10.NodeCollection.Count), "model" + (object) smdV10.NodeCollection.Count, (sbyte) 0));
              num = Convert.ToSByte(smdV10.NodeCollection.Count - 1);
              bones1.Add(new Time(smdV10.NodeCollection.Count - 1, new Point3D(Math.Round((double) point3D2[0], 0), Math.Round((double) point3D2[1], 0), Math.Round((double) point3D2[2], 0)), new Point3D()));
              entityV510.BrushCollection.RemoveAt(index1);
              --index1;
              goto label_23;
            }
          }
          point3DArray2[index1] = point3DList.ToArray();
          point3DArray1[index1] = point3D2;
        }
        for (int index4 = 0; index4 < entityV510.BrushCollection.Count; ++index4)
        {
          BrushV510 brush = entityV510.BrushCollection[index4];
          for (int index5 = 0; index5 < brush.Count; ++index5)
          {
            SideV510 sideV510 = brush[index5];
            if (!this.IsSpecialTexture(sideV510.Texture))
            {
              string key = StaticMethods.GoodFileName(sideV510.Texture);
              if (!dictionary.ContainsKey(key))
              {
                if (File.Exists(textureopen + sideV510.Texture.Replace('/', '\\') + ".png"))
                {
                  Image image = Image.FromFile(textureopen + sideV510.Texture.Replace('/', '\\') + ".png");
                  dictionary.Add(key, image.Size);
                  if (texturesave != "\\" && !this.IsSpecialTexture(sideV510.Texture))
                  {
                    if (File.Exists(texturesave + key + str))
                      File.Delete(texturesave + key + str);
                    image.Save(texturesave + key + str, format);
                  }
                }
                else
                  Messages.ThrowException("MapFileV510.ToSmdV10(string, string, ImageFormat)", "Could not find texture " + textureopen + sideV510.Texture.Replace('/', '\\') + ".png");
              }
              List<Point3D> startlist = new List<Point3D>();
              foreach (Point3D point in point3DArray2[index4])
              {
                if (StaticMethods.CloseEnough(sideV510.Plane.DistanceFrom(point), 0.0, 3))
                  startlist.Add(point);
              }
              Point3D[] corners = StaticMethods.OrderTrianglePoint3Ds(startlist);
              TextureAxis u = new TextureAxis(sideV510.S.X / sideV510.Scale.X, sideV510.S.Y / sideV510.Scale.X, sideV510.S.Z / sideV510.Scale.X, sideV510.S.Shift);
              TextureAxis v = new TextureAxis(sideV510.T.X / sideV510.Scale.Y, sideV510.T.Y / sideV510.Scale.Y, sideV510.T.Z / sideV510.Scale.Y, sideV510.T.Shift);
              sbyte[] bones2 = new sbyte[corners.Length];
              for (int index6 = 0; index6 < bones2.Length; ++index6)
                bones2[index6] = num;
              VertexV10[] uvVerts = StaticMethods.StFaceToUvVerts(bones2, corners, -sideV510.Plane.Normal, new TextureMapV42(u, v), dictionary[key]);
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

    public MapFileV220 ToV220()
    {
      MapFileV220 v220 = new MapFileV220();
      foreach (EntityV510 entityV510 in (List<EntityV510>) this)
        v220.Add(entityV510.ToV220());
      return v220;
    }

    public bool IsSpecialTexture(string texture)
    {
      string lower = texture.Replace('/', '\\').ToLower();
      return lower == "special\\" + MapFileV510.SpecialTextures.bsp.ToString() | lower == "special\\" + MapFileV510.SpecialTextures.clip.ToString() | lower == "special\\" + MapFileV510.SpecialTextures.enemyclip.ToString() | lower == "special\\" + MapFileV510.SpecialTextures.hint.ToString() | lower == "special\\" + MapFileV510.SpecialTextures.nodraw.ToString() | lower == "special\\" + MapFileV510.SpecialTextures.npcclip.ToString() | lower == "special\\" + MapFileV510.SpecialTextures.opaque.ToString() | lower == "special\\" + MapFileV510.SpecialTextures.origin.ToString() | lower == "special\\" + MapFileV510.SpecialTextures.playerclip.ToString() | lower == "special\\" + MapFileV510.SpecialTextures.skip.ToString() | lower == "special\\" + MapFileV510.SpecialTextures.sky.ToString() | lower == "special\\" + MapFileV510.SpecialTextures.trigger.ToString();
    }

    public enum SpecialTextures : byte
    {
      bsp,
      clip,
      enemyclip,
      hint,
      nodraw,
      npcclip,
      opaque,
      origin,
      playerclip,
      skip,
      sky,
      trigger,
    }
  }
}
