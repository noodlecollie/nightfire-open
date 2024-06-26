﻿// Decompiled with JetBrains decompiler
// Type: JbnLib.Fgd.Types.PointClass
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Lang;
using JbnLib.Shared;
using System.Collections.Generic;
using System.Text;

namespace JbnLib.Fgd.Types
{
  public class PointClass
  {
    private string _Name;
    private string _Text;
    private string _Group;
    private List<string> _Bases = new List<string>();
    private Size _Size;
    private Color _Color;
    private string _Shape;
    private string _Studio;
    private ClassSprite _Sprite;
    private ClassIconSprite _IconSprite;
    private List<Choices> _ChoicesCollection = new List<Choices>();
    private List<Color255> _ColorCollection = new List<Color255>();
    private List<Flags> _FlagsCollection = new List<Flags>();
    private List<Integer> _IntegerCollection = new List<Integer>();
    private List<Sound> _SoundCollection = new List<Sound>();
    private List<JbnLib.Fgd.Types.Sprite> _SpriteCollection = new List<JbnLib.Fgd.Types.Sprite>();
    private List<String> _StringCollection = new List<String>();
    private List<JbnLib.Fgd.Types.Studio> _StudioCollection = new List<JbnLib.Fgd.Types.Studio>();
    private List<TargetDestination> _TargetDestinationCollection = new List<TargetDestination>();
    private List<TargetSource> _TargetSourceCollection = new List<TargetSource>();

    public string Name
    {
      get => this._Name;
      set => this._Name = value;
    }

    public string Text
    {
      get => this._Text;
      set => this._Text = value;
    }

    public string Group
    {
      get => this._Group;
      set => this._Group = value;
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

    public string Shape
    {
      get => this._Shape;
      set => this._Shape = value;
    }

    public string Studio
    {
      get => this._Studio;
      set => this._Studio = value;
    }

    public ClassSprite Sprite
    {
      get => this._Sprite;
      set => this._Sprite = value;
    }

    public ClassIconSprite IconSprite
    {
      get => this._IconSprite;
      set => this._IconSprite = value;
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

    public List<JbnLib.Fgd.Types.Sprite> SpriteCollection
    {
      get => this._SpriteCollection;
      set => this._SpriteCollection = value;
    }

    public List<String> StringCollection
    {
      get => this._StringCollection;
      set => this._StringCollection = value;
    }

    public List<JbnLib.Fgd.Types.Studio> StudioCollection
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
      string[] strArray = data.Substring(0, data.IndexOf('[')).Trim().Split('=', ':');
      this._Name = strArray[1].Trim();
      this._Text = strArray[2].Trim().Trim('"');
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
      this._Group = StaticMethods.PullData(data1, "group").Trim('"');
      this._Shape = StaticMethods.PullData(data1, "shape");
      this._Studio = StaticMethods.PullData(data1, "studio").Trim('"');
      this._Sprite = new ClassSprite(StaticMethods.PullData(data1, "sprite"));
      this._IconSprite = new ClassIconSprite(StaticMethods.PullData(data1, "iconsprite"));
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
        Messages.ThrowException("Fgd.PointClass.Parse(string)", Messages.CLASSNAME + this._Name + " " + Messages.MISSING_RIGHT_BRACKETS + (object) (num1 - num2));
      if (num2 > num1)
        Messages.ThrowException("Fgd.PointClass.Parse(string)", Messages.CLASSNAME + this._Name + " " + Messages.MISSING_LEFT_BRACKETS + (object) (num2 - num1));
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
          if (str5.ToLower().Contains("(sprite)"))
            this._SpriteCollection.Add(new JbnLib.Fgd.Types.Sprite(str5));
          if (str5.ToLower().Contains("(string)"))
            this._StringCollection.Add(new String(str5));
          if (str5.ToLower().Contains("(studio)"))
            this._StudioCollection.Add(new JbnLib.Fgd.Types.Studio(str5));
          if (str5.ToLower().Contains("(target_destination)"))
            this._TargetDestinationCollection.Add(new TargetDestination(str5));
          if (str5.ToLower().Contains("(target_source)"))
            this._TargetSourceCollection.Add(new TargetSource(str5));
        }
      }
    }

    public new string ToString()
    {
      string str1 = "@PointClass";
      if (this._Group.Length > 0)
        str1 = str1 + " group(\"" + this._Group + "\")";
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
      if (this._Shape.Length > 0)
        str1 = str1 + " shape(" + this._Shape + ")";
      if (this._Studio.Length > 0)
        str1 = str1 + " studio(\"" + this._Studio + "\")";
      if (this._Sprite.Sprite.Length > 0)
        str1 = str1 + " sprite(" + this._Sprite.ToString() + ")";
      if (this._IconSprite.Sprite.Length > 0)
        str1 = str1 + " iconsprite(" + this._IconSprite.ToString() + ")";
      string str3 = str1 + " = " + this._Name;
      if (this._Text.Length > 0)
        str3 = str3 + " : \"" + this._Text + "\"";
      string str4 = str3 + "\n[\n";
      foreach (Choices choices in this._ChoicesCollection)
      {
        string str5 = choices.ToString();
        char[] chArray = new char[1]{ '\n' };
        foreach (string str6 in str5.Split(chArray))
          str4 = str4 + "\t" + str6 + "\n";
      }
      foreach (Color255 color in this._ColorCollection)
      {
        string str7 = color.ToString();
        char[] chArray = new char[1]{ '\n' };
        foreach (string str8 in str7.Split(chArray))
          str4 = str4 + "\t" + str8 + "\n";
      }
      foreach (Flags flags in this._FlagsCollection)
      {
        string str9 = flags.ToString();
        char[] chArray = new char[1]{ '\n' };
        foreach (string str10 in str9.Split(chArray))
          str4 = str4 + "\t" + str10 + "\n";
      }
      foreach (Integer integer in this._IntegerCollection)
      {
        string str11 = integer.ToString();
        char[] chArray = new char[1]{ '\n' };
        foreach (string str12 in str11.Split(chArray))
          str4 = str4 + "\t" + str12 + "\n";
      }
      foreach (Sound sound in this._SoundCollection)
      {
        string str13 = sound.ToString();
        char[] chArray = new char[1]{ '\n' };
        foreach (string str14 in str13.Split(chArray))
          str4 = str4 + "\t" + str14 + "\n";
      }
      foreach (JbnLib.Fgd.Types.Sprite sprite in this._SpriteCollection)
      {
        string str15 = sprite.ToString();
        char[] chArray = new char[1]{ '\n' };
        foreach (string str16 in str15.Split(chArray))
          str4 = str4 + "\t" + str16 + "\n";
      }
      foreach (String @string in this._StringCollection)
      {
        string str17 = @string.ToString();
        char[] chArray = new char[1]{ '\n' };
        foreach (string str18 in str17.Split(chArray))
          str4 = str4 + "\t" + str18 + "\n";
      }
      foreach (JbnLib.Fgd.Types.Studio studio in this._StudioCollection)
      {
        string str19 = studio.ToString();
        char[] chArray = new char[1]{ '\n' };
        foreach (string str20 in str19.Split(chArray))
          str4 = str4 + "\t" + str20 + "\n";
      }
      foreach (TargetDestination targetDestination in this._TargetDestinationCollection)
      {
        string str21 = targetDestination.ToString();
        char[] chArray = new char[1]{ '\n' };
        foreach (string str22 in str21.Split(chArray))
          str4 = str4 + "\t" + str22 + "\n";
      }
      foreach (TargetSource targetSource in this._TargetSourceCollection)
      {
        string str23 = targetSource.ToString();
        char[] chArray = new char[1]{ '\n' };
        foreach (string str24 in str23.Split(chArray))
          str4 = str4 + "\t" + str24 + "\n";
      }
      return str4 + "]\n\n";
    }
  }
}
