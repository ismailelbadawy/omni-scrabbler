using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class Timer : MonoBehaviour
{
    public Text uiText;
    [SerializeField] private float mainTimer=0;

    private float time;
    private bool canCount = true;
    private bool doOnce = false;
    public void Create()
    {
        time = mainTimer; 
    }
     void Update()
    {
        uiText.text = "0.00"; 
        if (time >= 0.0f && canCount)
        {
            time -= Time.deltaTime;
            uiText.text = time.ToString("F");
        }

        else if (time <= 0.0f && !doOnce)
        {
            canCount = false;
            doOnce = true;
            uiText.text = "0.00";
            time = 0.0f;
        }
    }

}
 