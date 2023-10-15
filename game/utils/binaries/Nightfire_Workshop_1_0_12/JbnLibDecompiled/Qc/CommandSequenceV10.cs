// Decompiled with JetBrains decompiler
// Type: JbnLib.Qc.CommandSequenceV10
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Mdl;
using JbnLib.Shared;
using JbnLib.Smd;
using System.Collections.Generic;

namespace JbnLib.Qc
{
  public class CommandSequenceV10
  {
    public const string Command = "$sequence";
    private string _Name;
    private OptionActivityV10 _Activity;
    private OptionAnimation _Animation;
    private List<OptionBlend> _BlendCollection = new List<OptionBlend>();
    private OptionControl _Control = new OptionControl();
    private List<OptionEvent> _EventCollection = new List<OptionEvent>();
    private OptionFps _Fps;
    private List<string> _FileCollection = new List<string>();
    private OptionFrame _Frame;
    private OptionLoop _Loop;
    private OptionNodeV10 _Node;
    private List<OptionPivot> _PivotCollection = new List<OptionPivot>();
    private OptionRotate _Rotate;
    private OptionRTransition _RTransition;
    private OptionScale _Scale;
    private OptionTransition _Transition;

    public string Name
    {
      get => this._Name;
      set => this._Name = value;
    }

    public OptionActivityV10 Activity
    {
      get => this._Activity;
      set => this._Activity = value;
    }

    public OptionAnimation Animation
    {
      get => this._Animation;
      set => this._Animation = value;
    }

    public List<OptionBlend> BlendCollection
    {
      get => this._BlendCollection;
      set => this._BlendCollection = value;
    }

    public OptionControl Control
    {
      get => this._Control;
      set => this._Control = value;
    }

    public List<OptionEvent> EventCollection
    {
      get => this._EventCollection;
      set => this._EventCollection = value;
    }

    public OptionFps Fps
    {
      get => this._Fps;
      set => this._Fps = value;
    }

    public List<string> FileCollection
    {
      get => this._FileCollection;
      set => this._FileCollection = value;
    }

    public OptionFrame Frame
    {
      get => this._Frame;
      set => this._Frame = value;
    }

    public OptionLoop Loop
    {
      get => this._Loop;
      set => this._Loop = value;
    }

    public OptionNodeV10 Node
    {
      get => this._Node;
      set => this._Node = value;
    }

    public List<OptionPivot> PivotCollection
    {
      get => this._PivotCollection;
      set => this._PivotCollection = value;
    }

    public OptionRotate Rotate
    {
      get => this._Rotate;
      set => this._Rotate = value;
    }

    public OptionRTransition RTransition
    {
      get => this._RTransition;
      set => this._RTransition = value;
    }

    public OptionScale Scale
    {
      get => this._Scale;
      set => this._Scale = value;
    }

    public OptionTransition Transition
    {
      get => this._Transition;
      set => this._Transition = value;
    }

    public new string ToString()
    {
      string str1 = "$sequence \"" + this._Name + "\"";
      foreach (string file in this._FileCollection)
        str1 = str1 + " \"" + file + "\"";
      if (this._Activity != null)
        str1 = str1 + " " + this._Activity.ToString();
      if (this._Animation != null)
        str1 = str1 + " " + this._Animation.ToString();
      foreach (OptionBlend blend in this._BlendCollection)
        str1 = str1 + " " + blend.ToString();
      if (this._Control.Flags != MotionFlags.None)
        str1 = str1 + " " + this._Control.ToString();
      if (this._Fps != null)
        str1 = str1 + " " + this._Fps.ToString();
      if (this._Frame != null)
        str1 = str1 + " " + this._Frame.ToString();
      if (this._Loop != null)
        str1 = str1 + " " + this._Loop.ToString();
      if (this._Node != null)
        str1 = str1 + " " + this._Node.ToString();
      if (this._Rotate != null)
        str1 = str1 + " " + this._Rotate.ToString();
      if (this._RTransition != null)
        str1 = str1 + " " + this._RTransition.ToString();
      if (this._Scale != null)
        str1 = str1 + " " + this._Scale.ToString();
      if (this._Transition != null)
        str1 = str1 + " " + this._Transition.ToString();
      if (this._PivotCollection.Count > 0)
      {
        foreach (OptionPivot pivot in this._PivotCollection)
          str1 = str1 + " " + pivot.ToString();
      }
      if (this._EventCollection.Count > 0)
      {
        string str2 = str1 + " {\r\n";
        foreach (OptionEvent optionEvent in this._EventCollection)
          str2 = str2 + "\t" + optionEvent.ToString() + "\r\n";
        str1 = str2 + "}";
      }
      return str1;
    }

    public CommandSequenceV11 ToV11()
    {
      CommandSequenceV11 v11 = new CommandSequenceV11();
      if (this._Activity != null)
        v11.Activity = new OptionActivityV11((ActivityV11) Enumerators.ConvertActivity(typeof (ActivityV10), typeof (ActivityV11), (object) this._Activity.Activity), this._Activity.ActivityWeight);
      v11.Animation = this._Animation;
      v11.BlendCollection = this._BlendCollection;
      v11.Control = this._Control;
      v11.EventCollection = this._EventCollection;
      v11.FileCollection = this._FileCollection;
      v11.Fps = this._Fps;
      v11.Frame = this._Frame;
      v11.Loop = this._Loop;
      v11.Name = this._Name;
      v11.Node = this._Node;
      v11.PivotCollection = this._PivotCollection;
      v11.Rotate = this._Rotate;
      v11.RTransition = this._RTransition;
      v11.Scale = this._Scale;
      v11.Transition = this._Transition;
      return v11;
    }

    public CommandSequenceV44 ToV44()
    {
      CommandSequenceV44 v44 = new CommandSequenceV44();
      if (this._Activity != null)
        v44.Activity = new OptionActivityV44((ActivityV44) Enumerators.ConvertActivity(typeof (ActivityV10), typeof (ActivityV44), (object) this._Activity.Activity), this._Activity.ActivityWeight);
      v44.Animation = this._Animation;
      v44.BlendCollection = this._BlendCollection;
      v44.Control = this._Control;
      v44.EventCollection = this._EventCollection;
      v44.FileCollection = this._FileCollection;
      v44.Fps = this._Fps;
      v44.Frame = this._Frame;
      v44.Loop = this._Loop;
      v44.Name = this._Name;
      if (this._Node != null)
        v44.Node = StaticMethods.SmdFile.Length != 0 ? new OptionNodeV44(SmdFile.GetNode(StaticMethods.SmdFile, this._Node.EntryBone)) : new OptionNodeV44(this._Node.EntryBone.ToString());
      v44.PivotCollection = this._PivotCollection;
      v44.Rotate = this._Rotate;
      v44.RTransition = this._RTransition;
      v44.Scale = this._Scale;
      v44.Transition = this._Transition;
      return v44;
    }
  }
}
