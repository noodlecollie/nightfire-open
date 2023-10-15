// Decompiled with JetBrains decompiler
// Type: JbnLib.Fgd.Types.BaseClass
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Lang;
using JbnLib.Shared;
using System.Collections.Generic;
using System.Text;

namespace JbnLib.Fgd.Types
{
  public class BaseClass
  {
    private string _Name;
    private List<string> _Bases = new List<string>();
    private Size _Size;
    private Color _Color;
    private List<Choices> _ChoicesCollection = new List<Choices>();
    private List<Color255> _ColorCollection = new List<Color255>();
    private List<Flags> _FlagsCollection = new List<Flags>();
    private List<Integer> _IntegerCollection = new List<Integer>();
    private List<Sound> _SoundCollection = new List<Sound>();
    private List<String> _StringCollection = new List<String>();
    private List<Studio> _StudioCollection = new List<Studio>();
    private List<TargetDestination> _TargetDestinationCollection = new List<TargetDestination>();
    private List<TargetSource> _TargetSourceCollection = new List<TargetSource>();

    public string Name
    {
      get => this._Name;
      set => this._Name = value;
    }

    public List<string> Bases
    {
      get => this._Bases;
      set => this._Bases = value;
    }

    public Size Size
    {
      get => this._Size;
      set => this._Size = value;
    }

    public Color Color
    {
      get => this._Color;
      set => this._Color = value;
    }

    public List<Choices> ChoicesCollection
    {
      get => this._ChoicesCollection;
      set => this._ChoicesCollection = value;
    }

    public List<Color255> ColorCollection
    {
      get => this._ColorCollection;
      set => this._ColorCollection = value;
    }

    public List<Flags> FlagsCollection
    {
      get => this._FlagsCollection;
      set => this._FlagsCollection = value;
    }

    public List<Integer> IntegerCollection
    {
      get => this._IntegerCollection;
      set => this._IntegerCollection = value;
    }

    public List<Sound> SoundCollection
    {
      get => this._SoundCollection;
      set => this._SoundCollection = value;
    }

    public List<String> StringCollection
    {
      get => this._StringCollection;
      set => this._StringCollection = value;
    }

    public List<Studio> StudioCollection
    {
      get => this._StudioCollection;
      set => this._StudioCollection = value;
    }

    public List<TargetDestination> TargetDestinationCollection
    {
      get => this._TargetDestinationCollection;
      set => this._TargetDestinationCollection = value;
    }

    public List<TargetSource> TargetSourceCollection
    {
      get => this._TargetSourceCollection;
      set => this._TargetSourceCollection = value;
    }

    public void Parse(string data)
    {
      string[] strArray = data.Substring(0, data.IndexOf('[')).Trim().Split('=');
      this._Name = strArray[1].Trim();
      string data1 = strArray[0].Trim();
      string str1 = StaticMethods.PullData(data1, "base");
      char[] chArray1 = new char[1]{ ',' };
      foreach (string str2 in str1.Split(chArray1))
      {
        string str3 = str2.Trim();
        if (str3.Length > 0)
          this._Bases.Add(str3);
      }
      string size = StaticMethods.PullData(data1, "size");
      this._Size = size.Length <= 0 ? new Size() : new Size(size);
      string line1 = StaticMethods.PullData(data1, "color");
      this._Color = line1.Length <= 0 ? new Color() : new Color(line1);
      byte[] bytes = Encoding.ASCII.GetBytes(data);
      int num1 = 0;
      int num2 = 0;
      foreach (byte num3 in bytes)
      {
        switch (num3)
        {
          case 91:
            ++num1;
            break;
          case 93:
            ++num2;
            break;
        }
      }
      if (num1 > num2)
        Messages.ThrowException("Fgd.BaseClass.Parse(string)", Messages.CLASSNAME + this._Name + " " + Messages.MISSING_RIGHT_BRACKETS + (object) (num1 - num2));
      if (num2 > num1)
        Messages.ThrowException("Fgd.BaseClass.Parse(string)", Messages.CLASSNAME + this._Name + " " + Messages.MISSING_LEFT_BRACKETS + (object) (num2 - num1));
      string line2 = "";
      bool flag1 = false;
      bool flag2 = true;
      string str4 = data.Substring(data.IndexOf('[') + 1, data.LastIndexOf(']') - data.IndexOf('[') - 1).Trim();
      char[] chArray2 = new char[1]{ '\n' };
      foreach (string str5 in str4.Split(chArray2))
      {
        if (flag1)
        {
          line2 = line2 + "\n" + str5;
          if (str5.Contains("]"))
          {
            flag1 = false;
            if (flag2)
              this._ChoicesCollection.Add(new Choices(line2));
            else
              this._FlagsCollection.Add(new Flags(line2));
          }
        }
        else
        {
          if (str5.ToLower().Contains("(choices)"))
          {
            flag1 = true;
            flag2 = true;
            line2 = str5;
          }
          if (str5.ToLower().Contains("(color255)"))
            this._ColorCollection.Add(new Color255(str5));
          if (str5.ToLower().Contains("(flags)"))
          {
            flag1 = true;
            flag2 = false;
            line2 = str5;
          }
          if (str5.ToLower().Contains("(integer)"))
            this._IntegerCollection.Add(new Integer(str5));
          if (str5.ToLower().Contains("(sound)"))
            this._SoundCollection.Add(new Sound(str5));
          if (str5.ToLower().Contains("(string)"))
            this._StringCollection.Add(new String(str5));
          if (str5.ToLower().Contains("(studio)"))
            this._StudioCollection.Add(new Studio(str5));
          if (str5.ToLower().Contains("(target_destination)"))
            this._TargetDestinationCollection.Add(new TargetDestination(str5));
          if (str5.ToLower().Contains("(target_source)"))
            this._TargetSourceCollection.Add(new TargetSource(str5));
        }
      }
    }

    public new string ToString()
    {
      string str1 = "@BaseClass";
      if (this._Bases.Count > 0)
      {
        string str2 = str1 + " base(" + this._Bases[0];
        for (int index = 1; index < this._Bases.Count; ++index)
          str2 = str2 + ", " + this._Bases[index];
        str1 = str2 + ")";
      }
      if (this._Size.HasValue)
        str1 = str1 + " size(" + this._Size.ToString() + ")";
      if (this._Color.HasValue)
        str1 = str1 + " color(" + this._Color.ToString() + ")";
      string str3 = str1 + " = " + this._Name + "\n[\n";
      foreach (Choices choices in this._ChoicesCollection)
      {
        string str4 = choices.ToString();
        char[] chArray = new char[1]{ '\n' };
        foreach (string str5 in str4.Split(chArray))
          str3 = str3 + "\t" + str5 + "\n";
      }
      foreach (Color255 color in this._ColorCollection)
      {
        string str6 = color.ToString();
        char[] chArray = new char[1]{ '\n' };
        foreach (string str7 in str6.Split(chArray))
          str3 = str3 + "\t" + str7 + "\n";
      }
      foreach (Flags flags in this._FlagsCollection)
      {
        string str8 = flags.ToString();
        char[] chArray = new char[1]{ '\n' };
        foreach (string str9 in str8.Split(chArray))
          str3 = str3 + "\t" + str9 + "\n";
      }
      foreach (Integer integer in this._IntegerCollection)
      {
        string str10 = integer.ToString();
        char[] chArray = new char[1]{ '\n' };
        foreach (string str11 in str10.Split(chArray))
          str3 = str3 + "\t" + str11 + "\n";
      }
      foreach (Sound sound in this._SoundCollection)
      {
        string str12 = sound.ToString();
        char[] chArray = new char[1]{ '\n' };
        foreach (string str13 in str12.Split(chArray))
          str3 = str3 + "\t" + str13 + "\n";
      }
      foreach (String @string in this._StringCollection)
      {
        string str14 = @string.ToString();
        char[] chArray = new char[1]{ '\n' };
        foreach (string str15 in str14.Split(chArray))
          str3 = str3 + "\t" + str15 + "\n";
      }
      foreach (Studio studio in this._StudioCollection)
      {
        string str16 = studio.ToString();
        char[] chArray = new char[1]{ '\n' };
        foreach (string str17 in str16.Split(chArray))
          str3 = str3 + "\t" + str17 + "\n";
      }
      foreach (TargetDestination targetDestination in this._TargetDestinationCollection)
      {
        string str18 = targetDestination.ToString();
        char[] chArray = new char[1]{ '\n' };
        foreach (string str19 in str18.Split(chArray))
          str3 = str3 + "\t" + str19 + "\n";
      }
      foreach (TargetSource targetSource in this._TargetSourceCollection)
      {
        string str20 = targetSource.ToString();
        char[] chArray = new char[1]{ '\n' };
        foreach (string str21 in str20.Split(chArray))
          str3 = str3 + "\t" + str21 + "\n";
      }
      return str3 + "]\n\n";
    }
  }
}
