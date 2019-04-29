using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO;
using System.Threading;
using System.Text;
using System;
public class GameManager : MonoBehaviour
{
    public Scores score;
    public RackHM rack1;
    public Rack rack;
    int frame;
    public Client client=new Client();
   // public GUIInterface guiinterface=new GUIInterface();
    public MakeLetter l;
    public Timer timer;
    string Letter;
    int number;
    List<string> input = new List<string>();
    List<string> input2 = new List<string>();
    string[] line1;
    StreamReader sr;
    int counter;
    void Start()
    {
       
        client.run();
       // timer.Create();
      // if (GUIInterface.Messages.Count>0){
        //    string message=GUIInterface.Messages[0];
          //  GUIInterface.Messages.RemoveAt(0);
            //GUIInterface.InterpretMessage(message);
    //     sr = new StreamReader("Game1.txt");
    //     line1 = sr.ReadLine().Split();
        // input.Add(GUIInterface.GetPname());
        //  input.Add(GUIInterface.GetOname());
        //  score.start(input);
        //  input.Clear();
        frame = 0;
         counter=0; 
    }

    // Update is called once per frame
    void Update()
    {
        if (/*!sr.EndOfStream &&*/ frame % 300000 == 0 && GUIInterface.Messages.Count>0)
        {
            string message=GUIInterface.Messages[0];
            GUIInterface.Messages.RemoveAt(0);
            GUIInterface.InterpretMessage(message);
            // input.Add(GUIInterface.GetPname());
            // input.Add(GUIInterface.GetOname());
            // score.start(input);
            // input.Clear(); 
        if (frame==0 && counter==0){
         input.Add(GUIInterface.GetPname());
         input.Add(GUIInterface.GetOname());
         score.start(input);
         input.Clear();
         counter=1;
        }
            List<string> newRack=GUIInterface.GetRack();
            // for (int i=0;i<newRack.Count;i++){
            //     Console.WriteLine(newRack[i]);
            // }
            if (newRack.Count>0)
            rack.Create(ref newRack);
            List<List<string>> letters=GUIInterface.GetLetters();
            if (letters.Count>0){
                foreach (List<string> letter in letters){
                List<string> newletter=letter;
                if (newletter.Count!=0) 
                l.Create(ref newletter);
                } 
            }
       score.update(int.Parse(GUIInterface.GetPscore()), "dog");
      // score.update(int.Parse(GUIInterface.GetOscore()), "dog");
             //newRack=GUIInterface.GetRack();
             //rack.update(2, ref newRack);
            
        //     line1 = sr.ReadLine().Split();
        //     if (line1[0] == "R")
        //     {
        //         if (line1[1] == "CREATE")
        //         {
        //             input.Add(line1[2]);
        //             input.Add(line1[3]);
        //             input.Add(line1[4]);
        //             input.Add(line1[5]);
        //             input.Add(line1[6]);
        //             input.Add(line1[7]);
        //             input.Add(line1[8]);
        //             rack.Create(ref input);
        //         }
        //         else if (line1[1] == "ADD")
        //         {
        //             for (int i = 2; i < line1.Length; i++)
        //             {
        //                 input.Add(line1[i]);

        //             }
        //             rack.update(1, ref input);

        //             input.Clear();

        //         }
        //     }
        //     else if (line1[0] == "M")
        //     {
        //         if (line1[1] == "P2")
        //         {
        //             input.Add(line1[2]);
        //             input.Add(line1[3]);
        //             input.Add(line1[4]);
        //             l.Create(ref input);
        //         }

        //         else if (line1[1] == "P1")
        //         {
        //             input.Add(line1[2]);
        //             input.Add(line1[3]);
        //             input.Add(line1[4]);
        //             rack.update(2, ref input);
        //         }

        //         input.Clear();
        //     }
        //     else if (line1[0] == "S")
        //     {
        //         if (line1[1] == "P1")
        //         {
        //             score.update(int.Parse(line1[2]), "dog");
        //         }
        //         else if (line1[1] == "P2")
        //         {
        //             score.update(int.Parse(line1[2]), "dog");
        //         }
        //     }
        //     else if (line1[0] == "SW")
        //     {
        //         for (int i = 0; i < int.Parse(line1[1]); i++)
        //         {
        //             input.Add(line1[i + 2]);
        //             input2.Add(line1[i + 2 + int.Parse(line1[1])]);
        //         }
        //         rack.Swab(int.Parse(line1[1]), input, input2);
        //         input.Clear();
        //         input2.Clear();
        //     }
         }
        else if (frame % 300000 != 0) frame++;
       // else if (sr.EndOfStream) sr.Close();

    }
}
