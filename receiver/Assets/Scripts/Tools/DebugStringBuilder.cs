using System;
using System.Diagnostics;
using System.Text;

public enum DebugLogMode
{
    None = 0,
    Regular = 1,
    Warning = 2,
    Error = 3,
    Assertion = 4
};

public sealed class DebugStringBuilder
{
    private StringBuilder m_Content;

    public DebugLogMode LogMode { get; set; } = DebugLogMode.None;

    public static DebugStringBuilder operator +(DebugStringBuilder lhs, object rhs)
    {
        return lhs.Append(rhs);
    }

    public DebugStringBuilder Append(string format, params object[] args)
    {
        m_Content.Append(string.Format(format, args));
        return this;
    }

    public DebugStringBuilder Append(object value)
    {
        m_Content.Append(value);
        return this;
    }

    public DebugStringBuilder AppendLine(string format, params object[] args)
    {
        m_Content.AppendLine(string.Format(format, args));
        return this;
    }

    public DebugStringBuilder AppendLine(object value)
    {
        m_Content.AppendLine(value.ToString());
        return this;
    }

    public DebugStringBuilder AppendLine()
    {
        m_Content.AppendLine();
        return this;
    }

    [Conditional("DEBUG")]
    public void WarnIf(bool condition)
    {
        if(!condition || LogMode != DebugLogMode.None) return;

        LogMode = DebugLogMode.Warning;
    }

    [Conditional("DEBUG")]
    public void ErrorIf(bool condition)
    {
        if(!condition || LogMode != DebugLogMode.None) return;

        LogMode = DebugLogMode.Error;
    }

    [Conditional("DEBUG")]
    public void AssertIf(bool condition)
    {
        if(!condition || LogMode != DebugLogMode.None) return;

        LogMode = DebugLogMode.Assertion;
    }

    [Conditional("DEBUG")]
    public void Print()
    {
        switch(LogMode)
        {
            case DebugLogMode.None:
            case DebugLogMode.Regular:
                UnityEngine.Debug.Log(m_Content);
                break;
            case DebugLogMode.Warning:
                UnityEngine.Debug.LogWarning(m_Content);
                break;
            case DebugLogMode.Error:
                UnityEngine.Debug.LogError(m_Content);
                break;
            case DebugLogMode.Assertion:
                UnityEngine.Debug.LogAssertion(m_Content);
                break;
            default:
                throw new System.NotImplementedException();
        }
    }

    public override string ToString()
    {
        return m_Content.ToString();
    }

    public DebugStringBuilder(string message, DebugLogMode logMode = DebugLogMode.None)
    {
        m_Content = new StringBuilder(message);
        LogMode = logMode;
    }

    public DebugStringBuilder(DebugLogMode logMode = DebugLogMode.None)
    {
        m_Content = new StringBuilder();
        LogMode = logMode;
    }
}
