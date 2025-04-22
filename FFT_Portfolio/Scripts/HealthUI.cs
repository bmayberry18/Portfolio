using UnityEngine;
using UnityEngine.UI;
using System.Collections;
using System.Collections.Generic;

public class HealthUI : MonoBehaviour
{

    public GameObject healthImagePrefab;
    public Transform healthParent;

    private List<GameObject> healthImages = new List<GameObject>();
    

    // Start is called once before the first execution of Update after the MonoBehaviour is created
    void Start()
    {

        UpdateHealthUI();
    }

    // Update is called once per frame
    void Update()
    {
        UpdateHealthUI();
    }

    void UpdateHealthUI(){
        int len = healthImages.Count;
        if (len != PlayerStats.health){
            if(len > PlayerStats.health){
                RemoveHealthImage();
            }else if(len < PlayerStats.health){
                NewHealthImage();
            }else{
                return;
            }
        }else{
            return;
        }
    }

    void NewHealthImage(){
        GameObject healthImage = Instantiate(healthImagePrefab, healthParent);
        healthImages.Add(healthImage);
    }

    void RemoveHealthImage(){
        GameObject image = healthImages[0];
        healthImages.Remove(image);
        Destroy(image);
    }
}
