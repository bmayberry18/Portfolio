using UnityEngine;
using UnityEngine.UI;
using TMPro;
using System.Collections;

public class PanelExpandAnimation : MonoBehaviour
{

    public RectTransform panel;
    public float duration;
    public float targetHeight;
    public RectTransform triangle;

    private float originalHeight;
    private float triangleOriginalZ;
    private bool isExpanded = false;
    private bool running = false;

    public int UpdateID = 0;
    public TMP_Text panelText;

    public UpgradeManager um;

    // Start is called once before the first execution of Update after the MonoBehaviour is created
    void Start()
    {
        originalHeight = panel.sizeDelta.y;
        triangleOriginalZ = triangle.eulerAngles.z;
        panel.pivot = new Vector2(panel.pivot.x,1f);
        //triangle.pivot = new Vector2(panel.pivot.x, 0f);
        //PanelExpand();
    }

    // Update is called once per frame
    void Update()
    {
        int num = WeaponController.weaponNumber;
        UpgradeManager.WeaponUpgrade wu = SetWeapon(num);


        if (panelText != null)
        {
            panelText.text = SetText(wu);
        }
    }


    public void PanelExpand(){

        if(running != false) return;
        if(isExpanded == false){
            StartCoroutine(ExpandCoroutine(targetHeight, duration, 0f));
        }else{
            StartCoroutine(ExpandCoroutine(originalHeight, duration, 180f));
        }

    }

    IEnumerator ExpandCoroutine(float targetHeight, float duration, float triangleRotation){
        running = true;
        float time = 0;
        float startHeight = panel.sizeDelta.y;
        string text;

        if(isExpanded == true){
            panelText.gameObject.SetActive(false);
        }
        
        while(time < duration){
            time += Time.deltaTime;
            float newHeight = Mathf.Lerp(startHeight, targetHeight, time/duration);
            float newRotation = Mathf.Lerp(triangle.eulerAngles.z, triangleRotation, time/(duration/2));
            panel.sizeDelta = new Vector2(panel.sizeDelta.x, newHeight);
            triangle.eulerAngles = new Vector3(0, 0, newRotation);
            yield return null;
        }

        if(isExpanded == false){
            panelText.gameObject.SetActive(true);
        }

        panel.sizeDelta = new Vector2(panel.sizeDelta.x, targetHeight);
        isExpanded = !isExpanded;
        running = false;
    }

    public string SetText(UpgradeManager.WeaponUpgrade wu){
        string text;
        switch (UpdateID)
        {
            case 1:
                text = $"{wu.fireRateValue[wu.fireRateLevel]} -> {wu.fireRateValue[wu.fireRateLevel + 1]}";
                break;
            case 2:
                text = $"{wu.damageValue[wu.damageLevel]} -> {wu.damageValue[wu.damageLevel + 1]}";
                break;
            case 3:
                text = $"{wu.bulletValue[wu.bulletLevel]} -> {wu.bulletValue[wu.bulletLevel + 1]}";
                break;
            case 4:
                text = $"{wu.criticalValue[wu.criticalLevel]} -> {wu.criticalValue[wu.criticalLevel + 1]}";
                break;
            default:
                text = "";
                break;
        }
        return text;
    }

    public UpgradeManager.WeaponUpgrade SetWeapon(int num){
        UpgradeManager.WeaponUpgrade wu = null;
        switch (num)
        {
            case 0:
                wu = um.PISTOL;
                break;
            case 1:
                wu = um.SMG;
                break;
            case 2:
                wu = um.M4;
                break;
            default:
                wu = null;
                break;
        }
        return wu;
    }

}
