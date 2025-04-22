using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using TMPro;
using UnityEngine.UI;

public class EnemyController : MonoBehaviour
{

    public class Enemy{
        public float speed;
        public int health;
        public Sprite sprite;

        public Enemy(float speed, int health, Sprite sprite){
            this.speed = speed;
            this.health = health;
            this.sprite = sprite;
        }
    };

    public class Wave{
        public float enemyCount;
        public float hotdogChance;
        public float burgerChance;
        public float friesChance;
        public float milkshakeChance;

        public Wave(float enemyCount, float hotdogChance, float burgerChance, float friesChance, float milkshakeChance){
            this.enemyCount = enemyCount;
            this.hotdogChance = hotdogChance;
            this.burgerChance = burgerChance;
            this.friesChance = friesChance;
            this.milkshakeChance = milkshakeChance;
        }
    }

    public int maxfriesHealth = 10;
    public int maxhotdogHealth = 20;
    public int maxburgerHealth = 50;
    public int maxmilkshakeHealth = 35;

    private int friesHealth;
    private int hotdogHealth;
    private int burgerHealth;
    private int milkshakeHealth;

    public float friesSpeed = 8f;
    public float hotdogSpeed = 5f;
    public float burgerSpeed = 1f;
    public float milkshakeSpeed = 4f;

    public float initialEnemyCount;
    private float enemyCount = 20f;
    public Wave initialWave;

    public Enemy fries;
    public Enemy hotdog;
    public Enemy burger;
    public Enemy milkshake;

    private float timeBetweenWaves;
    public float maxTimeBetweenWaves = 5f;

    private float timeBetweenSpawns;
    public float maxTimeBetweenSpawns = 0.25f;

    private GameObject[] enemies; 

    public float spawnOffset = 5f;
    private Vector3 screenBottomLeft, screenTopRight;

    public Sprite hotDogSprite;
    public Sprite ketchupHotDogSprite, mustardHotDogSprite, relishHotDogSprite;
    public Sprite burgerSprite;
    public Sprite friesSprite;
    public Sprite milkshakeSprite;

    public GameObject enemyObject;

    public static int enemyKills;

    public float waveNumber = 0;

    public TMP_Text waveText;
    public Image waveProgressBar;

    public static int enemiesKilledThisWave = 0;
    private int enemiesThisWave;


    // Start is called before the first frame update
    void Start()
    {
        StartFunc();
    }

    public void StartFunc(){
        waveNumber = 0;

        friesHealth = maxfriesHealth;
        hotdogHealth = maxhotdogHealth;
        burgerHealth = maxburgerHealth;
        milkshakeHealth = maxmilkshakeHealth;

        fries = new Enemy(friesSpeed, friesHealth, friesSprite);
        hotdog = new Enemy(hotdogSpeed, hotdogHealth, hotDogSprite);
        burger = new Enemy(burgerSpeed, burgerHealth, burgerSprite);
        milkshake = new Enemy(milkshakeSpeed, milkshakeHealth, milkshakeSprite);

        timeBetweenWaves = maxTimeBetweenWaves;
        initialEnemyCount = enemyCount;
        enemiesThisWave = (int)enemyCount;
        enemiesKilledThisWave = 0;
        initialWave = new Wave(initialEnemyCount, 100f, 0f, 0f, 0f);
        timeBetweenSpawns = maxTimeBetweenSpawns;
        screenBottomLeft = Camera.main.ScreenToWorldPoint(new Vector3(0f, 0f, 0f));
        screenTopRight = Camera.main.ScreenToWorldPoint(new Vector3(Screen.width, Screen.height, 0f));

    }

    // Update is called once per frame
    void Update()
    {

        if(timeBetweenWaves <= 0f){
            if(initialWave.enemyCount > 0f){
                if(timeBetweenSpawns <= 0f){
                    if (SpawnEnemy(initialWave.hotdogChance, initialWave.burgerChance, initialWave.friesChance, initialWave.milkshakeChance))
                    {
                        initialWave.enemyCount -= 1;
                        timeBetweenSpawns = maxTimeBetweenSpawns;
                    }
                }else{
                    timeBetweenSpawns -= Time.deltaTime;
                }
            }else{
                enemies = GameObject.FindGameObjectsWithTag("Enemy");
                //Debug.Log($"Enemies: {enemies.Length}");
                if (enemies.Length == 0)
                {
                    waveNumber++;
                    enemiesKilledThisWave = 0;

                    PlayerStats.wavesCompleted++;
                    PlayerStats.xp += 50;
                    PlayerStats.xpCollected += 50;

                    initialEnemyCount += 5;
                    enemiesThisWave = (int)initialEnemyCount;

                    float h = hotdogHealth * 1.2f;
                    float b = burgerHealth * 1.2f;
                    float f = friesHealth * 1.2f;
                    float ms = milkshakeHealth * 1.2f;

                    hotdogHealth = (int)h;
                    burgerHealth = (int)b;
                    friesHealth = (int)f;
                    milkshakeHealth = (int)ms;


                    float milkshakeChance;
                    if (waveNumber >= 5){
                        milkshakeChance = initialWave.milkshakeChance += 3;
                    }else{
                        milkshakeChance = initialWave.milkshakeChance;
                    }

                    Wave newWave = new Wave(initialEnemyCount, initialWave.hotdogChance += 3, initialWave.burgerChance += 5, initialWave.friesChance += 2, milkshakeChance);
                    initialWave = newWave;

                    fries = new Enemy(friesSpeed, friesHealth, friesSprite);
                    hotdog = new Enemy(hotdogSpeed, hotdogHealth, hotDogSprite);
                    burger = new Enemy(burgerSpeed, burgerHealth, burgerSprite);
                    milkshake = new Enemy(milkshakeSpeed, milkshakeHealth, milkshakeSprite);

                    if(maxTimeBetweenSpawns >= 0.05f){
                        maxTimeBetweenSpawns -= 0.05f;
                    }
                    //spawn enemy
                    timeBetweenWaves = maxTimeBetweenWaves;
                }
            }

        }else{
            timeBetweenWaves -= Time.deltaTime;
        }

        WaveDisplay();

    }

    bool SpawnEnemy(float hd, float bc, float fc, float msc){
        float random = Random.Range(0,hd+bc+fc+msc);
        if(random <= hd){
            //spawn hotdog
            InstantiateEnemy(hotdog, SpawnLocation(), hotdogBool:true);
            //Debug.Log("Spawn Hotdog");
            return true;
        }else if(random<= hd + bc){
            //spawn a burger
            InstantiateEnemy(burger, SpawnLocation());
            //Debug.Log("Spawn Burger");
            return true;
        }else if(random <= hd + bc + fc){
            //spawn a fry
            InstantiateEnemy(fries, SpawnLocation());
            //Debug.Log("Spawn Fries");
            return true;
        }else if (random <= hd + bc + fc + msc){
            InstantiateEnemy(milkshake, SpawnLocation(), milkshakeBool:true);
            return true;
        }else{
            return false;
        }
    }

    
    Vector3 SpawnLocation(){
        float spawnX = 0f;
        float spawnY = 0f;

        int side = Random.Range(0,4);

        switch(side){
            case 0: //Left side of screen
                spawnX = screenBottomLeft.x - spawnOffset;
                spawnY = Random.Range(screenBottomLeft.y, screenTopRight.y);
                break;
            case 1: //Right
                spawnX = screenTopRight.x + spawnOffset;
                spawnY = Random.Range(screenBottomLeft.y, screenTopRight.y);
                break;
            case 2: //Top
                spawnX = Random.Range(screenBottomLeft.x, screenTopRight.x);
                spawnY = screenTopRight.y + spawnOffset;
                break;
            case 3: //Bottom
                spawnX = Random.Range(screenBottomLeft.x, screenTopRight.x);
                spawnY = screenBottomLeft.y - spawnOffset;
                break;
            default:
                break;
        }
        Vector3 spawnPosition = new Vector3(spawnX, spawnY, 0f);
        return spawnPosition;
    }

    void InstantiateEnemy(Enemy enemy, Vector3 spawnPos, bool milkshakeBool = false, bool hotdogBool = false ){

        GameObject _e = Instantiate(enemyObject, spawnPos, Quaternion.identity);
        EnemyStats es = _e.GetComponent<EnemyStats>();
        if (milkshakeBool == true)
        {
            es.milkshake = true;
        }
        es.speed = enemy.speed;
        es.health = enemy.health;

        if(hotdogBool == true){
            int rand = Random.Range(0,3);
            switch(rand){
                case 0:
                    es.sprite = ketchupHotDogSprite;
                    break;
                case 1:
                    es.sprite = mustardHotDogSprite;
                    break;
                case 2:
                    es.sprite = relishHotDogSprite;
                    break;
                default:
                    break;
            }
        }else{
            es.sprite = enemy.sprite;
        }

    }

    public void DestroyAllEnemies(){
        enemies = GameObject.FindGameObjectsWithTag("Enemy");
        foreach(GameObject enemy in enemies){
            Destroy(enemy);
        }
    }

    void WaveDisplay(){
        waveText.text = $"Wave {waveNumber+1}";
        waveProgressBar.fillAmount = (float)enemiesKilledThisWave / (float)enemiesThisWave;
    }


}
