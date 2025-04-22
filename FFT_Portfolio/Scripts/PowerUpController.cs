using UnityEngine;
using UnityEngine.UI;
using System.Collections;
using System.Collections.Generic;

public class PowerUp
{
    public GameObject prefab;
    public Sprite sprite;
    public float effectDuration;
    public string name;
    public bool active;
    public IEnumerator coroutine;    


    public PowerUp(string name, float effectDuration, GameObject prefab, Sprite sprite)
    {
        this.effectDuration = effectDuration;
        this.name = name;
        this.prefab = prefab;
        this.sprite = sprite;
        active = false;
    }

    public IEnumerator StartAndWaitDuration(System.Action onComplete = null)
    {
        active = true;
        Debug.Log($"Powerup: {name} has started");
        yield return new WaitForSeconds(effectDuration);
        Debug.Log($"Powerup: {name} has ended");
        active = false;
        onComplete?.Invoke();
    }

}

public class PowerUpController : MonoBehaviour
{

    public List<PowerUp> powerupsOnHand = new List<PowerUp>{};

    public float[] instantKillDuration, shieldDuration, doubleFireRateDuration, nukeDuration, multishotDuration, freezeDuration;
    public PowerUp InstantKill, Shield, DoubleFireRate, Nuke, MultiShot, Freeze;

    public GameObject powerupPrefab;
    public Sprite instantKillSprite, shieldSprite, doubleFireRateSprite, nukeSprite, multishotSprite, freezeSprite;
    private List<EnemyStats> _enemyStats = new List<EnemyStats>{};
    public static bool shieldActive, multishotActive, freezeActive;

    public GameObject shieldObject;
    

    private void Start() {
        InstantKill = new PowerUp("InstantKill", instantKillDuration[PlayerStats.instantKillDurationLevel], powerupPrefab, instantKillSprite);
        Shield = new PowerUp("Shield", shieldDuration[PlayerStats.shieldDurationLevel], powerupPrefab, shieldSprite);
        DoubleFireRate = new PowerUp("DoubleFireRate", doubleFireRateDuration[PlayerStats.doubleFireRateDurationLevel], powerupPrefab, doubleFireRateSprite);
        Nuke = new PowerUp("Nuke", nukeDuration[PlayerStats.nukeDurationLevel], powerupPrefab, nukeSprite);
        MultiShot = new PowerUp("MutliShot", multishotDuration[PlayerStats.multishotDurationLevel], powerupPrefab, multishotSprite);
        Freeze = new PowerUp("Freeze", freezeDuration[PlayerStats.freezeDurationLevel], powerupPrefab, freezeSprite);
        ShieldActive();
    }


    private void Update() {
        
        if(powerupsOnHand.Count > 0){
            foreach (PowerUp p in powerupsOnHand)
            {

                //removes if the powerup isnt active
                if (!p.active)
                {
                    switch (p.name)
                    {
                        case "InstantKill":
                            int oldDamage = WeaponController.currentWeapon.damage;
                            InstantKillEffect(oldDamage);
                            StartCoroutine(p.StartAndWaitDuration(() =>
                            {
                                WeaponController.currentWeapon.damage = oldDamage;
                                powerupsOnHand.Remove(p);
                            }));
                            break;
                        case "Shield":
                            ShieldEffect();
                            StartCoroutine(p.StartAndWaitDuration(()=>
                            {
                                shieldActive = false;
                                ShieldActive();
                                powerupsOnHand.Remove(p);
                            }));
                            break;
                        case "DoubleFireRate":
                            float oldFireRate = WeaponController.currentWeapon.fireRate;
                            float oldMaxTimeBetweenShots = WeaponController.currentWeapon.maxTimeBetweenShots;
                            float oldTimeBetweenShots = WeaponController.currentWeapon.timeBetweenShots;
                            DoubleFireRateEffect(oldFireRate);
                            StartCoroutine(p.StartAndWaitDuration(() =>
                            {
                                WeaponController.currentWeapon.fireRate = oldFireRate;
                                WeaponController.currentWeapon.maxTimeBetweenShots = oldMaxTimeBetweenShots;
                                WeaponController.currentWeapon.timeBetweenShots = oldTimeBetweenShots;
                                powerupsOnHand.Remove(p);
                            }));
                            break;
                        case "Nuke":
                            NukeEffect();
                            StartCoroutine(p.StartAndWaitDuration(() =>
                            {
                                powerupsOnHand.Remove(p);
                            }));
                            break;
                        case "MutliShot":
                            MutliShotEffect();
                            StartCoroutine(p.StartAndWaitDuration(() =>
                            {
                                multishotActive = false;
                                powerupsOnHand.Remove(p);
                            }));
                            break;
                        case "Freeze":
                            FreezeEffect(false);
                            StartCoroutine(p.StartAndWaitDuration(() =>
                            {
                                FreezeEffect(true);
                                powerupsOnHand.Remove(p);
                            }));
                            break;
                        default:
                            break;
                    }
                }
            }
        }

        
    }



    //Called when enemy is killed if a powerup is supposed to be instantiated
    public PowerUp PowerUpSelector(){
        //Insta Kill, Shield, Double Firerate, Nuke, MutliShot
        int differentPowerupCount = 6;
        int powerupNumber = Random.Range(0, differentPowerupCount);
        GameObject powerUp;

        switch(powerupNumber){
            //InstaKill
            case 0:
                return InstantKill;
            //Shield
            case 1:
                return Shield;
            //Double FireRate
            case 2:
                return DoubleFireRate;
            //Nuke
            case 3:
                return Nuke;
            //MutliShot
            case 4:
                return MultiShot;
            case 5:
                return Freeze;
            default:
                Debug.LogError($"Powerup Number {powerupNumber} does not exist");
                return null;
        }
    }


    void InstantKillEffect(int oldDamage)
    {
        WeaponController.currentWeapon.damage = int.MaxValue;
    }

    void ShieldEffect()
    {
        shieldActive = true;
        ShieldActive();
    }

    void DoubleFireRateEffect(float oldFireRate)
    {
        Debug.Log($"Fire Rate before: {WeaponController.currentWeapon.fireRate}");
        WeaponController.currentWeapon.fireRate = oldFireRate * 2;
        WeaponController.currentWeapon.maxTimeBetweenShots = 1f / WeaponController.currentWeapon.fireRate;
        WeaponController.currentWeapon.timeBetweenShots = WeaponController.currentWeapon.maxTimeBetweenShots;
        Debug.Log($"Fire Rate After: {WeaponController.currentWeapon.fireRate}");
    }

    void NukeEffect()
    {
        if(GameObject.FindGameObjectWithTag("Enemy") == null) return;
        GameObject[] _enemies = GameObject.FindGameObjectsWithTag("Enemy");
        foreach(GameObject e in _enemies){
            if(e != null){
                _enemyStats.Add(e.GetComponent<EnemyStats>());
            }
        }
        foreach(EnemyStats e in _enemyStats){
            if(e != null){
                int health = e.health;
                e.Damage(health);
            }
        }
        
    }

    void MutliShotEffect()
    {
        multishotActive = true;
    }


    void FreezeEffect(bool reset = false){
        if (GameObject.FindGameObjectWithTag("Enemy") == null) return;
        GameObject[] _enemies = GameObject.FindGameObjectsWithTag("Enemy");
        foreach (GameObject e in _enemies)
        {
            if (e != null)
            {
                _enemyStats.Add(e.GetComponent<EnemyStats>());
            }
        }
        foreach (EnemyStats e in _enemyStats)
        {
            if (e != null)
            {
                e.FreezeEnemy(reset);
            }
        }
    }

    void ShieldActive(){
        if (shieldActive != true)
        {
            shieldObject.SetActive(false);
        }
        else
        {
            shieldObject.SetActive(true);
        }
    }

    



}
