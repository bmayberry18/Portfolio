using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using TMPro;

public class PlayerStats : MonoBehaviour
{

    public static int coins;
    public static int gems;
    public static int xp;

    public static int instantKillDurationLevel, shieldDurationLevel, doubleFireRateDurationLevel, nukeDurationLevel, multishotDurationLevel, freezeDurationLevel;
    //0 is false and 1 is true
    public static int smgLocked, m4Locked;
    public static int wavesCompleted, enemiesKilled, coinsCollected, gemsCollected, xpCollected;

    public static int totalXpToGive;

    public static int health;
    public static int maxHealth = 3;

    public TMP_Text coinText;
    public TMP_Text gemText;


    // Start is called before the first frame update
    void Start()
    {
        //PlayerPrefs.DeleteAll();
        LoadPlayerValues();
        health = maxHealth;
    }

    public static void SetHealthToMax(){
        PlayerStats.health = PlayerStats.maxHealth;
    }

    public static void LoadPlayerValues(){
        if (PlayerPrefs.HasKey("Coins"))
        {
            PlayerStats.coins = PlayerPrefs.GetInt("Coins");
        }
        else
        {
            PlayerPrefs.SetInt("Coins", 0);
            PlayerStats.coins = 0;
        }

        if (PlayerPrefs.HasKey("Gems"))
        {
            PlayerStats.gems = PlayerPrefs.GetInt("Gems");
        }
        else
        {
            PlayerPrefs.SetInt("Gems", 0);
            PlayerStats.gems = 0;
        }

        if(PlayerPrefs.HasKey("XP")){
            PlayerStats.xp = PlayerPrefs.GetInt("XP");
        }else{
            PlayerPrefs.SetInt("XP",0);
            PlayerStats.xp = 0;
        }

        if(PlayerPrefs.HasKey("MaxHealth")){
            PlayerStats.maxHealth = PlayerPrefs.GetInt("MaxHealth");
            PlayerStats.health = PlayerStats.maxHealth;
        }else{
            PlayerPrefs.SetInt("MaxHealth", 3);
            PlayerStats.health = PlayerStats.maxHealth;

        }

        if(PlayerPrefs.HasKey("InstantKillDurationLevel")){
            PlayerStats.instantKillDurationLevel = PlayerPrefs.GetInt("InstantKillDurationLevel");
        }else{
            PlayerPrefs.SetInt("InstantKillDurationLevel", 0);
            PlayerStats.instantKillDurationLevel = 0;
        }

        if (PlayerPrefs.HasKey("ShieldDurationLevel"))
        {
            PlayerStats.shieldDurationLevel = PlayerPrefs.GetInt("ShieldDurationLevel");
        }
        else
        {
            PlayerPrefs.SetInt("ShieldDurationLevel", 0);
            PlayerStats.shieldDurationLevel = 0;
        }

        if (PlayerPrefs.HasKey("DoubleFireRateDurationLevel"))
        {
            PlayerStats.doubleFireRateDurationLevel = PlayerPrefs.GetInt("DoubleFireRateDurationLevel");
        }
        else
        {
            PlayerPrefs.SetInt("DoubleFireRateDurationLevel", 0);
            PlayerStats.doubleFireRateDurationLevel = 0;
        }

        if (PlayerPrefs.HasKey("NukeDurationLevel"))
        {
            PlayerStats.nukeDurationLevel = PlayerPrefs.GetInt("NukeDurationLevel");
        }
        else
        {
            PlayerPrefs.SetInt("NukeDurationLevel", 0);
            PlayerStats.nukeDurationLevel = 0;
        }

        if (PlayerPrefs.HasKey("MultishotDurationLevel"))
        {
            PlayerStats.multishotDurationLevel = PlayerPrefs.GetInt("MultishotDurationLevel");
        }
        else
        {
            PlayerPrefs.SetInt("MultishotDurationLevel", 0);
            PlayerStats.multishotDurationLevel = 0;
        }

        if (PlayerPrefs.HasKey("FreezeDurationLevel"))
        {
            PlayerStats.freezeDurationLevel = PlayerPrefs.GetInt("FreezeDurationLevel");
        }
        else
        {
            PlayerPrefs.SetInt("FreezeDurationLevel", 0);
            PlayerStats.freezeDurationLevel = 0;
        }


        if (PlayerPrefs.HasKey("WeaponNumber")){
            WeaponController.weaponNumber = PlayerPrefs.GetInt("WeaponNumber");
        }
        else{
            PlayerPrefs.SetInt("WeaponNumber", 0);
            WeaponController.weaponNumber = 0;
        }

        if(PlayerPrefs.HasKey("SMGLock")){
            PlayerStats.smgLocked = PlayerPrefs.GetInt("SMGLock");
        }else{
            PlayerPrefs.SetInt("SMGLock", 1);
            PlayerStats.smgLocked = 1;
        }

        if(PlayerPrefs.HasKey("M4Lock")){
            PlayerStats.m4Locked = PlayerPrefs.GetInt("M4Lock");
        }else{
            PlayerPrefs.SetInt("M4Lock", 1);
            PlayerStats.m4Locked = 1;
        }
    }

    // Update is called once per frame
    void Update()
    {
        coinText.text = coins.ToString();
        gemText.text = gems.ToString();
    }
}
