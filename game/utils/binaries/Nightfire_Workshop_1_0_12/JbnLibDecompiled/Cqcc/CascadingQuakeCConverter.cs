// Decompiled with JetBrains decompiler
// Type: JbnLib.Cqcc.CascadingQuakeCConverter
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Lang;
using JbnLib.Qc;
using JbnLib.Shared;
using JbnLib.Smd;
using System.Collections.Generic;
using System.IO;

namespace JbnLib.Cqcc
{
  public class CascadingQuakeCConverter
  {
    private string _ModelName;
    private List<OmittedCommand> _OmittedCommands;
    private List<string> _ProccessedSmdCollection;

    public event SmdProcessedHandler SmdProcessed;

    public string ModelName => this._ModelName;

    public List<OmittedCommand> OmittedCommands => this._OmittedCommands;

    public List<string> ProccessedSmdCollection => this._ProccessedSmdCollection;

    public string[] V10ToV11(string loaddir, string savedir, string qc)
    {
      DirectoryInfo directoryInfo1 = new DirectoryInfo(loaddir);
      if (!directoryInfo1.Exists)
        Messages.ThrowException("Cqcc.CascadingQuakeCConverter.V10ToV11(string, string, string)", Messages.DIR_NOT_EXIST + directoryInfo1.FullName);
      DirectoryInfo directoryInfo2 = new DirectoryInfo(savedir);
      directoryInfo2.Create();
      List<string> stringList = new List<string>();
      QcFileV10 qcFileV10 = new QcFileV10();
      qcFileV10.Read(directoryInfo1.FullName + "\\" + StaticMethods.GoodExtension(directoryInfo1.FullName + "\\" + qc, ".qc"));
      QcFileV11 v11_1 = qcFileV10.ToV11();
      this._OmittedCommands = v11_1.OmittedCommands;
      this._ModelName = v11_1.ModelName.Value;
      this._ProccessedSmdCollection = new List<string>();
      for (int index = 0; index < v11_1.BodyCollection.Count; ++index)
      {
        if (!this.Processed(v11_1.BodyCollection[index].File))
        {
          SmdFileV10 smdFileV10 = new SmdFileV10(SmdType.Reference);
          smdFileV10.Read(directoryInfo1.FullName + "\\" + StaticMethods.GoodExtension(v11_1.BodyCollection[index].File, ".smd"));
          SmdFileV11 v11_2 = smdFileV10.ToV11();
          v11_2.Write(directoryInfo2.FullName + "\\" + StaticMethods.GoodExtension(v11_1.BodyCollection[index].File, ".smd"));
          string[] textures = v11_2.GetTextures();
          SmdProcessedHandler smdProcessed = this.SmdProcessed;
          if (smdProcessed != null)
            smdProcessed((object) this, new SmdProcessedEventArgs(v11_1.BodyCollection[index].File, textures, v11_2.TimeCollection.Count, v11_2.TriangleCollection.Count));
          foreach (string str in textures)
            stringList.Add(str);
          this._ProccessedSmdCollection.Add(v11_1.BodyCollection[index].File);
        }
      }
      for (int index1 = 0; index1 < v11_1.BodyGroupCollection.Count; ++index1)
      {
        for (int index2 = 0; index2 < v11_1.BodyGroupCollection[index1].BodyCollection.Count; ++index2)
        {
          if (v11_1.BodyGroupCollection[index1].BodyCollection[index2].File != null && !this.Processed(v11_1.BodyGroupCollection[index1].BodyCollection[index2].File))
          {
            SmdFileV10 smdFileV10 = new SmdFileV10(SmdType.Reference);
            smdFileV10.Read(directoryInfo1.FullName + "\\" + StaticMethods.GoodExtension(v11_1.BodyGroupCollection[index1].BodyCollection[index2].File, ".smd"));
            SmdFileV11 v11_3 = smdFileV10.ToV11();
            v11_3.Write(directoryInfo2.FullName + "\\" + StaticMethods.GoodExtension(v11_1.BodyGroupCollection[index1].BodyCollection[index2].File, ".smd"));
            string[] textures = v11_3.GetTextures();
            SmdProcessedHandler smdProcessed = this.SmdProcessed;
            if (smdProcessed != null)
              smdProcessed((object) this, new SmdProcessedEventArgs(v11_1.BodyGroupCollection[index1].BodyCollection[index2].File, textures, v11_3.TimeCollection.Count, v11_3.TriangleCollection.Count));
            foreach (string str in textures)
              stringList.Add(str);
            this._ProccessedSmdCollection.Add(v11_1.BodyGroupCollection[index1].BodyCollection[index2].File);
          }
        }
      }
      for (int index3 = 0; index3 < v11_1.SequenceCollection.Count; ++index3)
      {
        for (int index4 = 0; index4 < v11_1.SequenceCollection[index3].FileCollection.Count; ++index4)
        {
          if (!this.Processed(v11_1.SequenceCollection[index3].FileCollection[index4]))
          {
            SmdFileV10 smdFileV10 = new SmdFileV10(SmdType.Animation);
            smdFileV10.Read(directoryInfo1.FullName + "\\" + StaticMethods.GoodExtension(v11_1.SequenceCollection[index3].FileCollection[index4], ".smd"));
            SmdFileV11 v11_4 = smdFileV10.ToV11();
            v11_4.Write(directoryInfo2.FullName + "\\" + StaticMethods.GoodExtension(v11_1.SequenceCollection[index3].FileCollection[index4], ".smd"));
            SmdProcessedHandler smdProcessed = this.SmdProcessed;
            if (smdProcessed != null)
              smdProcessed((object) this, new SmdProcessedEventArgs(v11_1.SequenceCollection[index3].FileCollection[index4], (string[]) null, v11_4.TimeCollection.Count, v11_4.TriangleCollection.Count));
            this._ProccessedSmdCollection.Add(v11_1.SequenceCollection[index3].FileCollection[index4]);
          }
        }
        if (v11_1.SequenceCollection[index3].Animation != null && !this.Processed(v11_1.SequenceCollection[index3].Animation.File))
        {
          SmdFileV10 smdFileV10 = new SmdFileV10(SmdType.Animation);
          smdFileV10.Read(directoryInfo1.FullName + "\\" + StaticMethods.GoodExtension(v11_1.SequenceCollection[index3].Animation.File, ".smd"));
          SmdFileV11 v11_5 = smdFileV10.ToV11();
          v11_5.Write(directoryInfo2.FullName + "\\" + StaticMethods.GoodExtension(v11_1.SequenceCollection[index3].Animation.File, ".smd"));
          SmdProcessedHandler smdProcessed = this.SmdProcessed;
          if (smdProcessed != null)
            smdProcessed((object) this, new SmdProcessedEventArgs(v11_1.SequenceCollection[index3].Animation.File, (string[]) null, v11_5.TimeCollection.Count, v11_5.TriangleCollection.Count));
          this._ProccessedSmdCollection.Add(v11_1.SequenceCollection[index3].Animation.File);
        }
      }
      v11_1.Write(directoryInfo2.FullName + "\\" + StaticMethods.GoodExtension(qc, ".qc"));
      return StaticMethods.EliminateDuplicates(stringList.ToArray());
    }

    public string[] V10ToV44(string loaddir, string savedir, string qc)
    {
      DirectoryInfo directoryInfo1 = new DirectoryInfo(loaddir);
      if (!directoryInfo1.Exists)
        Messages.ThrowException("Cqcc.CascadingQuakeCConverter.V10ToV44(string, string, string)", Messages.DIR_NOT_EXIST + directoryInfo1.FullName);
      DirectoryInfo directoryInfo2 = new DirectoryInfo(savedir);
      directoryInfo2.Create();
      List<string> stringList = new List<string>();
      QcFileV10 qcFileV10 = new QcFileV10();
      qcFileV10.Read(directoryInfo1.FullName + "\\" + StaticMethods.GoodExtension(directoryInfo1.FullName + "\\" + qc, ".qc"));
      QcFileV44 v44_1 = qcFileV10.ToV44();
      this._OmittedCommands = v44_1.OmittedCommands;
      this._ModelName = v44_1.ModelName.Value;
      this._ProccessedSmdCollection = new List<string>();
      for (int index = 0; index < v44_1.BodyCollection.Count; ++index)
      {
        if (!this.Processed(v44_1.BodyCollection[index].File))
        {
          SmdFileV10 smdFileV10 = new SmdFileV10(SmdType.Reference);
          smdFileV10.Read(directoryInfo1.FullName + "\\" + StaticMethods.GoodExtension(v44_1.BodyCollection[index].File, ".smd"));
          SmdFileV44 v44_2 = smdFileV10.ToV44();
          v44_2.Write(directoryInfo2.FullName + "\\" + StaticMethods.GoodExtension(v44_1.BodyCollection[index].File, ".smd"));
          string[] textures = v44_2.GetTextures();
          SmdProcessedHandler smdProcessed = this.SmdProcessed;
          if (smdProcessed != null)
            smdProcessed((object) this, new SmdProcessedEventArgs(v44_1.BodyCollection[index].File, textures, v44_2.TimeCollection.Count, v44_2.TriangleCollection.Count));
          foreach (string str in textures)
            stringList.Add(str);
          this._ProccessedSmdCollection.Add(v44_1.BodyCollection[index].File);
        }
      }
      for (int index1 = 0; index1 < v44_1.BodyGroupCollection.Count; ++index1)
      {
        for (int index2 = 0; index2 < v44_1.BodyGroupCollection[index1].BodyCollection.Count; ++index2)
        {
          if (v44_1.BodyGroupCollection[index1].BodyCollection[index2].File != null && !this.Processed(v44_1.BodyGroupCollection[index1].BodyCollection[index2].File))
          {
            SmdFileV10 smdFileV10 = new SmdFileV10(SmdType.Reference);
            smdFileV10.Read(directoryInfo1.FullName + "\\" + StaticMethods.GoodExtension(v44_1.BodyGroupCollection[index1].BodyCollection[index2].File, ".smd"));
            SmdFileV44 v44_3 = smdFileV10.ToV44();
            v44_3.Write(directoryInfo2.FullName + "\\" + StaticMethods.GoodExtension(v44_1.BodyGroupCollection[index1].BodyCollection[index2].File, ".smd"));
            string[] textures = v44_3.GetTextures();
            SmdProcessedHandler smdProcessed = this.SmdProcessed;
            if (smdProcessed != null)
              smdProcessed((object) this, new SmdProcessedEventArgs(v44_1.BodyGroupCollection[index1].BodyCollection[index2].File, textures, v44_3.TimeCollection.Count, v44_3.TriangleCollection.Count));
            foreach (string str in textures)
              stringList.Add(str);
            this._ProccessedSmdCollection.Add(v44_1.BodyGroupCollection[index1].BodyCollection[index2].File);
          }
        }
      }
      for (int index3 = 0; index3 < v44_1.SequenceCollection.Count; ++index3)
      {
        for (int index4 = 0; index4 < v44_1.SequenceCollection[index3].FileCollection.Count; ++index4)
        {
          if (!this.Processed(v44_1.SequenceCollection[index3].FileCollection[index4]))
          {
            SmdFileV10 smdFileV10 = new SmdFileV10(SmdType.Animation);
            smdFileV10.Read(directoryInfo1.FullName + "\\" + StaticMethods.GoodExtension(v44_1.SequenceCollection[index3].FileCollection[index4], ".smd"));
            SmdFileV44 v44_4 = smdFileV10.ToV44();
            v44_4.Write(directoryInfo2.FullName + "\\" + StaticMethods.GoodExtension(v44_1.SequenceCollection[index3].FileCollection[index4], ".smd"));
            SmdProcessedHandler smdProcessed = this.SmdProcessed;
            if (smdProcessed != null)
              smdProcessed((object) this, new SmdProcessedEventArgs(v44_1.SequenceCollection[index3].FileCollection[index4], (string[]) null, v44_4.TimeCollection.Count, v44_4.TriangleCollection.Count));
            this._ProccessedSmdCollection.Add(v44_1.SequenceCollection[index3].FileCollection[index4]);
          }
        }
        if (v44_1.SequenceCollection[index3].Animation != null && !this.Processed(v44_1.SequenceCollection[index3].Animation.File))
        {
          SmdFileV10 smdFileV10 = new SmdFileV10(SmdType.Animation);
          smdFileV10.Read(directoryInfo1.FullName + "\\" + StaticMethods.GoodExtension(v44_1.SequenceCollection[index3].Animation.File, ".smd"));
          SmdFileV44 v44_5 = smdFileV10.ToV44();
          v44_5.Write(directoryInfo2.FullName + "\\" + StaticMethods.GoodExtension(v44_1.SequenceCollection[index3].Animation.File, ".smd"));
          SmdProcessedHandler smdProcessed = this.SmdProcessed;
          if (smdProcessed != null)
            smdProcessed((object) this, new SmdProcessedEventArgs(v44_1.SequenceCollection[index3].Animation.File, (string[]) null, v44_5.TimeCollection.Count, v44_5.TriangleCollection.Count));
          this._ProccessedSmdCollection.Add(v44_1.SequenceCollection[index3].Animation.File);
        }
      }
      v44_1.Write(directoryInfo2.FullName + "\\" + StaticMethods.GoodExtension(qc, ".qc"));
      return StaticMethods.EliminateDuplicates(stringList.ToArray());
    }

    public string[] V11ToV10(string loaddir, string savedir, string qc)
    {
      DirectoryInfo directoryInfo1 = new DirectoryInfo(loaddir);
      if (!directoryInfo1.Exists)
        Messages.ThrowException("Cqcc.CascadingQuakeCConverter.V11ToV10(string, string, string)", Messages.DIR_NOT_EXIST + directoryInfo1.FullName);
      DirectoryInfo directoryInfo2 = new DirectoryInfo(savedir);
      directoryInfo2.Create();
      List<string> stringList = new List<string>();
      QcFileV11 qcFileV11 = new QcFileV11();
      qcFileV11.Read(directoryInfo1.FullName + "\\" + StaticMethods.GoodExtension(directoryInfo1.FullName + "\\" + qc, ".qc"));
      QcFileV10 v10_1 = qcFileV11.ToV10();
      this._OmittedCommands = v10_1.OmittedCommands;
      this._ModelName = v10_1.ModelName.Value;
      this._ProccessedSmdCollection = new List<string>();
      for (int index = 0; index < v10_1.BodyCollection.Count; ++index)
      {
        if (!this.Processed(v10_1.BodyCollection[index].File))
        {
          SmdFileV11 smdFileV11 = new SmdFileV11(SmdType.Reference);
          smdFileV11.Read(directoryInfo1.FullName + "\\" + StaticMethods.GoodExtension(v10_1.BodyCollection[index].File, ".smd"));
          SmdFileV10 v10_2 = smdFileV11.ToV10();
          v10_2.Write(directoryInfo2.FullName + "\\" + StaticMethods.GoodExtension(v10_1.BodyCollection[index].File, ".smd"));
          string[] textures = v10_2.GetTextures();
          SmdProcessedHandler smdProcessed = this.SmdProcessed;
          if (smdProcessed != null)
            smdProcessed((object) this, new SmdProcessedEventArgs(v10_1.BodyCollection[index].File, textures, v10_2.TimeCollection.Count, v10_2.TriangleCollection.Count));
          foreach (string str in textures)
            stringList.Add(str);
          this._ProccessedSmdCollection.Add(v10_1.BodyCollection[index].File);
        }
      }
      for (int index1 = 0; index1 < v10_1.BodyGroupCollection.Count; ++index1)
      {
        for (int index2 = 0; index2 < v10_1.BodyGroupCollection[index1].BodyCollection.Count; ++index2)
        {
          if (v10_1.BodyGroupCollection[index1].BodyCollection[index2].File != null && !this.Processed(v10_1.BodyGroupCollection[index1].BodyCollection[index2].File))
          {
            SmdFileV11 smdFileV11 = new SmdFileV11(SmdType.Reference);
            smdFileV11.Read(directoryInfo1.FullName + "\\" + StaticMethods.GoodExtension(v10_1.BodyGroupCollection[index1].BodyCollection[index2].File, ".smd"));
            SmdFileV10 v10_3 = smdFileV11.ToV10();
            v10_3.Write(directoryInfo2.FullName + "\\" + StaticMethods.GoodExtension(v10_1.BodyGroupCollection[index1].BodyCollection[index2].File, ".smd"));
            string[] textures = v10_3.GetTextures();
            SmdProcessedHandler smdProcessed = this.SmdProcessed;
            if (smdProcessed != null)
              smdProcessed((object) this, new SmdProcessedEventArgs(v10_1.BodyGroupCollection[index1].BodyCollection[index2].File, textures, v10_3.TimeCollection.Count, v10_3.TriangleCollection.Count));
            foreach (string str in textures)
              stringList.Add(str);
            this._ProccessedSmdCollection.Add(v10_1.BodyGroupCollection[index1].BodyCollection[index2].File);
          }
        }
      }
      for (int index3 = 0; index3 < v10_1.SequenceCollection.Count; ++index3)
      {
        for (int index4 = 0; index4 < v10_1.SequenceCollection[index3].FileCollection.Count; ++index4)
        {
          if (!this.Processed(v10_1.SequenceCollection[index3].FileCollection[index4]))
          {
            SmdFileV11 smdFileV11 = new SmdFileV11(SmdType.Animation);
            smdFileV11.Read(directoryInfo1.FullName + "\\" + StaticMethods.GoodExtension(v10_1.SequenceCollection[index3].FileCollection[index4], ".smd"));
            SmdFileV10 v10_4 = smdFileV11.ToV10();
            v10_4.Write(directoryInfo2.FullName + "\\" + StaticMethods.GoodExtension(v10_1.SequenceCollection[index3].FileCollection[index4], ".smd"));
            SmdProcessedHandler smdProcessed = this.SmdProcessed;
            if (smdProcessed != null)
              smdProcessed((object) this, new SmdProcessedEventArgs(v10_1.SequenceCollection[index3].FileCollection[index4], (string[]) null, v10_4.TimeCollection.Count, v10_4.TriangleCollection.Count));
            this._ProccessedSmdCollection.Add(v10_1.SequenceCollection[index3].FileCollection[index4]);
          }
        }
        if (v10_1.SequenceCollection[index3].Animation != null && !this.Processed(v10_1.SequenceCollection[index3].Animation.File))
        {
          SmdFileV11 smdFileV11 = new SmdFileV11(SmdType.Animation);
          smdFileV11.Read(directoryInfo1.FullName + "\\" + StaticMethods.GoodExtension(v10_1.SequenceCollection[index3].Animation.File, ".smd"));
          SmdFileV10 v10_5 = smdFileV11.ToV10();
          v10_5.Write(directoryInfo2.FullName + "\\" + StaticMethods.GoodExtension(v10_1.SequenceCollection[index3].Animation.File, ".smd"));
          SmdProcessedHandler smdProcessed = this.SmdProcessed;
          if (smdProcessed != null)
            smdProcessed((object) this, new SmdProcessedEventArgs(v10_1.SequenceCollection[index3].Animation.File, (string[]) null, v10_5.TimeCollection.Count, v10_5.TriangleCollection.Count));
          this._ProccessedSmdCollection.Add(v10_1.SequenceCollection[index3].Animation.File);
        }
      }
      v10_1.Write(directoryInfo2.FullName + "\\" + StaticMethods.GoodExtension(qc, ".qc"));
      return StaticMethods.EliminateDuplicates(stringList.ToArray());
    }

    public string[] V11ToV44(string loaddir, string savedir, string qc)
    {
      DirectoryInfo directoryInfo1 = new DirectoryInfo(loaddir);
      if (!directoryInfo1.Exists)
        Messages.ThrowException("Cqcc.CascadingQuakeCConverter.V11ToV44(string, string, string)", Messages.DIR_NOT_EXIST + directoryInfo1.FullName);
      DirectoryInfo directoryInfo2 = new DirectoryInfo(savedir);
      directoryInfo2.Create();
      List<string> stringList = new List<string>();
      QcFileV11 qcFileV11 = new QcFileV11();
      qcFileV11.Read(directoryInfo1.FullName + "\\" + StaticMethods.GoodExtension(directoryInfo1.FullName + "\\" + qc, ".qc"));
      QcFileV44 v44_1 = qcFileV11.ToV44();
      this._OmittedCommands = v44_1.OmittedCommands;
      this._ModelName = v44_1.ModelName.Value;
      this._ProccessedSmdCollection = new List<string>();
      for (int index = 0; index < v44_1.BodyCollection.Count; ++index)
      {
        if (!this.Processed(v44_1.BodyCollection[index].File))
        {
          SmdFileV11 smdFileV11 = new SmdFileV11(SmdType.Reference);
          smdFileV11.Read(directoryInfo1.FullName + "\\" + StaticMethods.GoodExtension(v44_1.BodyCollection[index].File, ".smd"));
          SmdFileV44 v44_2 = smdFileV11.ToV44();
          v44_2.Write(directoryInfo2.FullName + "\\" + StaticMethods.GoodExtension(v44_1.BodyCollection[index].File, ".smd"));
          string[] textures = v44_2.GetTextures();
          SmdProcessedHandler smdProcessed = this.SmdProcessed;
          if (smdProcessed != null)
            smdProcessed((object) this, new SmdProcessedEventArgs(v44_1.BodyCollection[index].File, textures, v44_2.TimeCollection.Count, v44_2.TriangleCollection.Count));
          foreach (string str in textures)
            stringList.Add(str);
          this._ProccessedSmdCollection.Add(v44_1.BodyCollection[index].File);
        }
      }
      for (int index1 = 0; index1 < v44_1.BodyGroupCollection.Count; ++index1)
      {
        for (int index2 = 0; index2 < v44_1.BodyGroupCollection[index1].BodyCollection.Count; ++index2)
        {
          if (v44_1.BodyGroupCollection[index1].BodyCollection[index2].File != null && !this.Processed(v44_1.BodyGroupCollection[index1].BodyCollection[index2].File))
          {
            SmdFileV11 smdFileV11 = new SmdFileV11(SmdType.Reference);
            smdFileV11.Read(directoryInfo1.FullName + "\\" + StaticMethods.GoodExtension(v44_1.BodyGroupCollection[index1].BodyCollection[index2].File, ".smd"));
            SmdFileV44 v44_3 = smdFileV11.ToV44();
            v44_3.Write(directoryInfo2.FullName + "\\" + StaticMethods.GoodExtension(v44_1.BodyGroupCollection[index1].BodyCollection[index2].File, ".smd"));
            string[] textures = v44_3.GetTextures();
            SmdProcessedHandler smdProcessed = this.SmdProcessed;
            if (smdProcessed != null)
              smdProcessed((object) this, new SmdProcessedEventArgs(v44_1.BodyGroupCollection[index1].BodyCollection[index2].File, textures, v44_3.TimeCollection.Count, v44_3.TriangleCollection.Count));
            foreach (string str in textures)
              stringList.Add(str);
            this._ProccessedSmdCollection.Add(v44_1.BodyGroupCollection[index1].BodyCollection[index2].File);
          }
        }
      }
      for (int index3 = 0; index3 < v44_1.SequenceCollection.Count; ++index3)
      {
        for (int index4 = 0; index4 < v44_1.SequenceCollection[index3].FileCollection.Count; ++index4)
        {
          if (!this.Processed(v44_1.SequenceCollection[index3].FileCollection[index4]))
          {
            SmdFileV11 smdFileV11 = new SmdFileV11(SmdType.Animation);
            smdFileV11.Read(directoryInfo1.FullName + "\\" + StaticMethods.GoodExtension(v44_1.SequenceCollection[index3].FileCollection[index4], ".smd"));
            SmdFileV44 v44_4 = smdFileV11.ToV44();
            v44_4.Write(directoryInfo2.FullName + "\\" + StaticMethods.GoodExtension(v44_1.SequenceCollection[index3].FileCollection[index4], ".smd"));
            SmdProcessedHandler smdProcessed = this.SmdProcessed;
            if (smdProcessed != null)
              smdProcessed((object) this, new SmdProcessedEventArgs(v44_1.SequenceCollection[index3].FileCollection[index4], (string[]) null, v44_4.TimeCollection.Count, v44_4.TriangleCollection.Count));
            this._ProccessedSmdCollection.Add(v44_1.SequenceCollection[index3].FileCollection[index4]);
          }
        }
        if (v44_1.SequenceCollection[index3].Animation != null && !this.Processed(v44_1.SequenceCollection[index3].Animation.File))
        {
          SmdFileV11 smdFileV11 = new SmdFileV11(SmdType.Animation);
          smdFileV11.Read(directoryInfo1.FullName + "\\" + StaticMethods.GoodExtension(v44_1.SequenceCollection[index3].Animation.File, ".smd"));
          SmdFileV44 v44_5 = smdFileV11.ToV44();
          v44_5.Write(directoryInfo2.FullName + "\\" + StaticMethods.GoodExtension(v44_1.SequenceCollection[index3].Animation.File, ".smd"));
          SmdProcessedHandler smdProcessed = this.SmdProcessed;
          if (smdProcessed != null)
            smdProcessed((object) this, new SmdProcessedEventArgs(v44_1.SequenceCollection[index3].Animation.File, (string[]) null, v44_5.TimeCollection.Count, v44_5.TriangleCollection.Count));
          this._ProccessedSmdCollection.Add(v44_1.SequenceCollection[index3].Animation.File);
        }
      }
      v44_1.Write(directoryInfo2.FullName + "\\" + StaticMethods.GoodExtension(qc, ".qc"));
      return StaticMethods.EliminateDuplicates(stringList.ToArray());
    }

    public string[] V44ToV10(string loaddir, string savedir, string qc)
    {
      DirectoryInfo directoryInfo1 = new DirectoryInfo(loaddir);
      if (!directoryInfo1.Exists)
        Messages.ThrowException("Cqcc.CascadingQuakeCConverter.V44ToV10(string, string, string)", Messages.DIR_NOT_EXIST + directoryInfo1.FullName);
      DirectoryInfo directoryInfo2 = new DirectoryInfo(savedir);
      directoryInfo2.Create();
      List<string> stringList = new List<string>();
      QcFileV44 qcFileV44 = new QcFileV44();
      qcFileV44.Read(directoryInfo1.FullName + "\\" + StaticMethods.GoodExtension(directoryInfo1.FullName + "\\" + qc, ".qc"));
      QcFileV10 v10_1 = qcFileV44.ToV10();
      this._OmittedCommands = v10_1.OmittedCommands;
      this._ModelName = v10_1.ModelName.Value;
      this._ProccessedSmdCollection = new List<string>();
      for (int index = 0; index < v10_1.BodyCollection.Count; ++index)
      {
        if (!this.Processed(v10_1.BodyCollection[index].File))
        {
          SmdFileV44 smdFileV44 = new SmdFileV44(SmdType.Reference);
          smdFileV44.Read(directoryInfo1.FullName + "\\" + StaticMethods.GoodExtension(v10_1.BodyCollection[index].File, ".smd"));
          SmdFileV10 v10_2 = smdFileV44.ToV10();
          v10_2.Write(directoryInfo2.FullName + "\\" + StaticMethods.GoodExtension(v10_1.BodyCollection[index].File, ".smd"));
          string[] textures = v10_2.GetTextures();
          SmdProcessedHandler smdProcessed = this.SmdProcessed;
          if (smdProcessed != null)
            smdProcessed((object) this, new SmdProcessedEventArgs(v10_1.BodyCollection[index].File, textures, v10_2.TimeCollection.Count, v10_2.TriangleCollection.Count));
          foreach (string str in textures)
            stringList.Add(str);
          this._ProccessedSmdCollection.Add(v10_1.BodyCollection[index].File);
        }
      }
      for (int index1 = 0; index1 < v10_1.BodyGroupCollection.Count; ++index1)
      {
        for (int index2 = 0; index2 < v10_1.BodyGroupCollection[index1].BodyCollection.Count; ++index2)
        {
          if (v10_1.BodyGroupCollection[index1].BodyCollection[index2].File != null && !this.Processed(v10_1.BodyGroupCollection[index1].BodyCollection[index2].File))
          {
            SmdFileV44 smdFileV44 = new SmdFileV44(SmdType.Reference);
            smdFileV44.Read(directoryInfo1.FullName + "\\" + StaticMethods.GoodExtension(v10_1.BodyGroupCollection[index1].BodyCollection[index2].File, ".smd"));
            SmdFileV10 v10_3 = smdFileV44.ToV10();
            v10_3.Write(directoryInfo2.FullName + "\\" + StaticMethods.GoodExtension(v10_1.BodyGroupCollection[index1].BodyCollection[index2].File, ".smd"));
            string[] textures = v10_3.GetTextures();
            SmdProcessedHandler smdProcessed = this.SmdProcessed;
            if (smdProcessed != null)
              smdProcessed((object) this, new SmdProcessedEventArgs(v10_1.BodyGroupCollection[index1].BodyCollection[index2].File, textures, v10_3.TimeCollection.Count, v10_3.TriangleCollection.Count));
            foreach (string str in textures)
              stringList.Add(str);
            this._ProccessedSmdCollection.Add(v10_1.BodyGroupCollection[index1].BodyCollection[index2].File);
          }
        }
      }
      for (int index3 = 0; index3 < v10_1.SequenceCollection.Count; ++index3)
      {
        for (int index4 = 0; index4 < v10_1.SequenceCollection[index3].FileCollection.Count; ++index4)
        {
          if (!this.Processed(v10_1.SequenceCollection[index3].FileCollection[index4]))
          {
            SmdFileV44 smdFileV44 = new SmdFileV44(SmdType.Animation);
            smdFileV44.Read(directoryInfo1.FullName + "\\" + StaticMethods.GoodExtension(v10_1.SequenceCollection[index3].FileCollection[index4], ".smd"));
            SmdFileV10 v10_4 = smdFileV44.ToV10();
            v10_4.Write(directoryInfo2.FullName + "\\" + StaticMethods.GoodExtension(v10_1.SequenceCollection[index3].FileCollection[index4], ".smd"));
            SmdProcessedHandler smdProcessed = this.SmdProcessed;
            if (smdProcessed != null)
              smdProcessed((object) this, new SmdProcessedEventArgs(v10_1.SequenceCollection[index3].FileCollection[index4], (string[]) null, v10_4.TimeCollection.Count, v10_4.TriangleCollection.Count));
            this._ProccessedSmdCollection.Add(v10_1.SequenceCollection[index3].FileCollection[index4]);
          }
        }
        if (v10_1.SequenceCollection[index3].Animation != null && !this.Processed(v10_1.SequenceCollection[index3].Animation.File))
        {
          SmdFileV44 smdFileV44 = new SmdFileV44(SmdType.Animation);
          smdFileV44.Read(directoryInfo1.FullName + "\\" + StaticMethods.GoodExtension(v10_1.SequenceCollection[index3].Animation.File, ".smd"));
          SmdFileV10 v10_5 = smdFileV44.ToV10();
          v10_5.Write(directoryInfo2.FullName + "\\" + StaticMethods.GoodExtension(v10_1.SequenceCollection[index3].Animation.File, ".smd"));
          SmdProcessedHandler smdProcessed = this.SmdProcessed;
          if (smdProcessed != null)
            smdProcessed((object) this, new SmdProcessedEventArgs(v10_1.SequenceCollection[index3].Animation.File, (string[]) null, v10_5.TimeCollection.Count, v10_5.TriangleCollection.Count));
          this._ProccessedSmdCollection.Add(v10_1.SequenceCollection[index3].Animation.File);
        }
      }
      v10_1.Write(directoryInfo2.FullName + "\\" + StaticMethods.GoodExtension(qc, ".qc"));
      return StaticMethods.EliminateDuplicates(stringList.ToArray());
    }

    public string[] V44ToV11(string loaddir, string savedir, string qc)
    {
      DirectoryInfo directoryInfo1 = new DirectoryInfo(loaddir);
      if (!directoryInfo1.Exists)
        Messages.ThrowException("Cqcc.CascadingQuakeCConverter.V44ToV11(string, string, string)", Messages.DIR_NOT_EXIST + directoryInfo1.FullName);
      DirectoryInfo directoryInfo2 = new DirectoryInfo(savedir);
      directoryInfo2.Create();
      List<string> stringList = new List<string>();
      QcFileV44 qcFileV44 = new QcFileV44();
      qcFileV44.Read(directoryInfo1.FullName + "\\" + StaticMethods.GoodExtension(directoryInfo1.FullName + "\\" + qc, ".qc"));
      QcFileV11 v11_1 = qcFileV44.ToV11();
      this._OmittedCommands = v11_1.OmittedCommands;
      this._ModelName = v11_1.ModelName.Value;
      this._ProccessedSmdCollection = new List<string>();
      for (int index = 0; index < v11_1.BodyCollection.Count; ++index)
      {
        if (!this.Processed(v11_1.BodyCollection[index].File))
        {
          SmdFileV44 smdFileV44 = new SmdFileV44(SmdType.Reference);
          smdFileV44.Read(directoryInfo1.FullName + "\\" + StaticMethods.GoodExtension(v11_1.BodyCollection[index].File, ".smd"));
          SmdFileV11 v11_2 = smdFileV44.ToV11();
          v11_2.Write(directoryInfo2.FullName + "\\" + StaticMethods.GoodExtension(v11_1.BodyCollection[index].File, ".smd"));
          string[] textures = v11_2.GetTextures();
          SmdProcessedHandler smdProcessed = this.SmdProcessed;
          if (smdProcessed != null)
            smdProcessed((object) this, new SmdProcessedEventArgs(v11_1.BodyCollection[index].File, textures, v11_2.TimeCollection.Count, v11_2.TriangleCollection.Count));
          foreach (string str in textures)
            stringList.Add(str);
          this._ProccessedSmdCollection.Add(v11_1.BodyCollection[index].File);
        }
      }
      for (int index1 = 0; index1 < v11_1.BodyGroupCollection.Count; ++index1)
      {
        for (int index2 = 0; index2 < v11_1.BodyGroupCollection[index1].BodyCollection.Count; ++index2)
        {
          if (v11_1.BodyGroupCollection[index1].BodyCollection[index2].File != null && !this.Processed(v11_1.BodyGroupCollection[index1].BodyCollection[index2].File))
          {
            SmdFileV44 smdFileV44 = new SmdFileV44(SmdType.Reference);
            smdFileV44.Read(directoryInfo1.FullName + "\\" + StaticMethods.GoodExtension(v11_1.BodyGroupCollection[index1].BodyCollection[index2].File, ".smd"));
            SmdFileV11 v11_3 = smdFileV44.ToV11();
            v11_3.Write(directoryInfo2.FullName + "\\" + StaticMethods.GoodExtension(v11_1.BodyGroupCollection[index1].BodyCollection[index2].File, ".smd"));
            string[] textures = v11_3.GetTextures();
            SmdProcessedHandler smdProcessed = this.SmdProcessed;
            if (smdProcessed != null)
              smdProcessed((object) this, new SmdProcessedEventArgs(v11_1.BodyGroupCollection[index1].BodyCollection[index2].File, textures, v11_3.TimeCollection.Count, v11_3.TriangleCollection.Count));
            foreach (string str in textures)
              stringList.Add(str);
            this._ProccessedSmdCollection.Add(v11_1.BodyGroupCollection[index1].BodyCollection[index2].File);
          }
        }
      }
      for (int index3 = 0; index3 < v11_1.SequenceCollection.Count; ++index3)
      {
        for (int index4 = 0; index4 < v11_1.SequenceCollection[index3].FileCollection.Count; ++index4)
        {
          if (!this.Processed(v11_1.SequenceCollection[index3].FileCollection[index4]))
          {
            SmdFileV44 smdFileV44 = new SmdFileV44(SmdType.Animation);
            smdFileV44.Read(directoryInfo1.FullName + "\\" + StaticMethods.GoodExtension(v11_1.SequenceCollection[index3].FileCollection[index4], ".smd"));
            SmdFileV11 v11_4 = smdFileV44.ToV11();
            v11_4.Write(directoryInfo2.FullName + "\\" + StaticMethods.GoodExtension(v11_1.SequenceCollection[index3].FileCollection[index4], ".smd"));
            SmdProcessedHandler smdProcessed = this.SmdProcessed;
            if (smdProcessed != null)
              smdProcessed((object) this, new SmdProcessedEventArgs(v11_1.SequenceCollection[index3].FileCollection[index4], (string[]) null, v11_4.TimeCollection.Count, v11_4.TriangleCollection.Count));
            this._ProccessedSmdCollection.Add(v11_1.SequenceCollection[index3].FileCollection[index4]);
          }
        }
        if (v11_1.SequenceCollection[index3].Animation != null && !this.Processed(v11_1.SequenceCollection[index3].Animation.File))
        {
          SmdFileV44 smdFileV44 = new SmdFileV44(SmdType.Animation);
          smdFileV44.Read(directoryInfo1.FullName + "\\" + StaticMethods.GoodExtension(v11_1.SequenceCollection[index3].Animation.File, ".smd"));
          SmdFileV11 v11_5 = smdFileV44.ToV11();
          v11_5.Write(directoryInfo2.FullName + "\\" + StaticMethods.GoodExtension(v11_1.SequenceCollection[index3].Animation.File, ".smd"));
          SmdProcessedHandler smdProcessed = this.SmdProcessed;
          if (smdProcessed != null)
            smdProcessed((object) this, new SmdProcessedEventArgs(v11_1.SequenceCollection[index3].Animation.File, (string[]) null, v11_5.TimeCollection.Count, v11_5.TriangleCollection.Count));
          this._ProccessedSmdCollection.Add(v11_1.SequenceCollection[index3].Animation.File);
        }
      }
      v11_1.Write(directoryInfo2.FullName + "\\" + StaticMethods.GoodExtension(qc, ".qc"));
      return StaticMethods.EliminateDuplicates(stringList.ToArray());
    }

    private bool Processed(string smd) => this._ProccessedSmdCollection.Contains(smd);
  }
}
