using UnityEngine;

public class ProjectileCollision : MonoBehaviour
{
    bool collidedWithPlayer = false;
    private PowerUpController puc;

    private void Start() {
        puc = GameObject.FindGameObjectWithTag("GameManager").GetComponent<PowerUpController>();
    }


    public void DestroyProjectile(bool attackedPlayer = false)
    {
        Destroy(this.gameObject);

        if (attackedPlayer == true)
        {
            return;
        }
        PlayerStats.xp++;
        PlayerStats.xpCollected++;
    }


    private void OnCollisionEnter2D(Collision2D other)
    {
        if (collidedWithPlayer == true)
        {
            return;
        }
        if (PowerUpController.shieldActive != false)
        {
            return;
        }
        if (other.gameObject.CompareTag("Player"))
        {
            collidedWithPlayer = true;
            PlayerStats.health--;
            Debug.Log($"Player health = {PlayerStats.health}");
            if (PlayerStats.health <= 0)
            {
                Main.EndGame();
                DestroyProjectile(true);
            }
            else
            {
                DestroyProjectile(true);
            }
            // Main.EndGame();
        }
    }
}
