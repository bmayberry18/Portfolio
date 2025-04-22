using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BulletScript : MonoBehaviour
{
    public int damage;
    public int speed;

    private float time = 1f;

    private void Start(){
        this.GetComponent<Rigidbody2D>().linearVelocity = transform.up * speed;
        
    }

    private void Update() {
        if(time <= 0f){
            Destroy(this.gameObject);
        }else{
            time-=Time.deltaTime;
        }
    }

    private void OnTriggerEnter2D(Collider2D other) {
        if (other.gameObject.CompareTag("Enemy")){
            //Damage Enemy
            EnemyStats es = other.gameObject.GetComponent<EnemyStats>();
            int remainingDamage = es.Damage(damage, transform);

            if(remainingDamage == 0){
                Destroy(this.gameObject);
            }else{
                damage = remainingDamage;
            }
        }else if(other.gameObject.CompareTag("Projectile")){
            ProjectileCollision pc = other.gameObject.GetComponent<ProjectileCollision>();
            pc.DestroyProjectile();
        }
    }


}
