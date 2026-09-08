using Godot;
using MksBulletmlInterpreter;
using System;
using System.Diagnostics;

public partial class GodotDemoHandler : Node
{
	private bool _isInitialized = false;

	[Export]
	public string BulletMLFilepath = "res://BulletML_Files/basic_bml.xml";

	private bool _bmlFileLoaded = false;
	private uint _playbackHandle;

	Vector2 _windowSize;
	Vector2 _windowHalfSize;
	private float _scaleFactor = 30.0f;

	private bool _isPlaying = false;

	private const int _maxBullets = 5000;
	private VirtualBullet[] _bullets = new VirtualBullet[_maxBullets];
	private IntPtr[] _bulletPtrs = new IntPtr[_maxBullets];
	private uint[] _bulletHandlesToDelete = new uint[_maxBullets];

	public override void _Ready()
	{
		_windowSize = GetViewport().GetVisibleRect().Size;
		_windowHalfSize = new Vector2(_windowSize.X / 2f, _windowSize.Y / 2f);

		GD.Print("Starting up MksBulletmlInterpreter");

		if(MksBulletmlInterpreterNative.mksbmli_init() == (int)MksbmliErrorCode.NoError)
		{
			GD.Print("MksBulletmlInterpreter initialized successfully.");

			int major, minor, patch;
			MksBulletmlInterpreterNative.mksbmli_get_version(out major, out minor, out patch);
			GD.Print($"MksBulletmlInterpreter version: {major}.{minor}.{patch}");

			_isInitialized = true;

			MksBulletmlInterpreterNative.mksbmli_set_random_seed(1337);

			_bmlFileLoaded = MksBulletmlInterpreterNative.mksbmli_load_xml(BulletMLFilepath, out _playbackHandle) == (int)MksbmliErrorCode.NoError;
			if (_bmlFileLoaded)
			{
				GD.Print("BulletML file loaded successfully: " + BulletMLFilepath);

				GD.Print("BulletMLPlaybackHandle: " + _playbackHandle);

				MksBulletmlInterpreterNative.mksbmli_start_playback(_playbackHandle);

				MksBulletmlInterpreterNative.mksbmli_set_rank(_playbackHandle, 0.5f);

				//MksBulletmlInterpreterNative.mksbmli_set_emitter_center(_playbackHandle, 0, 0); // Relative to window center, the Unity Sprite is (0/0) at the center of the window      
				//MksBulletmlInterpreterNative.mksbmli_set_player_position(
				//    _playbackHandle,
				//    _windowHalfSize.X * _scaleFactor,
				//    -(_windowHalfSize.Y + (_windowHalfSize.Y / 2f)) * _scaleFactor);

				MksBulletmlInterpreterNative.mksbmli_set_emitter_center(_playbackHandle, 120, 160);
				MksBulletmlInterpreterNative.mksbmli_set_player_position(_playbackHandle, 120, -200);

				_isPlaying = true;

				GD.Print("Started playback");
			}
			else
			{
				GD.PrintErr("Failed to load BulletML file: " + BulletMLFilepath);
			}
		}
		else
		{
			GD.PrintErr("Failed to initialize MksBulletmlInterpreter.");

			_isInitialized = false;
		}
	}

	public override void _Notification(int what)
	{
		if (what == NotificationWMCloseRequest)
		{
			GD.Print("App is closing.");

			if (_isInitialized)
			{
				GD.Print("Shutting down MksBulletmlInterpreter");

				if (_isPlaying)
				{
					// 1. Stop playback if it is currently playing
					MksBulletmlInterpreterNative.mksbmli_stop_playback(_playbackHandle);

					GD.Print("Stopped playback");

					// 2. Clear all bullets
					MksBulletmlInterpreterNative.mksbmli_clear_bullets(_playbackHandle);
				}


				MksBulletmlInterpreterNative.mksbmli_shutdown();


				_isInitialized = false;
			}

			GetTree().Quit(); // you must actually quit yourself now
		}
	}

	public override void _PhysicsProcess(double delta)
	{
		if (_isPlaying)
		{
			// 1. Calculate the next frame for the BulletML playback
			MksBulletmlInterpreterNative.mksbmli_next_frame(_playbackHandle);

			// 2. Retrieve the current bullets from the BulletML playback
			int bulletCount = 0;
			if (MksBulletmlInterpreterNative.mksbmli_get_bullets(_playbackHandle, _maxBullets, _bulletPtrs, out bulletCount) == (int)MksbmliErrorCode.NoError)
			{
				GD.Print("Nos Bullets: " + bulletCount);
			}
			else
			{
				GD.PrintErr("Failed to get bullets from BulletML playback.");
			}
		}
	}
}
