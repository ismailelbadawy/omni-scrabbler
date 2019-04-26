using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class Cell : MonoBehaviour
{
    [HideInInspector]
    public Vector2Int mBoardPosition = Vector2Int.zero;
    [HideInInspector]
    public Board mBoard = null;
    [HideInInspector]
    public RectTransform mRectTransform = null;

    public void Setup(Vector2Int newBoardPos, Board newBoard)
    {
        mBoardPosition = newBoardPos;
        mBoard = newBoard;
        mRectTransform = GetComponent<RectTransform>();
    }
}