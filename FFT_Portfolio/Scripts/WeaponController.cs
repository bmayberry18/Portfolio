using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using TMPro;

public class WeaponController : MonoBehaviour {
    public class Weapon
    {

        public Sprite sprite;
        public float fireRate;
        public int damage;
        public int bulletSpeed;
        public float criticalHitChance;
        public int xp;

        [HideInInspector]
        public float timeBetweenShots;
        public float maxTimeBetweenShots;

        public Weapon(Sprite sprite, float fireRate, int damage, int bulletSpeed, float criticalHitChance, int xp)
        {
            this.sprite = sprite;
            this.fireRate = fireRate;
            this.damage = damage;
            this.bulletSpeed = bulletSpeed;
            this.criticalHitChance = criticalHitChance;
            this.xp = xp;
            maxTimeBetweenShots = 1f / fireRate;
            timeBetweenShots = maxTimeBetweenShots;
        }
    };
    public class KeyVector{
        public string fireRate;
        public string damage;
        public string bulletSpeed;
        public string criticalHitChance;
        public string xp;

        public KeyVector(string fr, string d, string bs, string ch, string xp){
            this.fireRate = fr;
            this.damage = d;
            this.bulletSpeed = bs;
            this.criticalHitChance = ch;
            this.xp = xp;
        }

    }
    public bool isThisInUpgrade;

    [SerializeField] private Sprite pistolSprite;
    [SerializeField] private Sprite smgSprite;
    [SerializeField] private Sprite m4Sprite;

    public static Sprite PistolSprite { get; private set; }
    public static Sprite SmgSprite { get; private set; }
    public static Sprite M4Sprite { get; private set; }

    //[HideInInspector]
    public Transform firePoint, casingPoint;
    public ParticleSystem fireParticles;
    public GameObject bullet_prefab;

    public Transform playerHandTransform;
    private Transform weaponTransform;
    public Transform playerRotation;

    public static int weaponNumber = 0;

    public static KeyVector pistolKeys = new KeyVector("Pistol_fr", "Pistol_d", "Pistol_bs", "Pistol_ch", "Pistol_xp");
    public static KeyVector smgKeys = new KeyVector("Smg_fr", "Smg_d", "Smg_bs", "Smg_ch", "Smg_xp");
    public static KeyVector m4Keys = new KeyVector("M4_fr", "M4_d", "M4_bs", "M4_ch", "M4_xp");

    public static Weapon currentWeapon;
    private Weapon[] weapons;
    public static Weapon pistol;
    public static Weapon smg;
    public static Weapon m4;

    public GameObject pistolPrefab, smgPrefab, m4Prefab;

    public static bool shootingActive;

    private void Awake()
    {
        PistolSprite = pistolSprite;
        SmgSprite = smgSprite;
        M4Sprite = m4Sprite;
    }

    private void Start() {
        //PlayerPrefs.DeleteAll();
        StartFunc();
    }

    private void Update() {
        //Debug.Log(currentWeapon.fireRate);
    }

    public void StartFunc(){
        shootingActive = true;

        if(isThisInUpgrade != true){
            weaponTransform = InstantiateWeaponPrefab();

        }
        InitializeWeapon();

        //Set the weapon transform as whichever prefab is correct.

        for (int i = 0; i < weaponTransform.childCount; i++){
            Transform child = weaponTransform.GetChild(i);
            switch(child.name){
                case "FirePoint":
                    firePoint = child;
                    break;
                case "CasingPoint":
                    casingPoint = child;
                    break;
                case "FireParticles":
                    fireParticles = child.gameObject.GetComponent<ParticleSystem>();
                    break;
                default:
                    break;
            }
        }

    }


    public static void InitializeWeapon(){
        switch (WeaponController.weaponNumber)
        {
            case 0:
                if (WeaponKeyCheck(WeaponController.pistolKeys))
                {
                    SetCurrentWeaponLogic(WeaponController.PistolSprite, WeaponController.pistolKeys);
                }
                else
                {
                    SetWeaponStats(WeaponController.pistolKeys, 1.2f, 10, 10, 0f, 0);
                    SetCurrentWeaponLogic(WeaponController.PistolSprite, WeaponController.pistolKeys);
                }

                break;
            case 1:
                if(WeaponKeyCheck(WeaponController.smgKeys)){
                    SetCurrentWeaponLogic(WeaponController.SmgSprite, WeaponController.smgKeys);
                }else{
                    SetWeaponStats(WeaponController.smgKeys, 4f, 4, 10, 0f, 0);
                    SetCurrentWeaponLogic(WeaponController.SmgSprite, WeaponController.smgKeys);
                }
                break;
            case 2:
                if(WeaponKeyCheck(WeaponController.m4Keys)){
                    SetCurrentWeaponLogic(WeaponController.M4Sprite, WeaponController.m4Keys);
                }else{
                    SetWeaponStats(WeaponController.m4Keys, 2f, 15, 15, 0f, 0);
                    SetCurrentWeaponLogic(WeaponController.M4Sprite, WeaponController.m4Keys);
                }
                break;
            default:
                Debug.LogError("Weapon Number does not exist");
                break;
        }
    }

    static void SetCurrentWeaponLogic(Sprite sprite, KeyVector key){
        currentWeapon = new Weapon(sprite, PlayerPrefs.GetFloat(key.fireRate), PlayerPrefs.GetInt(key.damage), PlayerPrefs.GetInt(key.bulletSpeed), PlayerPrefs.GetFloat(key.criticalHitChance), PlayerPrefs.GetInt(key.xp));
    }

    static void SetWeaponStats(KeyVector key, float fr, int d, int bs, float ch, int xp){
        PlayerPrefs.SetFloat(key.fireRate,fr);
        PlayerPrefs.SetInt(key.damage, d);
        PlayerPrefs.SetInt(key.bulletSpeed, bs);
        PlayerPrefs.SetFloat(key.criticalHitChance, ch);
        PlayerPrefs.SetInt(key.xp, xp);
    }

    Transform InstantiateWeaponPrefab(){
        GameObject weapon;
        Debug.Log("Instantiating");
        switch(WeaponController.weaponNumber){
            case 0:
                weapon = Instantiate(pistolPrefab, playerHandTransform);
                break;
            case 1:
                weapon = Instantiate(smgPrefab, playerHandTransform);
                break;
            case 2:
                weapon = Instantiate(m4Prefab, playerHandTransform);
                break;
            default:
                return null;
        }
        return weapon.transform;
    }

    public void Shoot(float angle, bool multishotCall = false, float multishotAngle = 0){
        if(currentWeapon.timeBetweenShots <= 0f){
            //shoot

            fireParticles.Play();
            if(multishotAngle == 0){

                GameObject bullet = Instantiate(bullet_prefab, firePoint.position, Quaternion.Euler(new Vector3(0, 0, angle - 90f)));
                BulletScript bs = bullet.GetComponent<BulletScript>();


                if (currentWeapon.criticalHitChance != 0)
                {
                    float random = Random.Range(0f, 100f);
                    if (random < currentWeapon.criticalHitChance)
                    {
                        int damage = (int)(currentWeapon.damage * 1.5f);
                        bs.damage = damage;
                    }
                    else
                    {
                        bs.damage = currentWeapon.damage;
                    }
                }
                else
                {
                    bs.damage = currentWeapon.damage;
                }
                bs.speed = currentWeapon.bulletSpeed;

            }else{
                GameObject bullet = Instantiate(bullet_prefab, firePoint.position, Quaternion.Euler(new Vector3(0, 0, angle - 90f)));
                GameObject bullet1 = Instantiate(bullet_prefab, firePoint.position, Quaternion.Euler(new Vector3(0, 0, angle - 90f - multishotAngle)));
                GameObject bullet2 = Instantiate(bullet_prefab, firePoint.position, Quaternion.Euler(new Vector3(0, 0, angle - 90f + multishotAngle)));

                BulletScript bs = bullet.GetComponent<BulletScript>();
                BulletScript bs1 = bullet1.GetComponent<BulletScript>();
                BulletScript bs2 = bullet2.GetComponent<BulletScript>();

                if (currentWeapon.criticalHitChance != 0)
                {
                    float random = Random.Range(0f, 100f);
                    if (random < currentWeapon.criticalHitChance)
                    {
                        int damage = (int)(currentWeapon.damage * 1.5f);
                        bs.damage = damage;
                        bs1.damage = damage;
                        bs2.damage = damage;
                    }
                    else
                    {
                        bs.damage = currentWeapon.damage;
                        bs1.damage = currentWeapon.damage;
                        bs2.damage = currentWeapon.damage;
                    }
                }
                else
                {
                    bs.damage = currentWeapon.damage;
                    bs1.damage = currentWeapon.damage;
                    bs2.damage = currentWeapon.damage;
                }
                bs.speed = currentWeapon.bulletSpeed;
                bs1.speed = currentWeapon.bulletSpeed;
                bs2.speed = currentWeapon.bulletSpeed;
            }

            if(PowerUpController.multishotActive != false && multishotCall != true){
                Shoot(angle, true, 15f);
            }
            currentWeapon.timeBetweenShots = currentWeapon.maxTimeBetweenShots;
        }else{
            currentWeapon.timeBetweenShots -= Time.deltaTime;
        }
    }


    public static void UpdateWeapon(float fr, int d, int bs, float ch, int xp){
        InitializeWeapon();
        WeaponController.currentWeapon.fireRate = fr;
        WeaponController.currentWeapon.damage = d;
        WeaponController.currentWeapon.bulletSpeed = bs;
        WeaponController.currentWeapon.criticalHitChance = ch;
        WeaponController.currentWeapon.xp = xp;

        switch (WeaponController.weaponNumber)
        {
            case 0:
                SetWeaponStats(WeaponController.pistolKeys, fr, d, bs, ch, xp);
                break;
            case 1:
                SetWeaponStats(WeaponController.smgKeys, fr, d, bs, ch, xp);
                break;
            case 2:
                SetWeaponStats(WeaponController.m4Keys, fr, d, bs, ch, xp);
                break;
            default:
                break;
        }
    }

    public static bool WeaponKeyCheck(WeaponController.KeyVector key)
    {
        if (!PlayerPrefs.HasKey(key.fireRate))
        {
            return false;
        }
        if (!PlayerPrefs.HasKey(key.damage))
        {
            return false;
        }
        if (!PlayerPrefs.HasKey(key.bulletSpeed))
        {
            return false;
        }
        if (!PlayerPrefs.HasKey(key.criticalHitChance))
        {
            return false;
        }
        if (!PlayerPrefs.HasKey(key.xp))
        {
            return false;
        }
        return true;
    }
}


    

