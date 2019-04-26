using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
public class Scores : MonoBehaviour
{
    public GameObject scoreboardrprefab;
    public GameObject scoreboardrprefab1;
    public GameObject scoreboardrprefab2;
    int turn = 1;
    int pl1score, pl2score;
    public Sprite[] Scoreboards;
    string Pl1name,Pl2name;
    string word1, word2;
    public Text P1name, P2name, P3name,P1Score, P2Score, P3Score, P1word, P2word, P3word;
    Vector3 pos1 = new Vector3(-6.5f,2,0);
    Vector3 pos2 = new Vector3(-6.5f, 0.8f, 0);
    Vector3 pos3 = new Vector3(-6.5f, -0.4f, 0);
    public int score;
    public void start(List<string> names)
    {
        pl1score = 0;
        pl2score = 0;
        Pl1name = names[0];
        Pl2name = names[1];
        P1name.text = " ";
        P2name.text = " ";
        P3name.text = " ";
        P1Score.text = " ";
        P2Score.text = " ";
        P3Score.text = " ";
        P1word.text = " ";
        P2word.text = " ";
        P3word.text = " ";
        scoreboardrprefab.SetActive(false);
        scoreboardrprefab1.SetActive(false);
        scoreboardrprefab2.SetActive(false);

        



    }
    public void update(int newscore,string word)
    {
        if (turn==1)
        {
            scoreboardrprefab.SetActive(true);
            P1name.text = Pl1name;
            P1Score.text = (pl1score + newscore).ToString();
            pl1score = pl1score + newscore;
            P1word.text = word;
            turn++;

        }
        else if (turn == 2)
        {
           
            scoreboardrprefab1.SetActive(true);
            P2name.text = Pl2name;
            P2Score.text = (pl2score + newscore).ToString();
            pl2score = pl2score + newscore;
            P2word.text = word;
            word1 = word;
            turn++;
        }
        else if (turn == 3)
        {
            scoreboardrprefab2.SetActive(true);
            P3name.text = Pl1name;
            P3Score.text = (pl1score + newscore).ToString();
            pl1score = pl1score + newscore;
            P3word.text = word;
            word2 = word;
            turn++;
        }
        else {
            if (turn % 2 == 0) {
                scoreboardrprefab.GetComponent<SpriteRenderer>().sprite = Scoreboards[1];
                scoreboardrprefab1.GetComponent<SpriteRenderer>().sprite = Scoreboards[0];
                scoreboardrprefab2.GetComponent<SpriteRenderer>().sprite = Scoreboards[1];


                
                P1name.text = Pl2name;
                P1Score.text = (pl2score).ToString();
                P1word.text = word1;


                P2name.text = Pl1name;
                P2Score.text = (pl1score).ToString();
                P2word.text = word2;
                word1 = word2;


             
                P3name.text = Pl2name;
                P3Score.text = (pl2score + newscore).ToString();
                pl2score = pl2score + newscore;
                P3word.text = word;
                word2 = word;
                turn++;

            }
            else {
                scoreboardrprefab.GetComponent<SpriteRenderer>().sprite = Scoreboards[0];
                scoreboardrprefab1.GetComponent<SpriteRenderer>().sprite = Scoreboards[1];
                scoreboardrprefab2.GetComponent<SpriteRenderer>().sprite = Scoreboards[0];


                P1name.text = Pl1name;
                P1Score.text = (pl1score).ToString();
                P1word.text = word1;


                P2name.text = Pl2name;
                P2Score.text = (pl2score).ToString();
                P2word.text = word2;
                word1 = word2;


                
                P3name.text = Pl1name;
                P3Score.text = (pl1score + newscore).ToString();
                pl1score = pl1score + newscore;
                P3word.text = word;
                word2 = word;
                turn++;
            }
        }
      
    }
}
