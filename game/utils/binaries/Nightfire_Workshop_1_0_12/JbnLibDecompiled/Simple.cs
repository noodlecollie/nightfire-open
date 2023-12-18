// Decompiled with JetBrains decompiler
// Type: JbnLib.Simple
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using FreeImageAPI;
using JbnLib.Mdl;
using JbnLib.Qc;
using JbnLib.Smd;
using JbnLib.Sp2;
using JbnLib.Spz;
using System.IO;

namespace JbnLib
{
  public static class Simple
  {
    public static void QcV10ToV11(string loadpath, string savepath)
    {
      QcFileV10 qcFileV10 = new QcFileV10();
      qcFileV10.Read(loadpath);
      qcFileV10.ToV11().Write(savepath);
    }

    public static void QcV10ToV44(string loadpath, string savepath)
    {
      QcFileV10 qcFileV10 = new QcFileV10();
      qcFileV10.Read(loadpath);
      qcFileV10.ToV44().Write(savepath);
    }

    public static void QcV11ToV10(string loadpath, string savepath)
    {
      QcFileV11 qcFileV11 = new QcFileV11();
      qcFileV11.Read(loadpath);
      qcFileV11.ToV10().Write(savepath);
    }

    public static void QcV11ToV44(string loadpath, string savepath)
    {
      QcFileV11 qcFileV11 = new QcFileV11();
      qcFileV11.Read(loadpath);
      qcFileV11.ToV44().Write(savepath);
    }

    public static void QcV44ToV10(string loadpath, string savepath)
    {
      QcFileV44 qcFileV44 = new QcFileV44();
      qcFileV44.Read(loadpath);
      qcFileV44.ToV10().Write(savepath);
    }

    public static void QcV44ToV11(string loadpath, string savepath)
    {
      QcFileV44 qcFileV44 = new QcFileV44();
      qcFileV44.Read(loadpath);
      qcFileV44.ToV11().Write(savepath);
    }

    public static string[] SmdV10ToV11(string loadpath, string savepath)
    {
      SmdFileV10 smdFileV10 = !SmdFile.HasTriangles(loadpath) ? new SmdFileV10(SmdType.Animation) : new SmdFileV10(SmdType.Reference);
      smdFileV10.Read(loadpath);
      SmdFileV11 v11 = smdFileV10.ToV11();
      v11.Write(savepath);
      return v11.GetTextures();
    }

    public static string[] SmdV10ToV44(string loadpath, string savepath)
    {
      SmdFileV10 smdFileV10 = !SmdFile.HasTriangles(loadpath) ? new SmdFileV10(SmdType.Animation) : new SmdFileV10(SmdType.Reference);
      smdFileV10.Read(loadpath);
      SmdFileV44 v44 = smdFileV10.ToV44();
      v44.Write(savepath);
      return v44.GetTextures();
    }

    public static string[] SmdV11ToV10(string loadpath, string savepath)
    {
      SmdFileV11 smdFileV11 = !SmdFile.HasTriangles(loadpath) ? new SmdFileV11(SmdType.Animation) : new SmdFileV11(SmdType.Reference);
      smdFileV11.Read(loadpath);
      SmdFileV10 v10 = smdFileV11.ToV10();
      v10.Write(savepath);
      return v10.GetTextures();
    }

    public static string[] SmdV11ToV44(string loadpath, string savepath)
    {
      SmdFileV11 smdFileV11 = !SmdFile.HasTriangles(loadpath) ? new SmdFileV11(SmdType.Animation) : new SmdFileV11(SmdType.Reference);
      smdFileV11.Read(loadpath);
      SmdFileV44 v44 = smdFileV11.ToV44();
      v44.Write(savepath);
      return v44.GetTextures();
    }

    public static string[] SmdV44ToV10(string loadpath, string savepath)
    {
      SmdFileV44 smdFileV44 = !SmdFile.HasTriangles(loadpath) ? new SmdFileV44(SmdType.Animation) : new SmdFileV44(SmdType.Reference);
      smdFileV44.Read(loadpath);
      SmdFileV10 v10 = smdFileV44.ToV10();
      v10.Write(savepath);
      return v10.GetTextures();
    }

    public static string[] SmdV44ToV11(string loadpath, string savepath)
    {
      SmdFileV44 smdFileV44 = !SmdFile.HasTriangles(loadpath) ? new SmdFileV44(SmdType.Animation) : new SmdFileV44(SmdType.Reference);
      smdFileV44.Read(loadpath);
      SmdFileV11 v11 = smdFileV44.ToV11();
      v11.Write(savepath);
      return v11.GetTextures();
    }

    public static string[] DumpV14ToV10(string model, string qc)
    {
      MdlFileV14 mdlFileV14 = new MdlFileV14();
      mdlFileV14.Read(model);
      return mdlFileV14.DumpV10Qc(qc);
    }

    public static string[] DumpV14ToV11(string model, string qc)
    {
      MdlFileV14 mdlFileV14 = new MdlFileV14();
      mdlFileV14.Read(model);
      return mdlFileV14.DumpV11Qc(qc);
    }

    public static string[] DumpV14ToV44(string model, string qc)
    {
      MdlFileV14 mdlFileV14 = new MdlFileV14();
      mdlFileV14.Read(model);
      return mdlFileV14.DumpV44Qc(qc);
    }

    public static void DumpSp2ToSpz(string sp2, string sp2texdir, string spz, string spztexdir)
    {
      Directory.CreateDirectory(spztexdir);
      Sp2File sp2File = new Sp2File();
      sp2File.Read(sp2);
      SpzFile spzFile = sp2File.ToSpzFile();
      spzFile.Write(spz);
      for (int index = 0; index < sp2File.FrameCollection.Count; ++index)
      {
        FileInfo fileInfo = new FileInfo(sp2File.FrameCollection[index].Image);
        FIBITMAP dib = (FIBITMAP) 0;
        dib = FreeImage.Load(FREE_IMAGE_FORMAT.FIF_PCX, sp2texdir + "\\" + fileInfo.Name, FREE_IMAGE_LOAD_FLAGS.DEFAULT);
        FreeImage.Save(FREE_IMAGE_FORMAT.FIF_PNG, dib, spztexdir + "\\" + spzFile.Images[index], FREE_IMAGE_SAVE_FLAGS.PNG_Z_DEFAULT_COMPRESSION);
        if (!dib.IsNull)
          FreeImage.Unload(dib);
        dib = (FIBITMAP) 0;
      }
    }

    public static void ConvertV11ToV14(string loadpath, string savepath)
    {
      FileInfo fileInfo1 = new FileInfo(loadpath);
      FileInfo fileInfo2 = new FileInfo(savepath);
      if (!fileInfo2.Directory.Exists)
        fileInfo2.Directory.Create();
      MdlFileV11 mdlFileV11 = new MdlFileV11();
      mdlFileV11.Read(loadpath);
      MdlFileV14 v14 = mdlFileV11.ToV14();
      string str = fileInfo1.FullName.Substring(0, fileInfo2.FullName.Length - fileInfo1.Extension.Length) + ".qce";
      if (File.Exists(str))
        v14.ReadQce(str);
      v14.Write(fileInfo2.FullName);
    }
  }
}
