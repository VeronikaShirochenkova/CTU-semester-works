using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.InputSystem;
using UnityEngine.XR.Interaction.Toolkit;

public class ActivateTeleportationRay : MonoBehaviour
{
    public GameObject rightTeleportation;

    public InputActionProperty rightActivate;
    
    
    void Update()
    {
        Vector2 vec = new Vector2(0.0f, 0.1f);
        rightTeleportation.SetActive(rightActivate.action.ReadValue<Vector2>().y > vec.y);
    }
}
