using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;
using TMPro;
using UnityEngine.UI;
using UnityEngine.EventSystems;

public class UpgradeManager : MonoBehaviour
{

    [Serializable]
    public class WeaponUpgrade{
            public string name;

            public int[] fireRateBuyPrices;
            public int[] damageBuyPrices;
            public int[] bulletSpeedBuyPrices;
            public int[] criticalHitBuyPrices;

            public float[] fireRateValue;
            public int[] damageValue;
            public int[] bulletValue;
            public float[] criticalValue;

            public int[] xpMaxValues;

            public int xpValue;

            public int fireRateLevel;
            public int damageLevel;
            public int bulletLevel;
            public int criticalLevel;
            public int xpLevel;

            public TMP_Text fireRateText;
            public TMP_Text damageText;
            public TMP_Text bulletSpeedText;
            public TMP_Text criticalHitText;
            public TMP_Text xpText;

            public Image xpBar;

            public void SaveValues()
            {
                PlayerPrefs.SetInt($"{name}_frLevel", fireRateLevel);
                PlayerPrefs.SetInt($"{name}_dLevel", damageLevel);
                PlayerPrefs.SetInt($"{name}_bsLevel", bulletLevel);
                PlayerPrefs.SetInt($"{name}_chLevel", criticalLevel);
                PlayerPrefs.SetInt($"{name}_xpLevel", xpLevel);
                PlayerPrefs.SetInt($"{name}_xpValue", xpValue);
            }

            public void LoadValues(){
                if(!PlayerPrefs.HasKey($"{name}_frLevel")){
                    fireRateLevel = 0;
                    damageLevel = 0;
                    bulletLevel = 0;
                    criticalLevel = 0;
                    xpLevel = 0;
                    xpValue = 0;
                }else{
                    fireRateLevel = PlayerPrefs.GetInt($"{name}_frLevel");
                    damageLevel = PlayerPrefs.GetInt($"{name}_dLevel");
                    bulletLevel = PlayerPrefs.GetInt($"{name}_bsLevel");
                    criticalLevel = PlayerPrefs.GetInt($"{name}_chLevel");
                    xpLevel = PlayerPrefs.GetInt($"{name}_xpLevel");
                    xpValue = PlayerPrefs.GetInt($"{name}_xpValue");
                }
            }

    }



    [SerializeField]
    public WeaponUpgrade PISTOL;

    [SerializeField]
    public WeaponUpgrade SMG;

    [SerializeField]
    public WeaponUpgrade M4;

    public WeaponUpgrade[] weaponUpgrades => new WeaponUpgrade[]{PISTOL, SMG, M4};

    public Image fireRateBar;
    public Image damageBar;
    public Image bulletSpeedBar;
    public Image criticalHitBar;

    public TMP_Text gemText;
    public TMP_Text coinText;

    private Vector2 screenCenter;
    public GraphicRaycaster uiRaycaster;
    public EventSystem eventSystem;

    public ScrollRect weaponScrollRect;
    public RectTransform weaponContent;
    



void Start(){

    PlayerStats.LoadPlayerValues();

    screenCenter = new Vector2(Screen.width / 2, Screen.height / 1.5f);
    PlayerStats.coins = PlayerPrefs.GetInt("Coins");
    PlayerStats.gems = PlayerPrefs.GetInt("Gems");
    foreach(WeaponUpgrade wu in weaponUpgrades){
        WeaponController.InitializeWeapon();
        wu.LoadValues();
        fillBars(wu);
        SetTexts(wu);
    }

    ScrollToWeapon();

    switch (WeaponController.weaponNumber){
        case 0:
            AddXp(PISTOL);
            fillBars(PISTOL);
            SetTexts(PISTOL);
            break;
        case 1:
            AddXp(SMG);
            fillBars(SMG);
            SetTexts(SMG);
            break;
        case 2:
            AddXp(M4);
            fillBars(M4);
            SetTexts(M4);
            break;
        default:
            break;
    }

}

void Update(){
        // Calculate the position 1/3 of the way from the top of the screen
        Vector2 customPointerPosition = new Vector2(
            Screen.width / 2,       // Center of the screen horizontally
            Screen.height * (2f / 3f) // 1/3 down from the top (2/3 of the height)
        );

        // Create PointerEventData
        PointerEventData pointerData = new PointerEventData(eventSystem);
        pointerData.position = customPointerPosition;

        List<RaycastResult> results = new List<RaycastResult>();
        uiRaycaster.Raycast(pointerData, results);

        if(results.Count > 0){
            foreach(RaycastResult result in results){

                //Debug.Log(result);

                switch (result.gameObject.tag)
                {
                    case "Pistol":
                        WeaponController.weaponNumber = 0;
                        PISTOL.LoadValues();
                        UpdateTextBars();
                        fillBars(PISTOL);
                        break;
                    case "Smg":
                        WeaponController.weaponNumber = 1;
                        SMG.LoadValues();
                        UpdateTextBars();
                        fillBars(SMG);
                        break;
                    case "M4":
                        WeaponController.weaponNumber = 2;
                        M4.LoadValues();
                        UpdateTextBars();
                        fillBars(M4);
                        break;
                    default:
                        break;

                }
            }
        }
        //Debug.Log(PlayerStats.coins);
    }

public void UpdateTextBars(){
    switch(WeaponController.weaponNumber){
        case 0:
            SetTexts(PISTOL);
            fillBars(PISTOL);
            break;
        case 1:
            SetTexts(SMG);
            fillBars(SMG);
            break;
        case 2:
            SetTexts(M4);
            fillBars(M4);
            break;
        default:
            break;
    }
}

void SetTexts(WeaponUpgrade wu){
    wu.fireRateText.text = wu.fireRateBuyPrices[wu.fireRateLevel].ToString();
    wu.damageText.text = wu.damageBuyPrices[wu.damageLevel].ToString();
    wu.bulletSpeedText.text = wu.bulletSpeedBuyPrices[wu.bulletLevel].ToString();
    wu.criticalHitText.text = wu.criticalHitBuyPrices[wu.criticalLevel].ToString();

    coinText.text = PlayerStats.coins.ToString();
    gemText.text = PlayerStats.gems.ToString();

    wu.xpText.text = $"{wu.xpValue}/{wu.xpMaxValues[wu.xpLevel]}";
}


public void BuyFireRate(){
    switch(WeaponController.weaponNumber){
        case 0:
            FireRateLogic(PISTOL);
            break;
        case 1:
            FireRateLogic(SMG);
            break;
        case 2:
            FireRateLogic(M4);
            break;
        default:
            break;
    }
}

public void BuyDamage(){
    switch(WeaponController.weaponNumber){
        case 0:
            DamageLogic(PISTOL);
            break;
        case 1:
            DamageLogic(SMG);
            break;
        case 2:
            DamageLogic(M4);
            break;
        default:
            break;
    }
}

public void BuyBulletSpeed(){
        switch (WeaponController.weaponNumber)
        {
            case 0:
                BulletSpeedLogic(PISTOL);
                break;
            case 1:
                BulletSpeedLogic(SMG);
                break;
            case 2:
                BulletSpeedLogic(M4);
                break;
            default:
                break;
        }
    }

public void BuyCriticalHit(){
        switch (WeaponController.weaponNumber)
        {
            case 0:
                CriticalHitLogic(PISTOL);
                break;
            case 1:
                CriticalHitLogic(SMG);
                break;
            case 2:
                CriticalHitLogic(M4);
                break;
            default:
                break;
        }
    }

private void SaveWeapon(){
    switch(WeaponController.weaponNumber){
        case 0:
            SaveWeaponLogic(PISTOL);
            break;
        case 1:
            SaveWeaponLogic(SMG);
            break;
        case 2:
            SaveWeaponLogic(M4);
            break;
        default:
            break;
    }
}

void SaveWeaponLogic(WeaponUpgrade wu){
    WeaponController.UpdateWeapon(wu.fireRateValue[wu.fireRateLevel], wu.damageValue[wu.damageLevel], wu.bulletValue[wu.bulletLevel], wu.criticalValue[wu.criticalLevel], wu.xpValue);
}

    public void fillBars(WeaponUpgrade wu){

    fireRateBar.fillAmount = (float)wu.fireRateLevel/(float)wu.fireRateBuyPrices.Length;
    damageBar.fillAmount = (float)wu.damageLevel/ (float)wu.damageBuyPrices.Length;
    bulletSpeedBar.fillAmount = (float)wu.bulletLevel/ (float)wu.bulletSpeedBuyPrices.Length;
    criticalHitBar.fillAmount = (float)wu.criticalLevel/ (float)wu.criticalHitBuyPrices.Length;

    wu.xpBar.fillAmount = (float)wu.xpValue/(float)wu.xpMaxValues[wu.xpLevel];
    }

    public void AddXp(WeaponUpgrade wu)
    {
        if (EnemyController.enemyKills != 0)
        {
            wu.xpValue += PlayerStats.totalXpToGive;
            PlayerStats.totalXpToGive = 0;
            
            if (wu.xpValue >= wu.xpMaxValues[wu.xpLevel])
            {
                wu.xpValue -= wu.xpMaxValues[wu.xpLevel];
                wu.xpLevel++;
            }
            wu.SaveValues();
        }
    }

    void ScrollToWeapon(){
        float normalizedPosition = (float)WeaponController.weaponNumber / (weaponUpgrades.Length - 1);
        weaponScrollRect.normalizedPosition = new Vector2(normalizedPosition, 0f);
    }
    

    void FireRateLogic(WeaponUpgrade wu){
        if (PlayerStats.coins >= wu.fireRateBuyPrices[wu.fireRateLevel])
        {
            PlayerStats.coins -= wu.fireRateBuyPrices[wu.fireRateLevel];
            PlayerPrefs.SetInt("Coins", PlayerStats.coins);
            wu.fireRateLevel++;
            SaveWeapon();
            wu.SaveValues();
        }
    }

    void DamageLogic(WeaponUpgrade wu){
        if (PlayerStats.coins >= wu.damageBuyPrices[wu.damageLevel])
        {
            PlayerStats.coins -= wu.damageBuyPrices[wu.damageLevel];
            PlayerPrefs.SetInt("Coins", PlayerStats.coins);
            wu.damageLevel++;
            SaveWeapon();
            wu.SaveValues();
        }
    }

    void BulletSpeedLogic(WeaponUpgrade wu){
        if (PlayerStats.coins >= wu.bulletSpeedBuyPrices[wu.bulletLevel])
        {
            PlayerStats.coins -= wu.bulletSpeedBuyPrices[wu.bulletLevel];
            PlayerPrefs.SetInt("Coins", PlayerStats.coins);
            wu.bulletLevel++;
            SaveWeapon();
            wu.SaveValues();
        }
    }

    void CriticalHitLogic(WeaponUpgrade wu){
        if (PlayerStats.coins >= wu.criticalHitBuyPrices[wu.criticalLevel])
        {
            PlayerStats.coins -= wu.criticalHitBuyPrices[wu.criticalLevel];
            PlayerPrefs.SetInt("Coins", PlayerStats.coins);
            wu.criticalLevel++;
            SaveWeapon();
            wu.SaveValues();
        }
    }
}

