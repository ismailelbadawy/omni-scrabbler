using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO;

public class Rack : MonoBehaviour
{
    public GameObject Letterprefab;
    public Sprite[] LetterSprite;
    public List<GameObject> LetterList = new List<GameObject>();
    public List<string> RackList = new List<string>();
    public List<int> RorB = new List<int>(); //the letter on borad(0) or on rack(1)
    int[] EmptyPlaces = new int[7];
    Vector3 pos = new Vector3(0, 0, 0);
    int arrayIdx = 0;
    int x;
    float row = -4.25f;
    float coloumn = -8.9f;
    public void Create(ref List <string> input)
    {
      
        for (int i = 0; i < 7; i++)
        {
            for (int j = 0; j < 26; j++)
            {
                if (LetterSprite[j].name == input[i])
                {
                     arrayIdx = j;

                }
            }
            EmptyPlaces[i] = 1;
            pos = new Vector3(coloumn + (i * 0.8f), row, -2);
            Sprite nLetter = LetterSprite[arrayIdx];
            GameObject newLetter = Instantiate(Letterprefab);
            newLetter.GetComponent<SpriteRenderer>().sprite = nLetter;
            newLetter.transform.position = pos;
            newLetter.transform.localScale = new Vector3(1, 1, 0);
            LetterList.Add(newLetter);
            RackList.Add(input[i]);
            RorB.Add(1);
        }
        input.Clear();
    }
    int ReturnIndx(string letter)
    {
        for (int i = 0; i < 26; i++)
        {
            if (letter == LetterSprite[i].name)
            {
                arrayIdx = i;
                break;
            }
        }
        return arrayIdx;
    }
    Vector3 GetPos(string a, string b)
    {
        if (int.Parse(a) == 7)
        {
            row = 0;
        }
        else if (int.Parse(a) > 7)
        {
            row = ((int.Parse(a)) - 7) * 0.65f;

        }
        else if (int.Parse(a) < 7)
        {
            row = (7 - (int.Parse(a))) * 0.65f;
            row = -row;
        }

        if (int.Parse(b) == 0)
        {
            coloumn = 0;
        }
        else if (int.Parse(b) > 0)
        {
            coloumn = (int.Parse(b)) * 0.65f;
        }
        return pos = new Vector3(coloumn, row, 0);
    }

    public void update(int type, ref List<string> input) //1:to add a letter //2:to remove a letter
    {
        if (type == 2)
        {
            arrayIdx = ReturnIndx(input[0]);
            pos = GetPos(input[1], input[2]);

            for (int i = 0; i < LetterList.Count; i++)
            {
                if (LetterList[i].GetComponent<SpriteRenderer>().sprite.name == input[0])
                {
                    LetterList[i].transform.localScale = new Vector3(0.8f, 0.8f, 0);
                    LetterList[i].GetComponent<SpriteRenderer>().transform.position = pos;
                    EmptyPlaces[i] = 0;
                    RackList[i] = " ";
                    RorB[i] = 0;
                    break;
                }
            }
            input.Clear(); 
        }
        else if (type==1)
        {
             row = -4.25f;
             coloumn = -8.9f;

            for (int i=0;i<input.Count;i++)
            {
                for (int j = 0; j < 7; j++)
                {
                    if (EmptyPlaces[j]==0)
                    {
                        pos = new Vector3(coloumn + (j * 0.8f), row, -2);
                        EmptyPlaces[j] = 1;
                        x = j;
                        break;
                    }
                }
                for (int k = 0; k < 26; k++)
                {
                    if (LetterSprite[k].name == input[i])
                    {
                        arrayIdx = k;

                    }
                }
                Sprite nLetter = LetterSprite[arrayIdx];
                GameObject newLetter = Instantiate(Letterprefab);
                newLetter.GetComponent<SpriteRenderer>().sprite = nLetter;
                newLetter.transform.position = pos;
                newLetter.transform.localScale = new Vector3(1, 1, 0);
                LetterList.Add(newLetter);
                RackList[x] = input[i];
                RorB.Add(1);
            }
        }
      
    }
    public void Swab(int numberofletters, List <string>old,List <string> newl)
    {
        for (int j=0;j<numberofletters;j++)
        {
            for (int i = 0; i < LetterList.Count; i++)
            {
                if (LetterList[i].GetComponent<SpriteRenderer>().sprite.name == old[j] && RorB[i]==1)
                {
                    x = i;
                    break;
                }
            }
            for (int k = 0; k < 7; k++)
            {
                if (RackList[k]== old[j])
                {
                    RackList[k] = newl[j];
                }
            }
            for (int k = 0; k < 26; k++)
            {
                if (LetterSprite[k].name == newl[j])
                {
                    arrayIdx = k;

                }
            }

            Sprite nLetter = LetterSprite[arrayIdx];
            LetterList[x].GetComponent<SpriteRenderer>().sprite = nLetter;
        }

    }
        
}
