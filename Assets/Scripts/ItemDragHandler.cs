using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.EventSystems;

public class ItemDragHandler : MonoBehaviour
{ 
    Vector3 dist;
    float posX;
    float posY;

    void OnMouseDown()
    {
        dist = Camera.main.WorldToScreenPoint(transform.position);
        posX = Input.mousePosition.x - dist.x;
        posY = Input.mousePosition.y - dist.y;
        transform.localScale = new Vector3(0.8f, 0.8f, 0);

    }

    void OnMouseDrag()
    {
        Vector3 curPos = new Vector3(Input.mousePosition.x - posX,
                  Input.mousePosition.y - posY, dist.z);

        Vector3 worldPos = Camera.main.ScreenToWorldPoint(curPos);
        transform.position = worldPos;
         transform.localScale = new Vector3(0.8f, 0.8f, 0);
        
    }
}