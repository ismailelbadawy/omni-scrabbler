using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO;

public class Rack : MonoBehaviour
{
    public GameObject LetterPrefab;
    public Sprite[] LetterSprite;
    public List<GameObject> RackList = new List<GameObject>();
    int[] EmptyPlaces = new int[7];
    Vector3 pos = new Vector3(0, 0, 0);
    int arrayIdx = 0;
    int x;
    float row = -3.25f;
    float coloumn = -8.45f;

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
            pos = new Vector3(coloumn + (i * 0.65f), row, -2);
            Sprite nLetter = LetterSprite[arrayIdx];
            GameObject newLetter = Instantiate(LetterPrefab);
            newLetter.GetComponent<SpriteRenderer>().sprite = nLetter;
            newLetter.transform.position = pos;
            newLetter.transform.localScale = new Vector3(1, 1, 0);
            RackList.Add(newLetter);
        }
        input.Clear();
    }

    public void Update(string Letter, int type, List<string> input) //1:to add a letter //2:to remove a letter
    {
        if (type == 2)
        {
            for (int i = 0; i < RackList.Count; i++)
            {
                if (RackList[i].GetComponent<SpriteRenderer>().sprite.name == Letter)
                {
                    RackList[i].GetComponent<SpriteRenderer>().enabled = false;
                    RackList[i].GetComponent<SpriteRenderer>().gameObject.SetActive(false);
                    EmptyPlaces[i] = 0;
                    break;
                }
            }
        }
        else if (type==1)
        {
            for (int i=0; i<input.Count; i++)
            {
                for (int j = 0; j < 7; j++)
                {
                    if (EmptyPlaces[j]==0)
                    {
                        pos = new Vector3(coloumn + (j * 0.65f), row, -2);
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
                GameObject newLetter = Instantiate(LetterPrefab);
                newLetter.GetComponent<SpriteRenderer>().sprite = nLetter;
                newLetter.transform.position = pos;
                newLetter.transform.localScale = new Vector3(1, 1, 0);
                RackList[x] = newLetter;
            }
        }
      
    }

    public void Swap(int numberofletters, List <string>old,List <string> newl)
    {
        for (int j=0; j<numberofletters; j++)
        {
            for (int i = 0; i < RackList.Count; i++)
            {
                if (RackList[i].GetComponent<SpriteRenderer>().sprite.name == old[j])
                {
                    RackList[i].GetComponent<SpriteRenderer>().enabled = false;
                    RackList[i].GetComponent<SpriteRenderer>().gameObject.SetActive(false);
                    EmptyPlaces[i] = 0;
                    pos = new Vector3(coloumn + (i * 0.65f), row, -2);
                    EmptyPlaces[i] = 1;
                    x = i;
                    break;
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
            GameObject newLetter = Instantiate(LetterPrefab);
            newLetter.GetComponent<SpriteRenderer>().sprite = nLetter;
            newLetter.transform.position = pos;
            newLetter.transform.localScale = new Vector3(1, 1, 0);
            RackList[x] = newLetter;
        }

    }
        
}
