using UnityEngine;
using UnityEngine.UI;
using TMPro;
using System.Collections;
using System.Collections.Generic;

[System.Serializable]
public class pu_upgrade{
    public TMP_Text upgradeCostText;
    public Image sprite;
    public int[] cost;
    public Image progressBar;
    public Button upgradeButton;

    public int id = 0;
    int level;

    void ReadLevel(){
        switch(id){
            case 0:
                level = PlayerStats.instantKillDurationLevel;
                break;
            case 1:
                level = PlayerStats.shieldDurationLevel;
                break;
            case 2:
                level = PlayerStats.doubleFireRateDurationLevel;
                break;
            case 3:
                level = PlayerStats.nukeDurationLevel;
                break;
            case 4:
                level = PlayerStats.multishotDurationLevel;
                break;
            default:
                break;
        }
    }

    void SetLevel(){
        switch (id)
        {
            case 0:
                PlayerStats.instantKillDurationLevel = level;
                break;
            case 1:
                PlayerStats.shieldDurationLevel = level;
                break;
            case 2:
                PlayerStats.doubleFireRateDurationLevel = level;
                break;
            case 3:
                PlayerStats.nukeDurationLevel = level;
                break;
            case 4:
                PlayerStats.multishotDurationLevel = level;
                break;
            default:
                break;
        }
    }

    void SaveValues(){
        switch(id){
            case 0:
                PlayerPrefs.SetInt("InstantKillDurationLevel", level);
                break;
            case 1:
                PlayerPrefs.SetInt("ShieldDurationLevel", level);
                break;
            case 2:
                PlayerPrefs.SetInt("DoubleFireRateDurationLevel", level);
                break;
            case 3:
                PlayerPrefs.SetInt("NukeDurationLevel", level);
                break;
            case 4:
                PlayerPrefs.SetInt("MultishotDurationLevel", level);
                break;
            default:
                break;
        }
    }

    public void SetText(bool readLevel){
        if(readLevel == true){
            ReadLevel();
        }
        //Debug.Log($"Level: {level}");
        upgradeCostText.text = (cost[level]).ToString();
    }

    public void Upgrade(){
        ReadLevel();
        if(PlayerStats.coins >= cost[level]){
            PlayerStats.coins -= cost[level];
            level++;
            SetLevel();
            SetText(false);
            SetProgressBar(false);
            SaveValues();
            PlayerPrefs.SetInt("Coins", PlayerStats.coins);
        }
    }

    public void SetProgressBar(bool readLevel){
        if(readLevel == true){
            ReadLevel();
        }

        progressBar.fillAmount = ((float)level / cost.Length);
        //Debug.Log($"level: {level}, cost: {cost.Length}, fillamount: {progressBar.fillAmount}");

    }
};

public class PowerUpUpgrades : MonoBehaviour
{

    [SerializeField]
    public List<pu_upgrade> pu_upgrades = new List<pu_upgrade>{};

    //Gameobject for the panel for all powerup upgrades
    public GameObject panel;

    private void Start() {

        foreach(pu_upgrade pu in pu_upgrades){
            pu.SetText(true);
            pu.SetProgressBar(true);
            pu.upgradeButton.onClick.AddListener(pu.Upgrade);
        }
    }

    //Used for the back button in the panel.
    public void Back()
    {
        panel.SetActive(false);
    }

    //Used for the powerup menu button to open upgrade screen
    public void OpenPowerupUpgrades(){
        panel.SetActive(true);
    }
}


