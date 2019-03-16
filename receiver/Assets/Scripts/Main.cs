using System;
using System.Collections.Generic;
using UnityEngine;
using Assets.Scripts.ExtensionClasses;

public class Main : MonoBehaviour
{
    [SerializeField]
    private string m_COMPortName = string.Empty;
    [SerializeField]
    private int m_BaudRate = 9600;

    [SerializeField]
    private float m_Scale = 1f;

    [SerializeField, ReadOnlyProperty]
    private List<SonarInput> m_Data = new List<SonarInput>(360 * 10);

    private void Start()
    {
        // 300, 600, 1200, 2400, 4800, 9600, 14400, 19200, 28800, 38400, 57600, 115200
        SerialPortSettings settings = SerialPortSettings.Default;
        settings.BaudRate = m_BaudRate;
        settings.LoopDelay = 50;

        //SonarSerialReceiver.OnUpdate += UpdateDataRevolving;
        SonarSerialReceiver.OnUpdate += UpdateDataOscillating;
        SonarSerialReceiver.Begin(m_COMPortName, settings);
    }

    private void OnDestroy()
    {
        SonarSerialReceiver.End();
    }

    SonarInput m_Origin;
    Vector3 m_LastCrossProduct;
    bool m_InitialCall = true;
    private void UpdateDataRevolving(SonarInput data)
    {
        if(m_InitialCall)
        {
            m_Origin = data;
            m_InitialCall = false;
        }

        data = new SonarInput(data.Distance, data.Angle - 90f);
        Vector3 crossProduct = MathTools.CrossProduct(data.Direction, m_Origin.Direction);
        lock(m_Data)
        {
            if((m_LastCrossProduct.z < 0f && crossProduct.z >= 0f) || (m_LastCrossProduct.z > 0f && crossProduct.z <= 0f))
            {
                m_Data.Clear();
            }

            m_Data.Add(data);
        }

        m_LastCrossProduct = crossProduct;
    }

    float m_LastAngle = 0f;
    int m_LastDirection = 0;
    private void UpdateDataOscillating(SonarInput data)
    {
        data = new SonarInput(data.Distance, data.Angle - 90f);
        int direction = MathTools.Sign(data.Angle - m_LastAngle);

        lock(m_Data)
        {
            if(direction != m_LastDirection)
            {
                m_Data.Clear();
                m_LastDirection = direction;
            }

            m_Data.Add(data);
        }

        m_LastAngle = data.Angle;
    }

    private void OnDrawGizmos()
    {
        if(m_Data.Count <= 1)
            return;

        SonarInput lastInput;
        lock(m_Data)
        {
            Gizmos.color = Color.red;
            for(int i = 1; i < m_Data.Count; i++)
            {
                Gizmos.DrawLine(m_Data[i - 1].Position * m_Scale, m_Data[i].Position * m_Scale);
            }

            lastInput = m_Data[m_Data.Count - 1];
        }

        Gizmos.color = Color.yellow;
        //Gizmos.DrawLine(Vector3.zero, m_Data[m_Data.Count - 1].Position.normalized * 5f);
        Gizmos.DrawLine(Vector3.zero, lastInput.Position * m_Scale);
        Gizmos.color = Color.yellow.SetAlpha(0.5f);
        Gizmos.DrawLine(lastInput.Position * m_Scale, lastInput.Position.normalized * (5f * m_Scale));
    }
}
