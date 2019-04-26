using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
public class Scores : MonoBehaviour
{
    public  Text Name;
    public  Text Score;
    public int score;

    public void Start(string n)
    {
        score = 0;
        Name.text = n;
        Score.text = "0";
        score = 7;
    }

    public void Update(int newscore)
    {
        score = newscore;
        Score.text= newscore.ToString();
    }
}
