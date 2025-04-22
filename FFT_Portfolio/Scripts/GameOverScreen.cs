using UnityEngine;
using TMPro;

public class GameOverScreen : MonoBehaviour
{

public TMP_Text WavesCompleted, EnemiesKilled, CoinsCollected, GemsCollected, XPCollected;

public GameObject gameOverScreen;

public RewardedAdsButton rab;
bool loadedAd = false;


void Update(){

    if(!gameOverScreen.activeSelf) return;
    if(loadedAd != true){
        rab.LoadAd();
        loadedAd = true;
    }


    WavesCompleted.text = $"{PlayerStats.wavesCompleted}";
    EnemiesKilled.text = $"{PlayerStats.enemiesKilled}";
    CoinsCollected.text = $"{PlayerStats.coinsCollected}";
    GemsCollected.text = $"{PlayerStats.gemsCollected}";
    XPCollected.text = $"{PlayerStats.xpCollected}";

}



}
