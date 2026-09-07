using System;
using UnityEngine;
using MksBulletmlInterpreter;

public class MksBmlToolkitHandler : MonoBehaviour
{
    bool MksBmlTkInitialized = false;

    int Major, Minor, Patch;

    //string BulletMLFilepath = "Assets/BulletML_Files/aimed_bml.xml";
    string BulletMLFilepath = @"F:\GitHub\mks-bulletml-toolkit\bulletml_files\basic_bml.xml";
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

    int BulletCount = 0;
    
    // Start is called once before the first execution of Update after the MonoBehaviour is created
    void Start()
    {
        Debug.Log("Initializing MksBulletmlInterpreter...");

        if(MksBulletmlInterpreterNative.mksbmli_init() == (int)MksbmliErrorCode.NoError)
        {
            Debug.Log("MksBulletmlInterpreter initialized successfully.");
            
            MksBmlTkInitialized = true;

            MksBulletmlInterpreterNative.mksbmli_get_version(out Major, out Minor, out Patch);  
            
            Debug.Log("MksBulletmlInterpreter version: " + Major + "." + Minor + "." + Patch);

            BmlFileLoaded = MksBulletmlInterpreterNative.mksbmli_load_xml(BulletMLFilepath, out BulletMLPlaybackHandle) == (int)MksbmliErrorCode.NoError;            
            if(BmlFileLoaded)
            {
                Debug.Log("BulletML file loaded successfully: " + BulletMLFilepath);
                
                Debug.Log("BulletMLPlaybackHandle: " + BulletMLPlaybackHandle);
               
                MksBulletmlInterpreterNative.mksbmli_set_random_seed(1337);
                MksBulletmlInterpreterNative.mksbmli_set_rank(BulletMLPlaybackHandle, 0.5f);
                MksBulletmlInterpreterNative.mksbmli_set_emitter_center(BulletMLPlaybackHandle, WindowCenterX, WindowCenterY);
                MksBulletmlInterpreterNative.mksbmli_set_player_position(BulletMLPlaybackHandle, WindowCenterX, WindowCenterY + (WindowCenterY / 2));

                MksBulletmlInterpreterNative.mksbmli_start_playback(BulletMLPlaybackHandle);
                
                MksBulletmlInterpreterNative.mksbmli_set_random_seed(1337);
                MksBulletmlInterpreterNative.mksbmli_set_rank(BulletMLPlaybackHandle, 0.5f);
                MksBulletmlInterpreterNative.mksbmli_set_emitter_center(BulletMLPlaybackHandle, WindowCenterX, WindowCenterY);
                MksBulletmlInterpreterNative.mksbmli_set_player_position(BulletMLPlaybackHandle, WindowCenterX, WindowCenterY + (WindowCenterY / 2));

                IsPlaying = true;
                
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
                MksBulletmlInterpreterNative.mksbmli_stop_playback(BulletMLPlaybackHandle);
                
                Debug.Log("Stopped playback");
                
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
           MksBulletmlInterpreterNative.mksbmli_next_frame(BulletMLPlaybackHandle);

            if(MksBulletmlInterpreterNative.mksbmli_get_bullets(BulletMLPlaybackHandle, MaxBullets, BulletPtrs, out BulletCount) == (int)MksbmliErrorCode.NoError)
            {
                Debug.Log("NosBullets: " + BulletCount);
                
                if(BulletCount > 0)
                {
                    for(int i = 0; i < BulletCount; i++)
                    {
                        Bullets[i] = MksBulletmlInterpreterNative.ReadBullet(BulletPtrs[i]);

                        // Example access after marshaling:
                        // Debug.Log($"Bullet {i}: handle={Bullets[i].handle}, pos=({Bullets[i].position.x}, {Bullets[i].position.y})");
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
