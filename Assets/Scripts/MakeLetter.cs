using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO;

public class MakeLetter : MonoBehaviour
{
    public GameObject LetterPrefab;
    public Sprite[] LetterSprite;
    float row = 0;
    float coloumn = 0;
    int arrayIdx = 0;
    Vector3 pos = new Vector3(0, 0, 0);
   
    // Start is called before the first frame update
    public void Create(ref List <string> input)
    {
        arrayIdx = ReturnIndx(input[0]);
        pos = GetPos(input[1], input[2]);
        Sprite nLetter = LetterSprite[arrayIdx];
        GameObject newLetter = Instantiate(LetterPrefab);
        newLetter.GetComponent<SpriteRenderer>().sprite = nLetter;
        newLetter.transform.position = pos;
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
            row = ((int.Parse(a)) - 7) * 0.65f ;
            
        }
        else if (int.Parse(a) < 7)
        {
            row = (7-(int.Parse(a))) * 0.65f;
            row = - row;
        }

        if (int.Parse(b) == 0)
        {
            coloumn = 0;
        }
        else if (int.Parse(b) > 0)
        {
            coloumn = (int.Parse(b))*0.65f;
        }
        return pos = new Vector3(coloumn, row, 0);
    }
    
    public void ReturnLetter(ref string Letter, ref int type)
    {
        Letter = LetterSprite[arrayIdx].name;
        type = 2;
    }

}
