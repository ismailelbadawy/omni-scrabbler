using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;


public class Board : MonoBehaviour
{
    public GameObject mCellPrefab;
    [HideInInspector]
    public Cell[,] mAllCells = new Cell[15, 15];

    public void Create()
    {
        for (int x = 0; x < 15; x++)
        {
            for (int y = 0; y < 15; y++)
            {
                GameObject newCell = Instantiate(mCellPrefab, transform);

                RectTransform rectTransform = newCell.GetComponent<RectTransform>();
                rectTransform.anchoredPosition = new Vector2((x * 100) + 1000, (y * 100) + 125);

                mAllCells[x, y] = newCell.GetComponent<Cell>();
                mAllCells[x, y].Setup(new Vector2Int(x, y), this);
            }
        }

        //Dark Red
        mAllCells[0, 0].GetComponent<SpriteRenderer>().color = new Color32(243, 103, 73, 255);
        mAllCells[7, 0].GetComponent<SpriteRenderer>().color = new Color32(243, 103, 73, 255);
        mAllCells[14, 0].GetComponent<SpriteRenderer>().color = new Color32(243, 103, 73, 255);
        mAllCells[0, 7].GetComponent<SpriteRenderer>().color = new Color32(243, 103, 73, 255);
        mAllCells[14, 7].GetComponent<SpriteRenderer>().color = new Color32(243, 103, 73, 255);
        mAllCells[0, 14].GetComponent<SpriteRenderer>().color = new Color32(243, 103, 73, 255);
        mAllCells[7, 14].GetComponent<SpriteRenderer>().color = new Color32(243, 103, 73, 255);
        mAllCells[14, 14].GetComponent<SpriteRenderer>().color = new Color32(243, 103, 73, 255);

        //Light Blue
        mAllCells[3, 0].GetComponent<SpriteRenderer>().color = new Color32(194, 214, 211, 255);
        mAllCells[11, 0].GetComponent<SpriteRenderer>().color = new Color32(194, 214, 211, 255);
        mAllCells[6, 2].GetComponent<SpriteRenderer>().color = new Color32(194, 214, 211, 255);
        mAllCells[8, 2].GetComponent<SpriteRenderer>().color = new Color32(194, 214, 211, 255);
        mAllCells[0, 3].GetComponent<SpriteRenderer>().color = new Color32(194, 214, 211, 255);
        mAllCells[7, 3].GetComponent<SpriteRenderer>().color = new Color32(194, 214, 211, 255);
        mAllCells[14, 3].GetComponent<SpriteRenderer>().color = new Color32(194, 214, 211, 255);
        mAllCells[2, 6].GetComponent<SpriteRenderer>().color = new Color32(194, 214, 211, 255);
        mAllCells[6, 6].GetComponent<SpriteRenderer>().color = new Color32(194, 214, 211, 255);
        mAllCells[8, 6].GetComponent<SpriteRenderer>().color = new Color32(194, 214, 211, 255);
        mAllCells[12, 6].GetComponent<SpriteRenderer>().color = new Color32(194, 214, 211, 255);
        mAllCells[3, 7].GetComponent<SpriteRenderer>().color = new Color32(194, 214, 211, 255);
        mAllCells[11, 7].GetComponent<SpriteRenderer>().color = new Color32(194, 214, 211, 255);
        mAllCells[2, 8].GetComponent<SpriteRenderer>().color = new Color32(194, 214, 211, 255);
        mAllCells[6, 8].GetComponent<SpriteRenderer>().color = new Color32(194, 214, 211, 255);
        mAllCells[8, 8].GetComponent<SpriteRenderer>().color = new Color32(194, 214, 211, 255);
        mAllCells[12, 8].GetComponent<SpriteRenderer>().color = new Color32(194, 214, 211, 255);
        mAllCells[0, 11].GetComponent<SpriteRenderer>().color = new Color32(194, 214, 211, 255);
        mAllCells[7, 11].GetComponent<SpriteRenderer>().color = new Color32(194, 214, 211, 255);
        mAllCells[14, 11].GetComponent<SpriteRenderer>().color = new Color32(194, 214, 211, 255);
        mAllCells[6, 12].GetComponent<SpriteRenderer>().color = new Color32(194, 214, 211, 255);
        mAllCells[8, 12].GetComponent<SpriteRenderer>().color = new Color32(194, 214, 211, 255);
        mAllCells[3, 14].GetComponent<SpriteRenderer>().color = new Color32(194, 214, 211, 255);
        mAllCells[11, 14].GetComponent<SpriteRenderer>().color = new Color32(194, 214, 211, 255);

        //Dark Blue
        mAllCells[5, 1].GetComponent<SpriteRenderer>().color = new Color32(64, 164, 180, 255);
        mAllCells[9, 1].GetComponent<SpriteRenderer>().color = new Color32(64, 164, 180, 255);
        mAllCells[1, 5].GetComponent<SpriteRenderer>().color = new Color32(64, 164, 180, 255);
        mAllCells[5, 5].GetComponent<SpriteRenderer>().color = new Color32(64, 164, 180, 255);
        mAllCells[9, 5].GetComponent<SpriteRenderer>().color = new Color32(64, 164, 180, 255);
        mAllCells[13, 5].GetComponent<SpriteRenderer>().color = new Color32(64, 164, 180, 255);
        mAllCells[1, 9].GetComponent<SpriteRenderer>().color = new Color32(64, 164, 180, 255);
        mAllCells[5, 9].GetComponent<SpriteRenderer>().color = new Color32(64, 164, 180, 255);
        mAllCells[9, 9].GetComponent<SpriteRenderer>().color = new Color32(64, 164, 180, 255);
        mAllCells[13, 9].GetComponent<SpriteRenderer>().color = new Color32(64, 164, 180, 255);
        mAllCells[5, 13].GetComponent<SpriteRenderer>().color = new Color32(64, 164, 180, 255);
        mAllCells[9, 13].GetComponent<SpriteRenderer>().color = new Color32(64, 164, 180, 255);

        //Light Red
        mAllCells[1, 1].GetComponent<SpriteRenderer>().color = new Color32(255, 193, 176, 255);
        mAllCells[13, 1].GetComponent<SpriteRenderer>().color = new Color32(255, 193, 176, 255);
        mAllCells[2, 2].GetComponent<SpriteRenderer>().color = new Color32(255, 193, 176, 255);
        mAllCells[12, 2].GetComponent<SpriteRenderer>().color = new Color32(255, 193, 176, 255);
        mAllCells[3, 3].GetComponent<SpriteRenderer>().color = new Color32(255, 193, 176, 255);
        mAllCells[11, 3].GetComponent<SpriteRenderer>().color = new Color32(255, 193, 176, 255);
        mAllCells[4, 4].GetComponent<SpriteRenderer>().color = new Color32(255, 193, 176, 255);
        mAllCells[10, 4].GetComponent<SpriteRenderer>().color = new Color32(255, 193, 176, 255);
        mAllCells[7, 7].GetComponent<SpriteRenderer>().color = new Color32(255, 193, 176, 255);
        mAllCells[4, 10].GetComponent<SpriteRenderer>().color = new Color32(255, 193, 176, 255);
        mAllCells[10, 10].GetComponent<SpriteRenderer>().color = new Color32(255, 193, 176, 255);
        mAllCells[3, 11].GetComponent<SpriteRenderer>().color = new Color32(255, 193, 176, 255);
        mAllCells[11, 11].GetComponent<SpriteRenderer>().color = new Color32(255, 193, 176, 255);
        mAllCells[2, 12].GetComponent<SpriteRenderer>().color = new Color32(255, 193, 176, 255);
        mAllCells[12, 12].GetComponent<SpriteRenderer>().color = new Color32(255, 193, 176, 255);
        mAllCells[1, 13].GetComponent<SpriteRenderer>().color = new Color32(255, 193, 176, 255);
        mAllCells[13, 13].GetComponent<SpriteRenderer>().color = new Color32(255, 193, 176, 255);


        mAllCells[7, 7].GetComponent<SpriteRenderer>();

    }

}