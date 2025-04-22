using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class SceneChanger : MonoBehaviour
{

public void PlayScreen(){

    PlayerStats.wavesCompleted = 0;
    PlayerStats.enemiesKilled = 0;
    PlayerStats.coinsCollected = 0;
    PlayerStats.gemsCollected = 0; 
    PlayerStats.xpCollected = 0;

    PlayerPrefs.SetInt("WeaponNumber", WeaponController.weaponNumber);

    SceneManager.LoadScene("GameScene");
    }

    public void UpgradeScreen(){
        SceneManager.LoadScene("GunUpgradeScene");

    }


}
