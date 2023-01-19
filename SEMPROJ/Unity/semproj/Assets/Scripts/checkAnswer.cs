using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using UnityEngine;
using UnityEngine.Events;

public class checkAnswer : MonoBehaviour
{
    public UnityEvent right;
    public UnityEvent half;
    public UnityEvent wrong;
    
    private List<string> rightAnswer;
    private List<string> getAnswer;
    // Start is called before the first frame update
    void Start()
    {
        rightAnswer = new List<string>();
        rightAnswer.Add("cube");
        rightAnswer.Add("horizontal");
        rightAnswer.Add("vertical");

        getAnswer = new List<string>();
    }

    private void OnTriggerEnter(Collider other)
    {
        addTerrarium(other.gameObject);
    }

    private void OnTriggerExit(Collider other)
    {
        removeTerrarium(other.gameObject);
    }

    public void addTerrarium(GameObject obj)
    {
        Debug.Log(obj.name);
        getAnswer.Add(obj.name);
    }

    public void removeTerrarium(GameObject obj)
    {
        Debug.Log(obj.name);
        getAnswer.Remove(obj.name);
    }

    public void check()
    {
        if (getAnswer.Count == 0 || getAnswer.Count > 3)
        {
            wrong.Invoke();
        }
        else
        {
            var res = getAnswer.Except(rightAnswer).ToList();
            if (res.Count > 0)
            {
                wrong.Invoke();
            }
            else if (res.Count == 0 && getAnswer.Count < 3)
            {
                half.Invoke();
            }
            else
            {
                right.Invoke();
            }
        }
    }
    
}
