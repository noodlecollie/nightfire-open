// Decompiled with JetBrains decompiler
// Type: JbnLib.Multi.MultiV14ToV11
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Lang;
using JbnLib.Mdl;
using JbnLib.Shared;
using JbnLib.Wrappers;
using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO;
using System.Threading;

namespace JbnLib.Multi
{
  public class MultiV14ToV11
  {
    private bool _Stop;
    private volatile bool _IsStopped = true;
    private Dictionary<int, Thread> _Loopers = new Dictionary<int, Thread>();
    private volatile System.Collections.Generic.Queue<int> _Retire = new System.Collections.Generic.Queue<int>();
    private readonly string _Temp = Environment.GetEnvironmentVariable("TEMP") + "\\mdlconvert";
    private volatile QueueItem[] _Work;
    private int _Threads = Environment.ProcessorCount;
    private int _Count = -1;
    private StudioMdl _StudioMdl;
    private volatile System.Collections.Generic.Queue<int> _ToDo = new System.Collections.Generic.Queue<int>();
    private List<MessageEventArgs> _Warnings = new List<MessageEventArgs>();
    private List<MessageEventArgs> _Errors = new List<MessageEventArgs>();
    private static List<string> _Files;
    private static bool _Recursive = true;

    public event ErrorAddedHandler ErrorAdded;

    public event WarningAddedHandler WarningAdded;

    public event ConversionStartedHandler ConversionStarted;

    public event ConversionCompletedHandler ConversionCompleted;

    public event ConversionFailedHandler ConversionFailed;

    public QueueItem[] Queue
    {
      get => this._Work;
      set
      {
        if (this._IsStopped)
        {
          this._Work = value;
          this._Count = value.Length;
        }
        else
          Messages.ThrowException("Multi.MultiV14ToV11.Queue.set", Messages.SET_PROHIBITED);
      }
    }

    public int Threads
    {
      get => this._Threads;
      set
      {
        if (this._IsStopped)
          this._Threads = value;
        else
          Messages.ThrowException("Multi.MultiV14ToV11.Threads.set", Messages.SET_PROHIBITED);
      }
    }

    public int Count => this._Count;

    public StudioMdl StudioMdl
    {
      get => this._StudioMdl;
      set
      {
        if (this._IsStopped)
          this._StudioMdl = value;
        else
          Messages.ThrowException("Multi.MultiV14ToV11.StudioMdl.set", Messages.SET_PROHIBITED);
      }
    }

    public int CountToDo => this._ToDo.Count;

    public int[] ToDo => this._ToDo.ToArray();

    public List<MessageEventArgs> Warnings => this._Warnings;

    private void AddWarning(MessageEventArgs message)
    {
      this._Warnings.Add(message);
      WarningAddedHandler warningAdded = this.WarningAdded;
      if (warningAdded == null)
        return;
      warningAdded((object) this, message);
    }

    public List<MessageEventArgs> Errors => this._Errors;

    private void AddError(MessageEventArgs message)
    {
      this._Errors.Add(message);
      ErrorAddedHandler errorAdded = this.ErrorAdded;
      if (errorAdded == null)
        return;
      errorAdded((object) this, message);
    }

    public MultiV14ToV11()
    {
      Thread.CurrentThread.CurrentUICulture = new CultureInfo("en");
      Thread.CurrentThread.CurrentCulture = new CultureInfo("en-US");
      this._StudioMdl = (StudioMdl) null;
      this._Work = (QueueItem[]) null;
    }

    public MultiV14ToV11(string studiomdl, QueueItem[] mdls)
    {
      Thread.CurrentThread.CurrentUICulture = new CultureInfo("en");
      Thread.CurrentThread.CurrentCulture = new CultureInfo("en-US");
      this._StudioMdl = new StudioMdl(studiomdl);
      this._Work = mdls;
      this._Count = mdls.Length;
    }

    public MultiV14ToV11(StudioMdl studiomdl, QueueItem[] mdls)
    {
      Thread.CurrentThread.CurrentUICulture = new CultureInfo("en");
      Thread.CurrentThread.CurrentCulture = new CultureInfo("en-US");
      this._StudioMdl = studiomdl;
      this._Work = mdls;
      this._Count = mdls.Length;
    }

    public MultiV14ToV11(string studiomdl, QueueItem[] mdls, int threads)
    {
      Thread.CurrentThread.CurrentUICulture = new CultureInfo("en");
      Thread.CurrentThread.CurrentCulture = new CultureInfo("en-US");
      this._StudioMdl = new StudioMdl(studiomdl);
      this._Threads = threads;
      this._Work = mdls;
      this._Count = mdls.Length;
    }

    public MultiV14ToV11(StudioMdl studiomdl, QueueItem[] mdls, int threads)
    {
      Thread.CurrentThread.CurrentUICulture = new CultureInfo("en");
      Thread.CurrentThread.CurrentCulture = new CultureInfo("en-US");
      this._StudioMdl = studiomdl;
      this._Threads = threads;
      this._Work = mdls;
      this._Count = mdls.Length;
    }

    public static QueueItem[] BuildQueue(string opendir, string savedir) => MultiV14ToV11.BuildQueue(opendir, savedir, new StudioMdlArgs(), true, true);

    public static QueueItem[] BuildQueue(string opendir, string savedir, StudioMdlArgs options) => MultiV14ToV11.BuildQueue(opendir, savedir, options, true, true);

    public static QueueItem[] BuildQueue(
      string opendir,
      string savedir,
      StudioMdlArgs options,
      bool recursive,
      bool checkdone)
    {
      DirectoryInfo directoryInfo1 = new DirectoryInfo(opendir);
      DirectoryInfo directoryInfo2 = new DirectoryInfo(savedir);
      if (!directoryInfo1.Exists)
        Messages.ThrowException("Multi.MultiV14ToV11.BuildQueue(string, string, StudioMdlOptions, bool, bool)", Messages.DIR_NOT_EXIST + directoryInfo1.FullName);
      MultiV14ToV11._Recursive = recursive;
      MultiV14ToV11._Files = new List<string>();
      MultiV14ToV11.ReadDirectory(directoryInfo1.FullName);
      List<QueueItem> queueItemList = new List<QueueItem>();
      for (int index = 0; index < MultiV14ToV11._Files.Count; ++index)
      {
        FileInfo fileInfo1 = new FileInfo(MultiV14ToV11._Files[index]);
        FileInfo fileInfo2 = new FileInfo(directoryInfo2.FullName + MultiV14ToV11._Files[index].Substring(directoryInfo1.FullName.Length));
        if (MdlFile.GetVersion(fileInfo1.FullName) == 14)
          queueItemList.Add(new QueueItem(fileInfo1.FullName, fileInfo2.FullName, options));
      }
      if (checkdone)
      {
        for (int index = queueItemList.Count - 1; index >= 0; --index)
        {
          if (File.Exists(queueItemList[index].Save))
          {
            if (MdlFile.GetVersion(queueItemList[index].Save) == 11)
              queueItemList.RemoveAt(index);
          }
          else if (MdlFile.GetVersion(queueItemList[index].Open) == 11)
          {
            FileInfo fileInfo = new FileInfo(queueItemList[index].Save);
            if (!fileInfo.Directory.Exists)
              fileInfo.Directory.Create();
            File.Copy(queueItemList[index].Open, queueItemList[index].Save, true);
            queueItemList.RemoveAt(index);
          }
        }
      }
      return queueItemList.ToArray();
    }

    private static void ReadDirectory(string directory)
    {
      foreach (string file in Directory.GetFiles(directory, "*.mdl"))
        MultiV14ToV11._Files.Add(file);
      if (!MultiV14ToV11._Recursive)
        return;
      foreach (string directory1 in Directory.GetDirectories(directory))
        MultiV14ToV11.ReadDirectory(directory1);
    }

    public static string[] CopyTextures(QueueItem[] queue, string texturedir)
    {
      List<string> stringList1 = new List<string>();
      List<string> stringList2 = new List<string>();
      List<string> stringList3 = new List<string>();
      foreach (QueueItem queueItem in queue)
      {
        string str = new FileInfo(queueItem.Save).DirectoryName + "\\textures\\";
        foreach (string texture in MdlFile.GetTextures(queueItem.Open))
        {
          if (!stringList2.Contains(texturedir + "\\" + texture + ".png"))
          {
            stringList2.Add(texturedir + "\\" + texture + ".png");
            stringList3.Add(str + texture + ".png");
          }
        }
      }
      for (int index = 0; index < stringList2.Count; ++index)
      {
        if (File.Exists(stringList2[index]))
        {
          Directory.CreateDirectory(new FileInfo(stringList3[index]).DirectoryName);
          File.Copy(stringList2[index], stringList3[index], true);
        }
        else
          stringList1.Add(Messages.FILE_NOT_FOUND + stringList2[index]);
      }
      return stringList1.ToArray();
    }

    public void Start()
    {
      if (this._StudioMdl == null)
        Messages.ThrowException("Multi.MultiV11ToV14.Start()", Messages.PARAM_NOT_SET + "StudioMdl");
      if (this._Work == null || this._Count == -1)
        Messages.ThrowException("Multi.MultiV11ToV14.Start()", Messages.PARAM_NOT_SET + "Queue");
      this._IsStopped = false;
      Directory.CreateDirectory(this._Temp);
      for (int index = 0; index < this._Work.Length; ++index)
        this._ToDo.Enqueue(index);
      while (!this._Stop)
      {
        while (this._Loopers.Count < this._Threads & this._ToDo.Count > 0)
        {
          int key = this._ToDo.Dequeue();
          Thread thread = new Thread(new ParameterizedThreadStart(this.WorkerThread));
          thread.Start((object) new KeyValuePair<int, QueueItem>(key, this._Work[key]));
          this._Loopers.Add(key, thread);
        }
        if (this._Retire.Count > 0)
          this._Loopers.Remove(this._Retire.Dequeue());
        if (this._ToDo.Count == 0)
          this._Stop = true;
        if (!this._Stop)
          Thread.Sleep(500);
      }
      while (this._Loopers.Count > 0)
      {
        if (this._Retire.Count > 0)
          this._Loopers.Remove(this._Retire.Dequeue());
        Thread.Sleep(500);
      }
      Directory.Delete(this._Temp, true);
      this._IsStopped = true;
    }

    private void WorkerThread(object parameter)
    {
      KeyValuePair<int, QueueItem> keyValuePair = (KeyValuePair<int, QueueItem>) parameter;
      DateTime now = DateTime.Now;
      ConversionStartedHandler conversionStarted = this.ConversionStarted;
      if (conversionStarted != null)
        conversionStarted((object) this, new ConversionStartedEventArgs(keyValuePair.Key));
      FileInfo fileInfo1 = new FileInfo(keyValuePair.Value.Open);
      string str = StaticMethods.StripExtension(fileInfo1.Name);
      if (!fileInfo1.Exists)
        Messages.ThrowException("Multi.MultiV14ToV11.WorkerThread(object)", Messages.FILE_NOT_FOUND + fileInfo1.FullName);
      try
      {
        MdlFileV14 mdlFileV14 = new MdlFileV14();
        mdlFileV14.Read(fileInfo1.FullName);
        Directory.CreateDirectory(this._Temp + "\\" + str);
        mdlFileV14.DumpV11Qc(this._Temp + "\\" + str + "\\" + str + ".qc");
        this._StudioMdl.Compile(this._Temp + "\\" + str + "\\" + str + ".qc", keyValuePair.Value.Options);
        if (File.Exists(this._Temp + "\\" + str + "\\" + str + ".qc.log"))
        {
          int logline = 1;
          StreamReader streamReader = new StreamReader(this._Temp + "\\" + str + "\\" + str + ".qc.log");
          while (streamReader.Peek() != -1)
          {
            string message = streamReader.ReadLine();
            if (message.StartsWith(Messages.WARNING))
              this.AddWarning(new MessageEventArgs(fileInfo1.Name, logline, message));
            else if (message.StartsWith(Messages.ERROR))
              this.AddError(new MessageEventArgs(fileInfo1.Name, logline, message));
            ++logline;
          }
          streamReader.Close();
        }
        string[] files = Directory.GetFiles(this._Temp + "\\" + str, "*.mdl");
        FileInfo fileInfo2 = new FileInfo(keyValuePair.Value.Save);
        if (files.Length == 1)
        {
          fileInfo2.Directory.Create();
          File.Copy(files[0], fileInfo2.FullName, true);
        }
        else
          this.AddError(new MessageEventArgs(fileInfo1.Name, -1, Messages.ERROR + "Studiomdl did not produce a .mdl file."));
        try
        {
          Directory.Delete(this._Temp + "\\" + str, true);
        }
        catch
        {
        }
        ConversionCompletedHandler conversionCompleted = this.ConversionCompleted;
        if (conversionCompleted != null)
          conversionCompleted((object) this, new ConversionCompletedEventArgs(keyValuePair.Key, fileInfo2.FullName, DateTime.Now - now));
      }
      catch (Exception ex)
      {
        ConversionFailedHandler conversionFailed = this.ConversionFailed;
        if (conversionFailed != null)
          conversionFailed((object) this, new ConversionFailedEventArgs(keyValuePair.Key, fileInfo1.FullName, ex));
      }
      this._Retire.Enqueue(keyValuePair.Key);
    }

    public void Stop() => this._Stop = true;

    public void WaitForStop()
    {
      if (!this._Stop)
        return;
      while (!this._IsStopped)
        Thread.Sleep(500);
    }

    public void Abort()
    {
      foreach (int key in this._Loopers.Keys)
        this._Loopers[key].Abort();
    }

    public bool IsStopped => this._IsStopped;
  }
}
