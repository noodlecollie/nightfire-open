// Decompiled with JetBrains decompiler
// Type: JbnLib.Log.Collections.PropertyComparer`1
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using System;
using System.Collections.Generic;
using System.ComponentModel;

namespace JbnLib.Log.Collections
{
  public class PropertyComparer<T> : IComparer<T>
  {
    private PropertyDescriptor _property;
    private ListSortDirection _direction;

    public PropertyComparer(PropertyDescriptor property, ListSortDirection direction)
    {
      this._property = property;
      this._direction = direction;
    }

    public int Compare(T xWord, T yWord)
    {
      object propertyValue1 = this.GetPropertyValue(xWord, this._property.Name);
      object propertyValue2 = this.GetPropertyValue(yWord, this._property.Name);
      return this._direction == ListSortDirection.Ascending ? this.CompareAscending(propertyValue1, propertyValue2) : this.CompareDescending(propertyValue1, propertyValue2);
    }

    public bool Equals(T xWord, T yWord) => xWord.Equals((object) yWord);

    public int GetHashCode(T obj) => obj.GetHashCode();

    private int CompareAscending(object xValue, object yValue) => !(xValue is IComparable) ? (!xValue.Equals(yValue) ? xValue.ToString().CompareTo(yValue.ToString()) : 0) : ((IComparable) xValue).CompareTo(yValue);

    private int CompareDescending(object xValue, object yValue) => this.CompareAscending(xValue, yValue) * -1;

    private object GetPropertyValue(T value, string property) => value.GetType().GetProperty(property).GetValue((object) value, (object[]) null);
  }
}
