using UnityEngine;
using UnityEngine.UI;
using System.Collections.Generic;

public class PowerUpUI : MonoBehaviour
{
    public List<powerUpUIObject> uiObjects = new List<powerUpUIObject>();
    public Transform parent;
    public GameObject prefab;
    List<powerUpUIObject> toAdd = new List<powerUpUIObject>();
    List<powerUpUIObject> toRemove = new List<powerUpUIObject>();

    public class powerUpUIObject{
        public GameObject powerUpUI;
        public Sprite sprite;
        public float effectDuration;
        public float timeRemaining;
        public Image backgroundImage;
        public bool active = false;

        public powerUpUIObject(Sprite sprite, float effectDuration, GameObject prefab, Transform parent){
            this.sprite = sprite;
            this.effectDuration = effectDuration;
            timeRemaining = effectDuration;
            
            this.powerUpUI = Instantiate(prefab, parent);
            this.backgroundImage = powerUpUI.GetComponent<Image>();
            GameObject child = powerUpUI.transform.Find("Icon").gameObject;
            Image child_i = child.GetComponent<Image>();
            child_i.sprite = sprite;
            active = true;
        }

    }


    void Update(){

        foreach(powerUpUIObject uiObject in uiObjects){
            //Debug.Log(uiObject);
            if(uiObject.active != false){
                if(uiObject.timeRemaining <= 0){
                    toRemove.Add(uiObject);
                }else{
                    uiObject.timeRemaining -= Time.deltaTime;
                    uiObject.backgroundImage.fillAmount = (uiObject.timeRemaining/uiObject.effectDuration);
                    //Debug.Log($"Fill Amount for {uiObject} is: {uiObject.backgroundImage.fillAmount}");
                }
            }
        }

        uiObjects.AddRange(toAdd);
        toAdd.Clear();

        foreach(powerUpUIObject uiObject in toRemove){
            Destroy(uiObject.powerUpUI);
            uiObjects.Remove(uiObject);
        }
        toRemove.Clear();

        
    }

    public void AddPowerUpUI(PowerUp powerUp){
        powerUpUIObject newObject = new powerUpUIObject(powerUp.sprite, powerUp.effectDuration, prefab, parent);
        toAdd.Add(newObject);
    }

}
