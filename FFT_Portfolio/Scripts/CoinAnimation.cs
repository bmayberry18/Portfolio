using UnityEngine;

public class CoinAnimation : MonoBehaviour
{
PowerUpUI PUI;

public float coinSpeed;

int xDir, yDir;
private Vector3 direction;

public PowerUp powerUp;

private void Start() {

    xDir = yDir = 1;
    direction = (Vector3.zero - transform.position).normalized;
    PUI = GameObject.FindGameObjectWithTag("GameManager").GetComponent<PowerUpUI>();

}

private void Update() {
    xDir = transform.position.x > 0 ? -1 : 1;
    yDir = transform.position.y > 0 ? -1 : 1;

    transform.position += direction * coinSpeed * Time.deltaTime;

    if (Vector3.Distance(transform.position, Vector3.zero) < 0.1f)
    {

        if(this.gameObject.CompareTag("Coin")){
            PlayerStats.coins++;
            PlayerStats.coinsCollected++;
        }else if(this.gameObject.CompareTag("Gem")){
            PlayerStats.gems++;
            PlayerStats.gemsCollected++;
        }else if(this.gameObject.CompareTag("PowerUp")){
            PowerUpController puc = GameObject.FindGameObjectWithTag("GameManager").GetComponent<PowerUpController>();
            puc.powerupsOnHand.Add(powerUp);
            PUI.AddPowerUpUI(powerUp);
        }


        Destroy(gameObject);
    }

    }


}
