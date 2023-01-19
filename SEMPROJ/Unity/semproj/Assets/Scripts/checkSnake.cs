using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Events;

public class checkSnake : MonoBehaviour
{
    public UnityEvent right;
    public UnityEvent wrong;
    
    public string trueSnakeType;
    private string getSnakeType;
    private string tag;
    void Start()
    {
        getSnakeType = "";
        tag = "snake";
    }

    private void OnTriggerEnter(Collider other)
    {
        if (other.gameObject.CompareTag(tag))
        {
            getSnakeType = other.gameObject.name; 
        }
        
    }

    private void OnTriggerExit(Collider other)
    {
        if (other.gameObject.CompareTag(tag))
        {
            getSnakeType = ""; 
        }
    }

    public void checkSnakeType()
    {
        if (getSnakeType == trueSnakeType)
        {
            right.Invoke();
        }
        else
        {
            wrong.Invoke();
        }
    }
}
