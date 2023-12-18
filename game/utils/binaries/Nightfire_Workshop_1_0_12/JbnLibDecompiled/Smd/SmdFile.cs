// Decompiled with JetBrains decompiler
// Type: JbnLib.Smd.SmdFile
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Shared;
using System;
using System.Collections.Generic;
using System.IO;

namespace JbnLib.Smd
{
  public class SmdFile
  {
    internal List<Node> _NodeCollection;
    internal List<Frame> _TimeCollection;
    internal bool _Reference;
    internal FileTokenizer _Tokenizer;
    public const string STRING_VERSION_ONE = "version 1";
    public const string STRING_SKELETON = "skeleton";
    public const string STRING_TRIANGLES = "triangles";
    public const string STRING_NODES = "nodes";
    public const string STRING_TIME = "time";
    public const string STRING_END = "end";

    public List<Node> NodeCollection
    {
      get => this._NodeCollection;
      set => this._NodeCollection = value;
    }

    public List<Frame> TimeCollection
    {
      get => this._TimeCollection;
      set => this._TimeCollection = value;
    }

    public bool HasNode(sbyte node)
    {
      foreach (Node node1 in this._NodeCollection)
      {
        if ((int) node1.Index == (int) node)
          return true;
      }
      return false;
    }

    public void SortNodes()
    {
      Node[] array = new Node[this._NodeCollection.Count];
      this._NodeCollection.CopyTo(array);
      this._NodeCollection.Clear();
      Array.Sort<Node>(array, (IComparer<Node>) new NodeComparer());
      foreach (Node node in array)
        this._NodeCollection.Add(node);
    }

    public int Rotate(double degrees)
    {
      int num1 = 0;
      float single = Convert.ToSingle(StaticMethods.DegToRad(degrees));
      for (int index1 = 0; index1 < this._TimeCollection.Count; ++index1)
      {
        for (int index2 = 0; index2 < this._TimeCollection[index1].Bones.Count; ++index2)
        {
          if (this._NodeCollection[this._TimeCollection[index1].Bones[index2].Node].Parent == (sbyte) -1)
          {
            this._TimeCollection[index1].Bones[index2].Rotation.Z += single;
            StaticMethods.Quadrant quadrant = StaticMethods.GetQuadrant(this._TimeCollection[index1].Bones[index2].Position.X, this._TimeCollection[index1].Bones[index2].Position.Y);
            double num2 = Math.Sqrt(Math.Pow((double) this._TimeCollection[index1].Bones[index2].Position.X, 2.0) + Math.Pow((double) this._TimeCollection[index1].Bones[index2].Position.Y, 2.0));
            double num3 = Math.Atan((double) this._TimeCollection[index1].Bones[index2].Position.Y / (double) this._TimeCollection[index1].Bones[index2].Position.X) + (double) single;
            this._TimeCollection[index1].Bones[index2].Position.X = Convert.ToSingle(Math.Cos(num3) * num2);
            this._TimeCollection[index1].Bones[index2].Position.Y = Convert.ToSingle(Math.Sin(num3) * num2);
            if (quadrant == StaticMethods.Quadrant.II || quadrant == StaticMethods.Quadrant.III)
            {
              this._TimeCollection[index1].Bones[index2].Position.X *= -1f;
              this._TimeCollection[index1].Bones[index2].Position.Y *= -1f;
            }
            ++num1;
          }
        }
      }
      return num1;
    }

    public static bool HasTriangles(string file)
    {
      StreamReader streamReader = new StreamReader(file);
      while (streamReader.Peek() != -1)
      {
        if (streamReader.ReadLine().Trim().ToLower() == "triangles")
        {
          streamReader.Close();
          return true;
        }
      }
      streamReader.Close();
      return false;
    }

    public static string[] GetNodes(string file)
    {
      List<Node> nodeList = new List<Node>();
      StreamReader streamReader = new StreamReader(file);
      while (streamReader.Peek() != -1)
      {
        if (streamReader.ReadLine().Trim().ToLower() == "nodes")
        {
          for (string line = streamReader.ReadLine(); line.Trim().ToLower() != "end"; line = streamReader.ReadLine())
            nodeList.Add(new Node(line));
          break;
        }
      }
      streamReader.Close();
      sbyte num = -1;
      foreach (Node node in nodeList)
      {
        if ((int) node.Index >= (int) num)
          num = node.Index;
      }
      string[] nodes = new string[(int) num + 1];
      foreach (Node node in nodeList)
        nodes[(int) node.Index] = node.Name;
      return nodes;
    }

    public static sbyte GetNode(string file, string node)
    {
      string[] nodes = SmdFile.GetNodes(file);
      for (sbyte node1 = 0; (int) node1 < nodes.Length; ++node1)
      {
        if (nodes[(int) node1] == node)
          return node1;
      }
      return -1;
    }

    public static string GetNode(string file, sbyte node)
    {
      string[] nodes = SmdFile.GetNodes(file);
      try
      {
        return nodes[(int) node];
      }
      catch
      {
        return "ERROR";
      }
    }

    public static bool IsReference(string file)
    {
      bool flag = false;
      StreamReader streamReader = new StreamReader(file);
      while (streamReader.Peek() != -1)
      {
        if (streamReader.ReadLine().Trim().ToLower() == "triangles")
        {
          flag = true;
          break;
        }
      }
      streamReader.Close();
      return flag;
    }
  }
}
