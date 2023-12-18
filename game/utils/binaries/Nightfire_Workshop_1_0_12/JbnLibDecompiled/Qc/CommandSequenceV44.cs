// Decompiled with JetBrains decompiler
// Type: JbnLib.Qc.CommandSequenceV44
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Mdl;
using JbnLib.Shared;
using JbnLib.Smd;
using System.Collections.Generic;

namespace JbnLib.Qc
{
  public class CommandSequenceV44
  {
    public const string Command = "$sequence";
    private string _Name;
    private OptionActivityV44 _Activity;
    private OptionAnimation _Animation;
    private List<OptionBlend> _BlendCollection = new List<OptionBlend>();
    private OptionControl _Control = new OptionControl();
    private List<OptionEvent> _EventCollection = new List<OptionEvent>();
    private OptionFps _Fps;
    private List<string> _FileCollection = new List<string>();
    private OptionFrame _Frame;
    private OptionLoop _Loop;
    private List<OptionPivot> _PivotCollection = new List<OptionPivot>();
    private OptionRotate _Rotate;
    private OptionRTransition _RTransition;
    private OptionScale _Scale;
    private OptionTransition _Transition;
    private OptionNodeV44 _Node;
    private OptionBlendWidth _BlendWidth;

    public string Name
    {
      get => this._Name;
      set => this._Name = value;
    }

    public OptionActivityV44 Activity
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

    public OptionNodeV44 Node
    {
      get => this._Node;
      set => this._Node = value;
    }

    public OptionBlendWidth BlendWidth
    {
      get => this._BlendWidth;
      set => this._BlendWidth = value;
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
      if (this._BlendWidth != null)
        str1 = str1 + " " + this._BlendWidth.ToString();
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

    public CommandSequenceV10 ToV10()
    {
      CommandSequenceV10 v10 = new CommandSequenceV10();
      if (this._Activity != null)
        v10.Activity = new OptionActivityV10((ActivityV10) Enumerators.ConvertActivity(typeof (ActivityV44), typeof (ActivityV10), (object) this._Activity.Activity), this._Activity.ActivityWeight);
      v10.Animation = this._Animation;
      v10.BlendCollection = this._BlendCollection;
      v10.Control = this._Control;
      v10.EventCollection = this._EventCollection;
      v10.FileCollection = this._FileCollection;
      v10.Fps = this._Fps;
      v10.Frame = this._Frame;
      v10.Loop = this._Loop;
      v10.Name = this._Name;
      if (this._Node != null)
        v10.Node = StaticMethods.SmdFile.Length != 0 ? new OptionNodeV10(SmdFile.GetNode(StaticMethods.SmdFile, this._Node.EntryBone)) : new OptionNodeV10((sbyte) -1);
      v10.PivotCollection = this._PivotCollection;
      v10.Rotate = this._Rotate;
      v10.RTransition = this._RTransition;
      v10.Scale = this._Scale;
      v10.Transition = this._Transition;
      return v10;
    }

    public CommandSequenceV11 ToV11()
    {
      CommandSequenceV11 v11 = new CommandSequenceV11();
      if (this._Activity != null)
        v11.Activity = new OptionActivityV11((ActivityV11) Enumerators.ConvertActivity(typeof (ActivityV44), typeof (ActivityV11), (object) this._Activity.Activity), this._Activity.ActivityWeight);
      v11.Animation = this._Animation;
      v11.BlendCollection = this._BlendCollection;
      v11.Control = this._Control;
      v11.EventCollection = this._EventCollection;
      v11.FileCollection = this._FileCollection;
      v11.Fps = this._Fps;
      v11.Frame = this._Frame;
      v11.Loop = this._Loop;
      v11.Name = this._Name;
      if (this._Node != null)
        v11.Node = StaticMethods.SmdFile.Length != 0 ? new OptionNodeV10(SmdFile.GetNode(StaticMethods.SmdFile, this._Node.EntryBone)) : new OptionNodeV10((sbyte) -1);
      v11.PivotCollection = this._PivotCollection;
      v11.Rotate = this._Rotate;
      v11.RTransition = this._RTransition;
      v11.Scale = this._Scale;
      v11.Transition = this._Transition;
      return v11;
    }
  }
}
