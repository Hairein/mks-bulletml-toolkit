// Example Unity script for handling MksBulletmlInterpreter playback and bullet management
// Provided BulletML files were generated and exported by the MKS BulletML Pattern Builder @ https://store.steampowered.com/app/3950780/MKS_BulletML_Pattern_Builder/
using System;
using UnityEngine;
using UnityEngine.InputSystem;
using MksBulletmlInterpreter;

public class MksBmlToolkitHandler : MonoBehaviour
{
    bool MksBmlTkInitialized = false;

    int Major, Minor, Patch;

    [SerializeField] Sprite BulletSprite;

    // UNCOMMENT one file here to load and display
    // and move the mouse around in the window for the aimed xml files
    //string BulletMLFilepath = "Assets/BulletML_Files/basic_bml.xml";
    //string BulletMLFilepath = "Assets/BulletML_Files/aimed_single_bml.xml";
    string BulletMLFilepath = "Assets/BulletML_Files/aimed_bml.xml";
    
    bool BmlFileLoaded = false;
    uint BulletMLPlaybackHandle = 0;

    int WindowWidth = Screen.width;
    int WindowHeight = Screen.height;   
    int WindowCenterX = Screen.width / 2;
    int WindowCenterY = Screen.height / 2;

    bool IsPlaying = false;

    const int MaxBullets = 5000;
    VirtualBullet[] Bullets = new VirtualBullet[MaxBullets];
    IntPtr[] BulletPtrs = new IntPtr[MaxBullets];
    uint[] BulletHandlesToDelete = new uint[MaxBullets];
    GameObject[] BulletObjects = new GameObject[MaxBullets];

    int BulletCount = 0;
    float ScaleFactor = 30.0f;

    void UpdatePlayerPositionFromMouse()
    {
        if (Mouse.current == null)
        {
            return;
        }

        Camera activeCamera = Camera.main;

        if (activeCamera == null)
        {
            return;
        }

        Vector2 mouseScreenPosition = Mouse.current.position.ReadValue();
        float distanceFromCamera = Mathf.Abs(activeCamera.transform.position.z);
        Vector3 mouseWorldPosition = activeCamera.ScreenToWorldPoint(new Vector3(mouseScreenPosition.x, mouseScreenPosition.y, distanceFromCamera));

        MksBulletmlInterpreterNative.mksbmli_set_player_position(
            BulletMLPlaybackHandle,
            mouseWorldPosition.x * ScaleFactor,
            -mouseWorldPosition.y * ScaleFactor);
    }

    void PreInstantiateBullets()
    {
        if (BulletSprite == null)
        {
            Debug.LogError("BulletSprite is not assigned.");
            return;
        }

        for (int i = 0; i < MaxBullets; i++)
        {
            GameObject bulletObject = new GameObject($"Bullet_{i}");
            bulletObject.transform.SetParent(transform, false);

            SpriteRenderer spriteRenderer = bulletObject.AddComponent<SpriteRenderer>();
            spriteRenderer.sprite = BulletSprite;

            bulletObject.SetActive(false);
            BulletObjects[i] = bulletObject;
        }
    }
    
    // Start is called once before the first execution of Update after the MonoBehaviour is created
    void Start()
    {
        Debug.Log("Initializing MksBulletmlInterpreter...");

        // 1. Initialize the MksBulletmlInterpreter
        if(MksBulletmlInterpreterNative.mksbmli_init() == (int)MksbmliErrorCode.NoError)
        {
            Debug.Log("MksBulletmlInterpreter initialized successfully.");
            
            MksBmlTkInitialized = true;

            // 2. Read the version of the MksBulletmlInterpreter
            MksBulletmlInterpreterNative.mksbmli_get_version(out Major, out Minor, out Patch);  
            
            Debug.Log("MksBulletmlInterpreter version: " + Major + "." + Minor + "." + Patch);

            // and set the random seed for reproducibility                
            MksBulletmlInterpreterNative.mksbmli_set_random_seed(1337);

            // 3. Load the BulletML file
            BmlFileLoaded = MksBulletmlInterpreterNative.mksbmli_load_xml(BulletMLFilepath, out BulletMLPlaybackHandle) == (int)MksbmliErrorCode.NoError;            
            if(BmlFileLoaded)
            {
                Debug.Log("BulletML file loaded successfully: " + BulletMLFilepath);
                
                Debug.Log("BulletMLPlaybackHandle: " + BulletMLPlaybackHandle);
               
                // 4. Start playback and configure initial settings
                MksBulletmlInterpreterNative.mksbmli_start_playback(BulletMLPlaybackHandle);
                
                // 5. Configure initial rank, emitter center, and player position
                MksBulletmlInterpreterNative.mksbmli_set_rank(BulletMLPlaybackHandle, 0.5f);
                MksBulletmlInterpreterNative.mksbmli_set_emitter_center(BulletMLPlaybackHandle, 0, 0); // Relative to window center      
                MksBulletmlInterpreterNative.mksbmli_set_player_position(
                    BulletMLPlaybackHandle,
                    WindowCenterX * ScaleFactor,
                    -(WindowCenterY + (WindowCenterY / 2f)) * ScaleFactor);
                
                IsPlaying = true;

                PreInstantiateBullets();
                
                Debug.Log("Started playback");
            }
            else
            {
                Debug.LogError("Failed to load BulletML file: " + BulletMLFilepath);
            }
        }
        else
        {
            Debug.LogError("Failed to initialize MksBulletmlInterpreter.");
        }

        Debug.Log("WindowCenter: (" + WindowCenterX + ", " + WindowCenterY + ")");
    }

    void OnDestroy()
    {
        if (MksBmlTkInitialized)
        {
            if(IsPlaying)
            {
                // 1. Stop playback if it is currently playing
                MksBulletmlInterpreterNative.mksbmli_stop_playback(BulletMLPlaybackHandle);
                
                Debug.Log("Stopped playback");
                
                // 2. Clear all bullets
                MksBulletmlInterpreterNative.mksbmli_clear_bullets(BulletMLPlaybackHandle);                
            }

            Debug.Log("Shutting down MksBulletmlInterpreter...");


            MksBulletmlInterpreterNative.mksbmli_shutdown();
            
            MksBmlTkInitialized = false;
        }
    }

    // Update is called once per frame
    void FixedUpdate()
    {
       if(IsPlaying) 
       {
            UpdatePlayerPositionFromMouse();

            // 1. Calculate the next frame for the BulletML playback
            MksBulletmlInterpreterNative.mksbmli_next_frame(BulletMLPlaybackHandle);

            // 2. Retrieve the current bullets from the BulletML playback
            if(MksBulletmlInterpreterNative.mksbmli_get_bullets(BulletMLPlaybackHandle, MaxBullets, BulletPtrs, out BulletCount) == (int)MksbmliErrorCode.NoError)
            {
                //Debug.Log("NosBullets: " + BulletCount);
                
                if(BulletCount > 0)
                {
                    int BulletDeleteCount = 0;

                    for(int i = 0; i < BulletCount; i++)
                    {
                        Bullets[i] = MksBulletmlInterpreterNative.ReadBullet(BulletPtrs[i]);
                        float bulletX = Bullets[i].position.x / ScaleFactor;
                        float bulletY = Bullets[i].position.y / ScaleFactor;

                        if (BulletObjects[i] != null)
                        {
                            BulletObjects[i].SetActive(true);
                            BulletObjects[i].transform.position = new Vector3(bulletX, bulletY, 0f);
                        }

                        bool isOutsideWindow = bulletX < (-WindowCenterX/ScaleFactor) || bulletX > (WindowCenterX/ScaleFactor) || bulletY < -WindowCenterY/ScaleFactor || bulletY > (WindowCenterY/ScaleFactor);
                        if (isOutsideWindow)
                        {
                            BulletHandlesToDelete[BulletDeleteCount++] = Bullets[i].handle;

                            if (BulletObjects[i] != null)
                            {
                                BulletObjects[i].SetActive(false);
                            }
                        }

                        // Example access after marshaling:
                        // Debug.Log($"Bullet {i}: handle={Bullets[i].handle}, pos=({Bullets[i].position.x}, {Bullets[i].position.y})");
                    }


                    // 3. Delete bullets that have moved outside the window
                    if (BulletDeleteCount > 0)
                    {
                        MksBulletmlInterpreterNative.mksbmli_delete_bullets(BulletMLPlaybackHandle, BulletHandlesToDelete, BulletDeleteCount);
                    }

                    for(int i = BulletCount; i < MaxBullets; i++)
                    {
                        if (BulletObjects[i] != null)
                        {
                            BulletObjects[i].SetActive(false);
                        }
                    }
                }
            }
            else
            {
                Debug.LogError("Failed to get bullets.");
            }
       }
    }
}
