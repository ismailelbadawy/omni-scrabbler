using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO;

public class GameManager : MonoBehaviour
{

    public Scores p1;
    public Scores p2;
    public Rack rack;
    public MakeLetter l;
    public Timer timer;
    string Letter;
    int number;
    List<string> input = new List<string>();
    List<string> input2 = new List<string>();
    string[] line1;
    StreamReader sr;

    void Start()
    {
        timer.Create();
        sr = new StreamReader("Game.txt");
        line1 = sr.ReadLine().Split();
        p1.Start(line1[0]);
        p2.Start(line1[1]);
       
        do
        {
            line1 = sr.ReadLine().Split();
            if (line1[0] == "R")
            {
                if (line1[1] == "CREATE")
                {
                    input.Add(line1[2]);
                    input.Add(line1[3]);
                    input.Add(line1[4]);
                    input.Add(line1[5]);
                    input.Add(line1[6]);
                    input.Add(line1[7]);
                    input.Add(line1[8]);
                    rack.Create(ref input);
                }
                else if (line1[1] == "ADD")
                {
                    for (int i = 2; i < line1.Length; i++)
                    {
                        input.Add(line1[i]);
                    }

                    rack.Update(Letter, 1, input);

                    input.Clear();

                }
            }
            else if (line1[0] == "M")
            {
                input.Add(line1[1]);
                input.Add(line1[2]);
                input.Add(line1[3]);
                l.Create(ref input);

                l.ReturnLetter(ref Letter, ref number);
                rack.Update(Letter, number, input);

            }
            else if (line1[0] == "S")
            {
                if (line1[1] == "P1")
                {
                    p1.Update(int.Parse(line1[2]));
                }
                else if (line1[1] == "P2")
                {
                    p2.Update(int.Parse(line1[2]));
                }
            }
            else if (line1[0] == "SW")
            {
                for (int i = 0; i < int.Parse(line1[1]); i++)
                {
                    input.Add(line1[i + 2]);
                    input2.Add(line1[i + 2 + int.Parse(line1[1])]);
                }
                rack.Swap(int.Parse(line1[1]), input, input2);
                input.Clear();
                input2.Clear();
            }
        } while (!sr.EndOfStream);
        
    }

    // Update is called once per frame
    void Update()
    {

        
    }
}
