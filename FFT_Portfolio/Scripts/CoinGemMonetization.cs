using UnityEngine;

public class CoinGemMonetization : MonoBehaviour
{

    public GameObject coinPanel, gemPanel;

    // Start is called once before the first execution of Update after the MonoBehaviour is created
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    public void EnableCoinPanel(){
        coinPanel.SetActive(true);
    }

    public void EnableGemPanel(){
        gemPanel.SetActive(true);
    }

    public void BackCoin(){
        coinPanel.SetActive(false);
    }

    public void BackGem(){
        gemPanel.SetActive(false);
    }
}
