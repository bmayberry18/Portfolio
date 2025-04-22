using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class Main : MonoBehaviour
{
    private Camera m_camera;
    private Transform playerTransform;
    public static Vector3 mousePos;

    private static WeaponController wc;
    private static EnemyController ec;

    public GameObject endGamePanel;

    public static Main _main;
    public static bool gameOver = false;
    private bool isFlipped = false;

    public Transform fireFromPosition;

    private Vector2 previousDirection;


    // Start is called before the first frame update
    void Start()
    {
        //Assigns the camera gameObject
        m_camera = Camera.main;
        //Assigns the player transform
        playerTransform = GameObject.FindGameObjectWithTag("Player").GetComponent<Transform>();

        //Find the weapon controller that is attatched to the gameManager.
        wc = this.gameObject.GetComponent<WeaponController>();
        ec = this.gameObject.GetComponent<EnemyController>();

        _main = this.GetComponent<Main>();

        gameOver = false;

        //PlayerStats.currency = PlayerPrefs.GetInt("Currency");
        
    }

    void Awake(){
        Application.targetFrameRate = 60;
    }

    // Update is called once per frame
    void Update()
    {
        //Gets the input values for the mouse
        mousePos = Input.mousePosition;
        //converrts the mouse position to worldspace
        mousePos = m_camera.ScreenToWorldPoint(new Vector3(mousePos.x, mousePos.y, 0));

        float angle = RotatePlayer();
        if(WeaponController.shootingActive == true){
            wc.Shoot(angle);
        }
    }

    private float RotatePlayer(){
        // Calculate the angle between the player and the mouse

        float xpos = mousePos.x - fireFromPosition.position.x;
        float ypos = mousePos.y - fireFromPosition.position.y;
        Vector2 direction;

        if (xpos < 0.5f && xpos > -0.5f && ypos < 0.5f && ypos > -0.5f)
        {
            direction = previousDirection;
        }else{
            direction = new Vector2(xpos, ypos);
            previousDirection = direction;
        }


        float angle = Mathf.Atan2(direction.y, direction.x) * Mathf.Rad2Deg;

        // Flip threshold — avoid flipping near vertical top/bottom
        if (!isFlipped && direction.x < -1f)
        {
            isFlipped = true;
        }
        else if (isFlipped && direction.x > 1f)
        {
            isFlipped = false;
        }

        // Apply rotation with conditional flipping
        if (isFlipped)
        {
            playerTransform.rotation = Quaternion.Euler(180, 0, -angle);
        }
        else
        {
            playerTransform.rotation = Quaternion.Euler(0, 0, angle);
        }

        return angle;
    }

    public static void EndGame(){
        _main.endGamePanel.SetActive(true);
        float ek = EnemyController.enemyKills;
        PlayerStats.coins += (int)(ek/10);
        PlayerPrefs.SetInt("Coins", PlayerStats.coins);
        PlayerPrefs.SetInt("Gems", PlayerStats.gems);
        PlayerStats.totalXpToGive = PlayerStats.xpCollected;
        WeaponController.shootingActive = false;
        Main.gameOver = true;
    }

    public void PlayAgain(){
        ec.DestroyAllEnemies();
        _main.endGamePanel.SetActive(false);
        wc.StartFunc();
        ec.StartFunc();
        PlayerStats.health = PlayerStats.maxHealth;
        Main.gameOver = false;
    }

    public void ExitGame(){
        
    }

    public void DoubleReward(){
        PlayerStats.coins+=PlayerStats.coinsCollected/2;
        PlayerStats.gems += PlayerStats.gemsCollected/2;
        PlayerPrefs.SetInt("Coins", PlayerStats.coins);
        PlayerPrefs.SetInt("Gems", PlayerStats.gems);
    }

    private void OnApplicationQuit() {
        //PlayerPrefs.SetInt("Currency", PlayerStats.currency);
        PlayerPrefs.Save();
    }
}
