using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using TMPro;

public class EnemyStats : MonoBehaviour
{

    public float speed;
    public int health;
    private int maxHealth;
    public Sprite sprite;
    public Image healthBar;
    public GameObject healthBarBackground;
    public ParticleSystem damageParticles;
    
    public Color _green;
    public Color _orange;
    public Color _red;
    public Color _white;
    public Color _freeze;

    private Vector3 targetPosition;
    private Vector3 direction;

    public GameObject damageText;
    public GameObject damageParent;

    private SpriteRenderer sr;
    public Animator damageAnimation;
    public AnimationClip damageClip;

    public GameObject coinPrefab;
    public GameObject gemPrefab;

    private PowerUpController puc;

    bool collidedWithPlayer = false;

    public bool milkshake = false;
    public float milkshakeRadius = 10f;
    public float spiralAngle = 15f;


    float timeBetweenAttacks;
    public float maxTimeBetweenAttacks = 5f;

    public GameObject milkshakeProjectile;
    public float milkshakeProjectileSpeed = 1f;

    private float originalSpeed;



    void Start(){
        sr = this.GetComponent<SpriteRenderer>();
        // Calculate the direction towards the center of the screen (0, 0)
        targetPosition = Vector3.zero; // The center of the screen is at (0, 0) in world space
        direction = (targetPosition - transform.position).normalized; // Get the direction and normalize it

        puc = GameObject.FindGameObjectWithTag("GameManager").GetComponent<PowerUpController>();

        if(transform.position.x >= 0){
            sr.flipX = true;
        }

        sr.sprite = sprite;
        maxHealth = health;

        timeBetweenAttacks = maxTimeBetweenAttacks;
        originalSpeed = speed;
    }

    void Update(){
        if(Main.gameOver == true){
            return;
        }

        if (PowerUpController.shieldActive != false)
        {
            direction = (targetPosition - transform.position).normalized; // Get the direction and normalize it
        }

        Move();
        

        if(health == maxHealth){
            healthBar.gameObject.SetActive(false);
            healthBarBackground.SetActive(false);
        }else{
            if(healthBar.gameObject.activeSelf == false){
                healthBar.gameObject.SetActive(true);
                healthBarBackground.SetActive(true);
            }
            float fill = (float)health / (float)maxHealth;
            healthBar.fillAmount = fill;
            if (fill > 0.67f)
            {
                healthBar.color = _green;
            }
            else if (fill > 0.33f)
            {
                healthBar.color = _orange;
            }
            else
            {
                healthBar.color = _red;
            }
        }


        if(maxTimeBetweenAttacks > 0){
            if (timeBetweenAttacks <= 0)
            {
                //Attack
                if(milkshake == true){
                    GameObject projectile = Instantiate(milkshakeProjectile, transform.position, Quaternion.identity);
                    Rigidbody2D rb = projectile.GetComponent<Rigidbody2D>();
                    Vector3 _direction = (targetPosition - transform.position).normalized;
                    rb.linearVelocity = _direction * milkshakeProjectileSpeed;
                }

                timeBetweenAttacks = maxTimeBetweenAttacks;
            }
            else
            {
                timeBetweenAttacks -= Time.deltaTime;
            }
        }

        // if(!IsAnimationPlaying(damageClip.name)){
        //     damageAnimation.Play(damageClip.name);
        // }



    }
    
    void Move()
    {
        if(milkshake == true){
            if (transform.position.x >= 0)
            {
                sr.flipX = true;
            }

            sr.sprite = sprite;
            float dist = Vector3.Distance(targetPosition, transform.position);
            direction = (targetPosition - transform.position).normalized; // Get the direction and normalize it
            if (dist > milkshakeRadius){
                //Spiral in
                float theta = spiralAngle * Mathf.Deg2Rad;
                Vector2 rotatedDirection = new Vector2(
                    direction.x * Mathf.Cos(theta) - direction.y * Mathf.Sin(theta),
                    direction.x * Mathf.Sin(theta) + direction.y * Mathf.Cos(theta)
                );

                transform.position += (Vector3)(rotatedDirection.normalized * speed * Time.deltaTime);
            }else{
                Vector2 orbitDirection = new Vector2(-direction.y, direction.x);
                transform.position += (Vector3)(orbitDirection.normalized * speed *  Time.deltaTime);
            }
        }else{
            // Move the enemy in the direction of the center at the specified speed
            transform.position += direction * speed * Time.deltaTime;
        }

    }

    // Call this method to check if a specific animation is playing
    public bool IsAnimationPlaying(string animationName)
    {
        // Get the current state information from the Animator's first layer (usually 0)
        AnimatorStateInfo stateInfo = damageAnimation.GetCurrentAnimatorStateInfo(0);

        // Check if the current state's name matches the animation we’re looking for
        return stateInfo.IsName(animationName);
    }


    public int Damage(int damage, Transform bulletTransform = null){
        health -= damage;
        GameObject text = Instantiate(damageText, damageParent.transform);
        TMP_Text text1 = text.transform.GetChild(0).gameObject.GetComponent<TMP_Text>();
        TMP_Text text2 = text.transform.GetChild(1).gameObject.GetComponent<TMP_Text>();
        text1.text = damage.ToString();
        text2.text = damage.ToString();

        if(bulletTransform != null){
            damageParticles.transform.position = new Vector3(transform.position.x, bulletTransform.position.y, 1f);
            damageParticles.transform.rotation = Quaternion.Euler(bulletTransform.eulerAngles.z-90f, -90f, 0f);
            damageParticles.Play();
        }

        if (health <= 0){
            DestroyEnemy();
            if(health == 0){
                return 0;
            }else{
                return Mathf.Abs(health);
            }
        }else{
            damageAnimation.Play(damageClip.name,0, 0.0f);
            return 0;
        }
    }

    void DestroyEnemy(bool attackedPlayer = false){
        Destroy(this.gameObject);
        EnemyController.enemiesKilledThisWave++;
        if(attackedPlayer == true){
            return;
        }
        PlayerStats.xp++;
        PlayerStats.xpCollected++;
        PlayerStats.enemiesKilled++;

        int randomNum = Random.Range(0, 101);
        if(randomNum >= 100){
            GameObject gem = Instantiate(gemPrefab, transform.position, Quaternion.identity);
        }else if(randomNum >= 97 && randomNum < 100){
            Debug.Log("Instantiated a powerup");
            PowerUp powerUp = puc.PowerUpSelector();
            GameObject pu_object = Instantiate(powerUp.prefab, transform.position, Quaternion.identity);
            CoinAnimation ca = pu_object.GetComponent<CoinAnimation>();
            ca.powerUp = powerUp;
        }
        else
        {
            GameObject coin = Instantiate(coinPrefab, transform.position, Quaternion.identity);
        }
        EnemyController.enemyKills++;
    }


    private void OnCollisionEnter2D(Collision2D other) {
        if(collidedWithPlayer == true){
            return;
        }
        if(PowerUpController.shieldActive != false){
            return;
        }
        if (other.gameObject.CompareTag("Player"))
        {
            collidedWithPlayer = true;
            PlayerStats.health--;
            if(PlayerStats.health <= 0){
                Main.EndGame();
                DestroyEnemy(true);
            }else{
                DestroyEnemy(true);
            }
            // Main.EndGame();
        }
    }

    public void FreezeEnemy(bool reset = false){
        if(reset == false){
            sr.color = _freeze;
            speed = (int)((float) speed/ 10f);
        }else{
            sr.color = _white;
            speed = originalSpeed;
        }
    }

}
