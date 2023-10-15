// Decompiled with JetBrains decompiler
// Type: JbnLib.Shared.StaticMethods
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Bsp;
using JbnLib.Lang;
using JbnLib.Map;
using JbnLib.Mdl;
using JbnLib.Network;
using JbnLib.Smd;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Drawing.Imaging;
using System.IO;
using System.Net;
using System.Net.Sockets;
using System.Runtime.Serialization.Formatters.Binary;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading;

namespace JbnLib.Shared
{
  public static class StaticMethods
  {
    private static string _SmdFile = "";

    internal static string SmdFile
    {
      get => StaticMethods._SmdFile;
      set
      {
        if (System.IO.File.Exists(value))
          StaticMethods._SmdFile = value;
        else
          StaticMethods._SmdFile = "";
      }
    }

    public static void Buffer16(ref BinaryWriter bw)
    {
      long num1 = bw.BaseStream.Position % 16L;
      if (num1 == 0L)
        return;
      long num2 = 16L - num1;
      for (int index = 0; (long) index < num2; ++index)
        bw.Write((byte) 0);
      bw.Flush();
    }

    public static void Buffer4(ref BinaryWriter bw)
    {
      long num1 = bw.BaseStream.Position % 4L;
      if (num1 == 0L)
        return;
      long num2 = 4L - num1;
      for (int index = 0; (long) index < num2; ++index)
        bw.Write((byte) 0);
      bw.Flush();
    }

    public static int Buffer(int length, int bufferto)
    {
      int num = length % bufferto;
      return num != 0 ? bufferto - num : num;
    }

    public static string GoodFileName(string name) => name.Substring(name.LastIndexOfAny(new char[2]
    {
      '\\',
      '/'
    }) + 1);

    public static string GoodExtension(string file, string extension)
    {
      FileInfo fileInfo = new FileInfo(file);
      return fileInfo.Name.Substring(0, fileInfo.Name.Length - fileInfo.Extension.Length) + extension;
    }

    public static void DumpLines(string path, List<string> values)
    {
      StreamWriter streamWriter = new StreamWriter(path, true);
      foreach (string str in values)
        streamWriter.WriteLine(str);
      streamWriter.WriteLine();
      streamWriter.Flush();
      streamWriter.Close();
    }

    public static void DumpLines(string path, string[] values)
    {
      StreamWriter streamWriter = new StreamWriter(path, true);
      foreach (string str in values)
        streamWriter.WriteLine(str);
      streamWriter.WriteLine();
      streamWriter.Flush();
      streamWriter.Close();
    }

    public static string[] EliminateDuplicates(string[] input)
    {
      List<string> stringList = new List<string>();
      for (int index = 0; index < input.Length; ++index)
      {
        if (!stringList.Contains(input[index]))
          stringList.Add(input[index]);
      }
      return stringList.ToArray();
    }

    public static string StripExtension(string file)
    {
      FileInfo fileInfo = new FileInfo(file);
      return fileInfo.Name.Substring(0, fileInfo.Name.Length - fileInfo.Extension.Length);
    }

    public static double DegToRad(double deg) => Math.PI / 180.0 * deg;

    public static double RadToDeg(double rad) => 180.0 / Math.PI * rad;

    public static T DeepCopy<T>(T obj)
    {
      MemoryStream serializationStream = new MemoryStream();
      BinaryFormatter binaryFormatter = new BinaryFormatter();
      binaryFormatter.Serialize((Stream) serializationStream, (object) obj);
      serializationStream.Seek(0L, SeekOrigin.Begin);
      T obj1 = (T) binaryFormatter.Deserialize((Stream) serializationStream);
      serializationStream.Close();
      return obj1;
    }

    public static StaticMethods.Quadrant GetQuadrant(float x, float y) => StaticMethods.GetQuadrant(Convert.ToDouble(x), Convert.ToDouble(y));

    public static StaticMethods.Quadrant GetQuadrant(double x, double y) => Math.Sign(x) == -1 ? (Math.Sign(y) == -1 ? StaticMethods.Quadrant.III : StaticMethods.Quadrant.II) : (Math.Sign(y) == -1 ? StaticMethods.Quadrant.IV : StaticMethods.Quadrant.I);

    public static Point3D GetIntersection(Plane3D p1, Plane3D p2, Plane3D p3)
    {
      Point3D normal1 = p1.Normal;
      Point3D normal2 = p2.Normal;
      Point3D normal3 = p3.Normal;
      double num1 = normal1.dX * p1[0].dX + normal1.dY * p1[0].dY + normal1.dZ * p1[0].dZ;
      double num2 = normal2.dX * p2[0].dX + normal2.dY * p2[0].dY + normal2.dZ * p2[0].dZ;
      double num3 = normal3.dX * p3[0].dX + normal3.dY * p3[0].dY + normal3.dZ * p3[0].dZ;
      Vector3D vector3D1 = new Vector3D(normal2, normal3);
      double num4 = new Vector3D(normal1, vector3D1.CrossProduct()).DotProduct();
      if (num4 == 0.0)
        throw new DivideByZeroException();
      Vector3D vector3D2 = new Vector3D(normal3, normal1);
      Vector3D vector3D3 = new Vector3D(normal1, normal2);
      Point3D point3D = vector3D1.CrossProduct();
      return (num1 * point3D + num2 * vector3D2.CrossProduct() + num3 * vector3D3.CrossProduct()) / num4;
    }

    public static bool Contains(Point3D needle, List<Point3D> haystack, float tolerance)
    {
      for (int index = 0; index < haystack.Count; ++index)
      {
        if ((double) Math.Abs(needle.X - haystack[index].X) <= (double) tolerance && (double) Math.Abs(needle.Y - haystack[index].Y) <= (double) tolerance && (double) Math.Abs(needle.Z - haystack[index].Z) <= (double) tolerance)
          return true;
      }
      return false;
    }

    public static int Compare(byte[] a, byte[] b)
    {
      int num1 = 0;
      int num2 = a.Length <= b.Length ? (a.Length >= b.Length ? a.Length : b.Length) : a.Length;
      for (int index = 0; index < num2; ++index)
        num1 += (int) b[index] - (int) a[index];
      return num1;
    }

    public static Point3D Transform(Point3D pnt, Matrix tfrm)
    {
      if (tfrm.Rows >= 3 && tfrm.Cols > 3)
        return new Point3D(pnt.dX * tfrm[0, 0] + pnt.dY * tfrm[0, 1] + pnt.dZ * tfrm[0, 2] + tfrm[0, 3], pnt.dX * tfrm[1, 0] + pnt.dY * tfrm[1, 1] + pnt.dZ * tfrm[1, 2] + tfrm[1, 3], pnt.dX * tfrm[2, 0] + pnt.dY * tfrm[2, 1] + pnt.dZ * tfrm[2, 2] + tfrm[2, 3]);
      if (tfrm.Rows >= 3 && tfrm.Cols == 3)
        return new Point3D(pnt.dX * tfrm[0, 0] + pnt.dY * tfrm[0, 1] + pnt.dZ * tfrm[0, 2], pnt.dX * tfrm[1, 0] + pnt.dY * tfrm[1, 1] + pnt.dZ * tfrm[1, 2], pnt.dX * tfrm[2, 0] + pnt.dY * tfrm[2, 1] + pnt.dZ * tfrm[2, 2]);
      Messages.ThrowException("StaticMethods.Transform(Point3D, Matrix)", "Matrix must be at least 3x3 or bigger.");
      return new Point3D();
    }

    public static Matrix InvRotate(Point3D angles)
    {
      double num1 = angles.dZ * (Math.PI / 180.0);
      double num2 = Math.Sin(num1);
      double num3 = Math.Cos(num1);
      double num4 = angles.dY * (Math.PI / 180.0);
      double num5 = Math.Sin(num4);
      double num6 = Math.Cos(num4);
      double num7 = angles.dX * (Math.PI / 180.0);
      double num8 = Math.Sin(num7);
      double num9 = Math.Cos(num7);
      return new Matrix(new double[4, 3]
      {
        {
          num6 * num3,
          num6 * num2,
          -num5
        },
        {
          num8 * num5 * num3 + num9 * -num2,
          num8 * num5 * num2 + num9 * num3,
          num8 * num6
        },
        {
          num9 * num5 * num3 + -num8 * -num2,
          num9 * num5 * num2 + -num8 * num3,
          num9 * num6
        },
        {
          0.0,
          0.0,
          0.0
        }
      });
    }

    public static Matrix Multiply3x4(Matrix a, Matrix b)
    {
      if (a.Rows != 3 || a.Cols != 4 || b.Rows != 3 || b.Cols != 4)
      {
        Messages.ThrowException("StaticMethods.Multiply3x4(Matrix, Matrix)", "Arguments must both be 3x4 matrices.");
        return new Matrix(1, 1);
      }
      return new Matrix(new double[3, 4]
      {
        {
          a[0, 0] * b[0, 0] + a[0, 1] * b[1, 0] + a[0, 2] * b[2, 0],
          a[0, 0] * b[0, 1] + a[0, 1] * b[1, 1] + a[0, 2] * b[2, 1],
          a[0, 0] * b[0, 2] + a[0, 1] * b[1, 2] + a[0, 2] * b[2, 2],
          a[0, 0] * b[0, 3] + a[0, 1] * b[1, 3] + a[0, 2] * b[2, 3] + a[0, 3]
        },
        {
          a[1, 0] * b[0, 0] + a[1, 1] * b[1, 0] + a[1, 2] * b[2, 0],
          a[1, 0] * b[0, 1] + a[1, 1] * b[1, 1] + a[1, 2] * b[2, 1],
          a[1, 0] * b[0, 2] + a[1, 1] * b[1, 2] + a[1, 2] * b[2, 2],
          a[1, 0] * b[0, 3] + a[1, 1] * b[1, 3] + a[1, 2] * b[2, 3] + a[1, 3]
        },
        {
          a[2, 0] * b[0, 0] + a[2, 1] * b[1, 0] + a[2, 2] * b[2, 0],
          a[2, 0] * b[0, 1] + a[2, 1] * b[1, 1] + a[2, 2] * b[2, 1],
          a[2, 0] * b[0, 2] + a[2, 1] * b[1, 2] + a[2, 2] * b[2, 2],
          a[2, 0] * b[0, 3] + a[2, 1] * b[1, 3] + a[2, 2] * b[2, 3] + a[2, 3]
        }
      });
    }

    public static Point3D[] OrderTrianglePoint3Ds(List<Point3D> startlist)
    {
      Point3D[] point3DArray = new Point3D[startlist.Count];
      if (startlist.Count < 3)
        Messages.ThrowException("StaticMethods.OrderTrianglePoint3Ds(List<Point3D>)", "Invalid list of face vertices. Count is less than 3.");
      int index1 = 2;
      try
      {
        Point3D initial = new Point3D(startlist[1].dX - startlist[0].dX, startlist[1].dY - startlist[0].dY, startlist[1].dZ - startlist[0].dZ);
        double num1 = new Vector3D(initial, new Point3D(startlist[index1].dX - startlist[0].dX, startlist[index1].dY - startlist[0].dY, startlist[index1].dZ - startlist[0].dZ)).Angle();
        for (int index2 = 3; index2 < startlist.Count; ++index2)
        {
          double num2 = new Vector3D(initial, new Point3D(startlist[index2].dX - startlist[0].dX, startlist[index2].dY - startlist[0].dY, startlist[index2].dZ - startlist[0].dZ)).Angle();
          if (num2 > num1)
          {
            num1 = num2;
            index1 = index2;
          }
        }
      }
      catch (DivideByZeroException ex)
      {
        Messages.ThrowException("StaticMethods.OrderTrianglePoint3Ds(List<Point3D>)", "Cannot order points if some are identical!");
      }
      point3DArray[0] = startlist[0];
      point3DArray[1] = startlist[index1];
      Point3D[] array = new Point3D[startlist.Count - 2];
      for (int index3 = 1; index3 < startlist.Count; ++index3)
      {
        if (index3 != index1)
        {
          int index4 = index3 - 1;
          if (index3 > index1)
            --index4;
          array[index4] = startlist[index3];
        }
      }
      Array.Sort<Point3D>(array, (IComparer<Point3D>) new Vector3D(point3DArray[0], point3DArray[1]));
      array.CopyTo((Array) point3DArray, 2);
      return point3DArray;
    }

    public static string ResolveV510ToV220SpecialTexture(string texture)
    {
      switch (texture)
      {
        case "special/bsp":
        case "special/clip":
        case "special/enemyclip":
        case "special/npcclip":
        case "special/opaque":
        case "special/playerclip":
          return MapFileV220.SpecialTextures.CLIP.ToString();
        case "special/hint":
          return MapFileV220.SpecialTextures.HINT.ToString();
        case "special/nodraw":
          return MapFileV220.SpecialTextures.NULL.ToString();
        case "special/origin":
          return MapFileV220.SpecialTextures.ORIGIN.ToString();
        case "special/skip":
          return MapFileV220.SpecialTextures.SKIP.ToString();
        case "special/sky":
          return MapFileV220.SpecialTextures.SKY.ToString();
        case "special/trigger":
          return MapFileV220.SpecialTextures.TRIGGER.ToString();
        default:
          return texture;
      }
    }

    public static string ResolveV220ToV510SpecialTexture(string texture)
    {
      if (texture.Contains(MapFileV220.SpecialTextures.BEVEL.ToString()) | texture.Contains(MapFileV220.SpecialTextures.SKIP.ToString()))
        return "special/skip";
      if (texture.Contains(MapFileV220.SpecialTextures.CLIP.ToString()))
        return "special/clip";
      if (texture.Contains(MapFileV220.SpecialTextures.HINT.ToString()))
        return "special/hint";
      if (texture.Contains(MapFileV220.SpecialTextures.NULL.ToString()))
        return "special/nodraw";
      if (texture.Contains(MapFileV220.SpecialTextures.ORIGIN.ToString()))
        return "special/origin";
      if (texture.Contains(MapFileV220.SpecialTextures.SKY.ToString()))
        return "special/sky";
      return texture.Contains(MapFileV220.SpecialTextures.TRIGGER.ToString()) ? "special/trigger" : texture;
    }

    public static ModelMaterial ResolveWorldToModelMaterial(WorldMaterial wld)
    {
      switch (wld)
      {
        case WorldMaterial.wld_chrome:
          return ModelMaterial.mdl_chrome;
        case WorldMaterial.wld_glass:
        case WorldMaterial.wld_glassadditive:
        case WorldMaterial.wld_glasssimple:
        case WorldMaterial.wld_multiglass:
          return ModelMaterial.mdl_additive;
        case WorldMaterial.wld_lightmapselfillum:
          return ModelMaterial.mdl_basicselfillum;
        case WorldMaterial.wld_masked:
        case WorldMaterial.wld_maskedenvmapglass:
        case WorldMaterial.wld_maskedenvmapsolid:
        case WorldMaterial.wld_maskedmultiglass:
        case WorldMaterial.wld_maskedmultisolid:
          return ModelMaterial.mdl_masked;
        default:
          return ModelMaterial.mdl_basic;
      }
    }

    public static string ResolveWorldToModelMaterial(string wld)
    {
      switch (wld.ToLower())
      {
        case "wld_chrome":
          return "mdl_chrome";
        case "wld_glass":
        case "wld_glassadditive":
        case "wld_glasssimple":
        case "wld_multiglass":
          return "mdl_additive";
        case "wld_lightmapselfillum":
          return "mdl_basicselfillum";
        case "wld_masked":
        case "wld_maskedenvmapglass":
        case "wld_maskedenvmapsolid":
        case "wld_maskedmultiglass":
        case "wld_maskedmultisolid":
          return "mdl_masked";
        default:
          return "mdl_basic";
      }
    }

    public static WorldMaterial ResolveModelToWorldMaterial(ModelMaterial mdl)
    {
      switch (mdl)
      {
        case ModelMaterial.mdl_additive:
        case ModelMaterial.mdl_oceanwaves:
        case ModelMaterial.mdl_specular:
        case ModelMaterial.mdl_water:
          return WorldMaterial.wld_glass;
        case ModelMaterial.mdl_basicselfillum:
        case ModelMaterial.mdl_basicselfillumnofade:
          return WorldMaterial.wld_lightmapselfillum;
        case ModelMaterial.mdl_chrome:
          return WorldMaterial.wld_chrome;
        case ModelMaterial.mdl_cloud:
        case ModelMaterial.mdl_lightningcloud:
          return WorldMaterial.wld_envmapglass;
        case ModelMaterial.mdl_masked:
          return WorldMaterial.wld_masked;
        case ModelMaterial.mdl_terrain:
        case ModelMaterial.mdl_terrain_hazed:
          return WorldMaterial.wld_envmapsolid;
        default:
          return WorldMaterial.wld_lightmap;
      }
    }

    public static string ResolveModelToWorldMaterial(string mdl)
    {
      switch (mdl.ToLower())
      {
        case "mdl_additive":
        case "mdl_oceanwaves":
        case "mdl_specular":
        case "mdl_water":
          return "wld_glass";
        case "mdl_basicselfillum":
        case "mdl_basicselfillumnofade":
          return "wld_lightmapselfillum";
        case "mdl_chrome":
          return "wld_chrome";
        case "mdl_cloud":
        case "mdl_lightningcloud":
          return "wld_envmapglass";
        case "mdl_masked":
          return "wld_masked";
        case "mdl_terrain":
        case "mdl_terrain_hazed":
          return "wld_envmapsolid";
        default:
          return "wld_lightmap";
      }
    }

    public static string CheckImgExtension(ImageFormat format)
    {
      if (format == ImageFormat.Bmp)
        return ".bmp";
      if (format == ImageFormat.Gif)
        return ".gif";
      if (format == ImageFormat.Jpeg)
        return ".jpg";
      if (format == ImageFormat.Tiff)
        return ".tif";
      if (format == ImageFormat.Emf || format == ImageFormat.MemoryBmp || format == ImageFormat.Png || format == ImageFormat.Wmf)
        return ".png";
      throw new Exception("Saving as this image format is not supported.");
    }

    public static VertexV10[] StFaceToUvVerts(
      Point3D[] corners,
      Point3D norm,
      TextureMapV42 st,
      Size texture)
    {
      sbyte[] bones = new sbyte[corners.Length];
      for (int index = 0; index < bones.Length; ++index)
        bones[index] = (sbyte) 0;
      return StaticMethods.StFaceToUvVerts(bones, corners, norm, st, texture);
    }

    public static VertexV10[] StFaceToUvVerts(
      sbyte[] bones,
      Point3D[] corners,
      Point3D norm,
      TextureMapV42 st,
      Size texture)
    {
      VertexV10[] uvVerts = new VertexV10[corners.Length];
      if (texture.Width == 0 || texture.Height == 0)
      {
        for (int index = 0; index < corners.Length; ++index)
          uvVerts[index] = new VertexV10(bones[index], corners[index], norm, new TextureCoordinate(0.0f, 0.0f));
      }
      else
      {
        Point3D point3D1 = st.S.GetPoint3D();
        Point3D point3D2 = st.T.GetPoint3D();
        Point3D point3D3 = new Point3D((float) ((double) point3D1[1] * (double) point3D2[2] - (double) point3D1[2] * (double) point3D2[1]), (float) ((double) point3D1[2] * (double) point3D2[0] - (double) point3D1[0] * (double) point3D2[2]), (float) ((double) point3D1[0] * (double) point3D2[1] - (double) point3D1[1] * (double) point3D2[0]));
        Matrix matrix1 = new Matrix(new double[3, 3]
        {
          {
            -(double) point3D3.X,
            (double) point3D1.X,
            (double) point3D2.X
          },
          {
            -(double) point3D3.Y,
            (double) point3D1.Y,
            (double) point3D2.Y
          },
          {
            -(double) point3D3.Z,
            (double) point3D1.Z,
            (double) point3D2.Z
          }
        }).Inverse();
        for (int index = 0; index < corners.Length; ++index)
        {
          Matrix matrix2 = matrix1 * new Matrix(new double[3, 1]
          {
            {
              (double) corners[index].X
            },
            {
              (double) corners[index].Y
            },
            {
              (double) corners[index].Z
            }
          });
          if (matrix2 == null)
            Messages.ThrowException("StaticMethods.StFaceToUvVerts(sbyte[], Point3D[], Point3D, TextureMapV42, Size)", "Texture axes are parallel, normal is unresolvable. Please check the texture on the brush face with corners: \n" + corners.ToString());
          uvVerts[index] = new VertexV10(bones[index], corners[index], norm, new TextureCoordinate(Convert.ToSingle((matrix2[1, 0] * point3D1.SumComponentSqrs() + (double) st.S.Shift) / (double) texture.Width), Convert.ToSingle(-(matrix2[2, 0] * point3D2.SumComponentSqrs() + (double) st.T.Shift) / (double) texture.Height)));
        }
      }
      return uvVerts;
    }

    public static TextureCoordinate[] StFaceToUvFace(
      Point3D[] corners,
      TextureMapV42 st,
      Size texture)
    {
      Point3D point3D1 = st.S.GetPoint3D();
      Point3D point3D2 = st.T.GetPoint3D();
      Point3D point3D3 = new Point3D((float) ((double) point3D1[1] * (double) point3D2[2] - (double) point3D1[2] * (double) point3D2[1]), (float) ((double) point3D1[2] * (double) point3D2[0] - (double) point3D1[0] * (double) point3D2[2]), (float) ((double) point3D1[0] * (double) point3D2[1] - (double) point3D1[1] * (double) point3D2[0]));
      Matrix matrix1 = new Matrix(new double[3, 3]
      {
        {
          -(double) point3D3.X,
          (double) point3D1.X,
          (double) point3D2.X
        },
        {
          -(double) point3D3.Y,
          (double) point3D1.Y,
          (double) point3D2.Y
        },
        {
          -(double) point3D3.Z,
          (double) point3D1.Z,
          (double) point3D2.Z
        }
      }).Inverse();
      TextureCoordinate[] uvFace = new TextureCoordinate[corners.Length];
      for (int index = 0; index < corners.Length; ++index)
      {
        Matrix matrix2 = matrix1 * new Matrix(new double[3, 1]
        {
          {
            (double) corners[index].X
          },
          {
            (double) corners[index].Y
          },
          {
            (double) corners[index].Z
          }
        });
        uvFace[index] = new TextureCoordinate(Convert.ToSingle((matrix2[1, 0] * point3D1.SumComponentSqrs() + (double) st.S.Shift) / (double) texture.Width), Convert.ToSingle(-(matrix2[2, 0] * point3D2.SumComponentSqrs() + (double) st.T.Shift) / (double) texture.Height));
      }
      return uvFace;
    }

    public static SideV510 UvFaceToStFace(Triangle<VertexV10> tri, Size texture)
    {
      SideV510 stFace = new SideV510();
      stFace._Plane = new Plane3D(tri.Vertices[0].Position, tri.Vertices[1].Position, tri.Vertices[2].Position);
      stFace._Texture = tri.Texture.ToLower().TrimEnd(".png".ToCharArray()).TrimEnd(".bmp".ToCharArray());
      float[] numArray1 = new float[3]
      {
        tri.Vertices[0].TextureCoordinate.U * (float) texture.Width,
        tri.Vertices[1].TextureCoordinate.U * (float) texture.Width,
        tri.Vertices[2].TextureCoordinate.U * (float) texture.Width
      };
      float[] numArray2 = new float[3]
      {
        tri.Vertices[0].TextureCoordinate.V * (float) texture.Height,
        tri.Vertices[1].TextureCoordinate.V * (float) texture.Height,
        tri.Vertices[2].TextureCoordinate.V * (float) texture.Height
      };
      Vector2D vector2D1 = new Vector2D(new Point(numArray1[0], numArray2[0]), new Point(numArray1[1], numArray2[1]));
      Vector2D vector2D2 = new Vector2D(vector2D1[0], new Point(numArray1[2], numArray2[2]));
      Vector3D vector3D1 = new Vector3D(tri.Vertices[0].Position, tri.Vertices[1].Position);
      Vector3D vector3D2 = new Vector3D(tri.Vertices[0].Position, tri.Vertices[2].Position);
      Matrix matrix1 = new Matrix(new double[2, 2]
      {
        {
          (double) vector2D1.Difference().X,
          (double) vector2D2.Difference().X
        },
        {
          (double) vector2D1.Difference().Y,
          (double) vector2D2.Difference().Y
        }
      }).Inverse() * new Matrix(new double[2, 1]
      {
        {
          -(double) numArray1[0]
        },
        {
          -(double) numArray2[0]
        }
      });
      Point3D point3D1 = tri.Vertices[0].Position + vector3D1.Difference() * matrix1[0, 0] + vector3D2.Difference() * matrix1[0, 0];
      Matrix matrix2 = new Matrix(new double[2, 2]
      {
        {
          (double) vector2D1.Difference().X,
          (double) vector2D2.Difference().X
        },
        {
          (double) vector2D1.Difference().Y,
          (double) vector2D2.Difference().Y
        }
      }).Inverse() * new Matrix(new double[2, 1]
      {
        {
          1.0 - (double) numArray1[0]
        },
        {
          -(double) numArray2[0]
        }
      });
      Matrix matrix3 = new Matrix(new double[2, 2]
      {
        {
          (double) vector2D1.Difference().X,
          (double) vector2D2.Difference().X
        },
        {
          (double) vector2D1.Difference().Y,
          (double) vector2D2.Difference().Y
        }
      }).Inverse() * new Matrix(new double[2, 1]
      {
        {
          (double) numArray1[0]
        },
        {
          1.0 - (double) numArray2[0]
        }
      });
      Point3D point3D2 = tri.Vertices[0].Position + vector3D1.Difference() * matrix2[0, 0] + vector3D2.Difference() * matrix1[1, 0] - point3D1;
      Point3D point3D3 = tri.Vertices[0].Position + vector3D1.Difference() * matrix3[0, 0] + vector3D2.Difference() * matrix1[1, 0] - point3D1;
      Point3D point3D4 = (point3D2 - point3D1).Normalize();
      Point3D point3D5 = -(point3D3 - point3D1).Normalize();
      Point3D normal = new Plane3D(tri.Vertices[0].Position, tri.Vertices[1].Position, tri.Vertices[2].Position).Normal;
      Matrix matrix4 = new Matrix(new double[3, 3]
      {
        {
          (double) point3D4.X,
          (double) point3D5.X,
          -(double) normal.X
        },
        {
          (double) point3D4.Y,
          (double) point3D5.Y,
          -(double) normal.Y
        },
        {
          (double) point3D4.Z,
          (double) point3D5.Z,
          -(double) normal.Z
        }
      }).Inverse() * new Matrix(new double[3, 1]
      {
        {
          -(double) point3D1.X
        },
        {
          -(double) point3D1.Y
        },
        {
          -(double) point3D1.Z
        }
      });
      Point3D point3D6 = (point3D2 - point3D1) / (point3D2 - point3D1).SumComponentSqrs();
      Point3D point3D7 = -(point3D3 - point3D1) / (point3D3 - point3D1).SumComponentSqrs();
      stFace._S = new TextureAxis(point3D6.X, point3D6.Y, point3D6.Z, Convert.ToSingle(matrix4[0, 0]));
      stFace._T = new TextureAxis(point3D7.X, point3D7.Y, point3D7.Z, Convert.ToSingle(matrix4[1, 0]));
      stFace.Scale.X = 1f / Convert.ToSingle((point3D2 - point3D1).Magnitude);
      stFace.Scale.Y = 1f / Convert.ToSingle((point3D3 - point3D1).Magnitude);
      return stFace;
    }

    public static bool CloseEnough(double a, double b, int decimals) => Math.Round(a, decimals) == Math.Round(b, decimals);

    public static string FormatDateTime(DateTime a) => a.Month.ToString().PadLeft(2, '0') + "/" + a.Day.ToString().PadLeft(2, '0') + "/" + a.Year.ToString() + " - " + a.Hour.ToString().PadLeft(2, '0') + ":" + a.Minute.ToString().PadLeft(2, '0') + ":" + a.Second.ToString().PadLeft(2, '0');

    public static string DeleteLineComment(string line) => line.Contains("//") ? line.Substring(0, line.LastIndexOf("//")).Trim() : line;

    public static string PullData(string data, string name)
    {
      int startIndex = data.ToLower().IndexOf(name.ToLower() + "(");
      return startIndex != -1 ? data.Substring(startIndex + name.Length + 1, data.IndexOf(')', startIndex) - (startIndex + name.Length + 1)) : "";
    }

    public static string PullQuoted(string data)
    {
      int num = data.ToLower().IndexOf('"');
      return num != -1 ? data.Substring(num + 1, data.LastIndexOf('"') - num - 1) : "";
    }

    public static string MagicQuotes(string data)
    {
      try
      {
        return Convert.ToInt64(data).ToString();
      }
      catch
      {
        return "\"" + data + "\"";
      }
    }

    public static bool IsValidIP(string ip) => ip.Length != 0 && new Regex("^([1-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])(\\.([0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])){3}$").IsMatch(ip);

    public static bool IsValidPort(string port)
    {
      try
      {
        int uint16 = (int) Convert.ToUInt16(port);
        return true;
      }
      catch
      {
        return false;
      }
    }

    public static IPAddress DnsLookup(string domain)
    {
      try
      {
        IPAddress[] addressList = Dns.GetHostEntry(domain).AddressList;
        return addressList.Length != 0 ? addressList[0] : (IPAddress) null;
      }
      catch
      {
        return (IPAddress) null;
      }
    }

    public static IPEndPoint ToEndPoint(string ipport)
    {
      string[] strArray = ipport.Split(':');
      return StaticMethods.IsValidIP(strArray[0]) && StaticMethods.IsValidPort(strArray[1]) ? new IPEndPoint(IPAddress.Parse(strArray[0]), (int) Convert.ToUInt16(strArray[1])) : (IPEndPoint) null;
    }

    public static IPEndPoint ToEndPoint(IPPort ipport) => StaticMethods.IsValidIP(ipport.IP) ? new IPEndPoint(IPAddress.Parse(ipport.IP), ipport.Port) : (IPEndPoint) null;

    public static IPEndPoint ToEndPoint(string ip, string port) => StaticMethods.IsValidIP(ip) && StaticMethods.IsValidPort(port) ? new IPEndPoint(IPAddress.Parse(ip), (int) Convert.ToUInt16(port)) : (IPEndPoint) null;

    public static IPEndPoint ToEndPoint(string ip, ushort port) => StaticMethods.IsValidIP(ip) ? new IPEndPoint(IPAddress.Parse(ip), (int) port) : (IPEndPoint) null;

    public static IPEndPoint ToEndPoint(IPAddress ip, string port) => StaticMethods.IsValidPort(port) ? new IPEndPoint(ip, (int) Convert.ToUInt16(port)) : (IPEndPoint) null;

    public static IPEndPoint ToEndPoint(IPAddress ip, ushort port) => new IPEndPoint(ip, (int) port);

    public static ServerStatus GetServerStatus(IPEndPoint server)
    {
      try
      {
        ServerStatus serverStatus = new ServerStatus();
        string raw = "";
        UdpClient udpClient = new UdpClient();
        byte[] bytes = Encoding.ASCII.GetBytes("\\status\\");
        udpClient.Send(bytes, bytes.Length, server);
        DateTime now = DateTime.Now;
        for (byte index = 0; index < (byte) 100; ++index)
        {
          if (udpClient.Available > 0)
          {
            raw = Encoding.ASCII.GetString(udpClient.Receive(ref server));
            TimeSpan timeSpan = DateTime.Now - now;
            serverStatus.Ping = timeSpan.Milliseconds;
            break;
          }
          Thread.Sleep(10);
        }
        udpClient.Close();
        serverStatus.EndPointQuery = server;
        if (raw.Length <= 0)
          return (ServerStatus) null;
        string str = "";
        while (str != "FALSE")
        {
          str = StaticMethods.GetToken(ref raw);
          switch (str)
          {
            case "bots_mode":
              serverStatus.BotsMode = Convert.ToByte(StaticMethods.GetToken(ref raw));
              str = "FALSE";
              continue;
            case "ctflimit":
              serverStatus.CTFLimit = Convert.ToInt32(StaticMethods.GetToken(ref raw));
              continue;
            case "dedicated":
              serverStatus.Dedicated = StaticMethods.GetToken(ref raw) == "1";
              continue;
            case "fraglimit":
              serverStatus.FragLimit = Convert.ToInt32(StaticMethods.GetToken(ref raw));
              continue;
            case "friendlyfire":
              serverStatus.FriendlyFire = StaticMethods.GetToken(ref raw) == "1";
              continue;
            case "gamemode":
              serverStatus.GameMode = StaticMethods.GetToken(ref raw);
              continue;
            case "gamename":
              serverStatus.GameName = StaticMethods.GetToken(ref raw);
              continue;
            case "gametype":
              serverStatus.GameType = StaticMethods.GetToken(ref raw);
              continue;
            case "gamever":
              serverStatus.GameVer = StaticMethods.GetToken(ref raw);
              continue;
            case "hostname":
              serverStatus.HostName = StaticMethods.GetToken(ref raw);
              continue;
            case "hostport":
              serverStatus.HostPort = Convert.ToUInt16(StaticMethods.GetToken(ref raw));
              serverStatus.EndPointHost = StaticMethods.ToEndPoint(server.Address, serverStatus.HostPort);
              continue;
            case "mapname":
              serverStatus.MapName = StaticMethods.GetToken(ref raw);
              continue;
            case "maxplayers":
              serverStatus.MaxPlayers = Convert.ToByte(StaticMethods.GetToken(ref raw));
              continue;
            case "numplayers":
              serverStatus.NumPlayers = Convert.ToByte(StaticMethods.GetToken(ref raw));
              continue;
            case "password":
              serverStatus.Password = StaticMethods.GetToken(ref raw) == "1";
              continue;
            case "teamplay":
              serverStatus.TeamPlay = StaticMethods.GetToken(ref raw) == "1";
              continue;
            case "timelimit":
              serverStatus.TimeLimit = Convert.ToInt32(StaticMethods.GetToken(ref raw));
              continue;
            case "weaponstay":
              serverStatus.WeaponStay = StaticMethods.GetToken(ref raw) == "1";
              continue;
            default:
              continue;
          }
        }
        string token1;
        for (token1 = StaticMethods.GetToken(ref raw); token1.StartsWith("player_"); token1 = StaticMethods.GetToken(ref raw))
        {
          string token2 = StaticMethods.GetToken(ref raw);
          StaticMethods.GetToken(ref raw);
          int int32_1 = Convert.ToInt32(StaticMethods.GetToken(ref raw));
          StaticMethods.GetToken(ref raw);
          int int32_2 = Convert.ToInt32(StaticMethods.GetToken(ref raw));
          serverStatus.Players.Add(new ServerStatusPlayer(token2, int32_1, int32_2));
        }
        if (token1 == "final")
          StaticMethods.GetToken(ref raw);
        if (StaticMethods.GetToken(ref raw) == "queryid")
          serverStatus.QueryID = Convert.ToSingle(StaticMethods.GetToken(ref raw));
        return serverStatus;
      }
      catch
      {
        return (ServerStatus) null;
      }
    }

    private static string GetToken(ref string raw)
    {
      string token = "";
      if (raw[0] == '\\')
      {
        int startIndex = raw.IndexOf('\\', 1);
        if (startIndex == -1)
        {
          token = raw.Substring(1).Trim();
          raw = "";
        }
        else
        {
          token = raw.Substring(1, startIndex - 1);
          raw = raw.Substring(startIndex);
        }
      }
      return token;
    }

    public enum Quadrant : byte
    {
      I = 1,
      II = 2,
      III = 3,
      IV = 4,
    }
  }
}
