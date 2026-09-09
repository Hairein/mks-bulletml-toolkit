using Godot;
using MksBulletmlInterpreter;
using System;
using System.Diagnostics;
using System.Runtime.InteropServices;

public partial class GodotDemoHandler : Node
{
	private bool _isInitialized = false;

	// Choose 1 XML file to use for the demo. Uncomment the desired file and comment out the others.
	//[Export]
	//public string BulletMLFilepath = "res://BulletML_Files/basic_bml.xml";
	//[Export]
	//public string BulletMLFilepath = "res://BulletML_Files/aimed_single_bml.xml";
	[Export]
	public string BulletMLFilepath = "res://BulletML_Files/aimed_bml.xml";
		
	[Export]
	public string BulletTexture = "res://Textures/8x8_default_bullet.png";
	private bool _bmlFileLoaded = false;
	private uint _playbackHandle;

	Vector2 _windowSize;
	Vector2 _windowHalfSize;

	private bool _isPlaying = false;

	private const int _maxBullets = 5000;
	private VirtualBullet[] _bullets = new VirtualBullet[_maxBullets];
	private IntPtr[] _bulletPtrs = new IntPtr[_maxBullets];
	private uint[] _bulletHandlesToDelete = new uint[_maxBullets];
	Sprite2D[] _bulletObjects = new Sprite2D[_maxBullets];

	private void PreInstantiateBullets()
	{
		for (int i = 0; i < _maxBullets; i++)
		{
			_bullets[i] = new VirtualBullet();
			_bulletPtrs[i] = Marshal.AllocHGlobal(Marshal.SizeOf(typeof(VirtualBullet)));
			_bulletHandlesToDelete[i] = 0;
			_bulletObjects[i] = new Sprite2D();
			_bulletObjects[i].Texture = GD.Load<Texture2D>(BulletTexture);
			_bulletObjects[i].Visible = false;
			AddChild(_bulletObjects[i]);
		}
	}

	private void UpdatePlayerPositionFromMouse()
	{
		Vector2 mousePosition = GetViewport().GetMousePosition();
		float playerX = mousePosition.X - _windowHalfSize.X;
		float playerY = mousePosition.Y - _windowHalfSize.Y;

		MksBulletmlInterpreterNative.mksbmli_set_player_position(
			_playbackHandle,
			playerX,
			playerY);	
	}

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

			string globalPath = ProjectSettings.GlobalizePath(BulletMLFilepath); // Get absolute filepath from resource
			_bmlFileLoaded = MksBulletmlInterpreterNative.mksbmli_load_xml(globalPath, out _playbackHandle) == (int)MksbmliErrorCode.NoError;
			if (_bmlFileLoaded)
			{
				GD.Print("BulletML file loaded successfully: " + BulletMLFilepath);

				GD.Print("BulletMLPlaybackHandle: " + _playbackHandle);

				MksBulletmlInterpreterNative.mksbmli_start_playback(_playbackHandle);

				MksBulletmlInterpreterNative.mksbmli_set_rank(_playbackHandle, 0.5f);

				MksBulletmlInterpreterNative.mksbmli_set_emitter_center(_playbackHandle, 0, 0); // Relative to window center, the Godot Sprite is offset to the center of the viewport
				MksBulletmlInterpreterNative.mksbmli_set_player_position(
					_playbackHandle,
					0f,
					(_windowHalfSize.Y + (_windowHalfSize.Y / 2f)));

				PreInstantiateBullets();

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
					_isPlaying = false;

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
			// Update the player position based on the mouse position
			UpdatePlayerPositionFromMouse();

			// 1. Calculate the next frame for the BulletML playback
			MksBulletmlInterpreterNative.mksbmli_next_frame(_playbackHandle);

			// 2. Retrieve the current bullets from the BulletML playback
			int bulletCount = 0;
			if (MksBulletmlInterpreterNative.mksbmli_get_bullets(_playbackHandle, _maxBullets, _bulletPtrs, out bulletCount) == (int)MksbmliErrorCode.NoError)
			{
				//GD.Print("Nos Bullets: " + bulletCount);
				int bulletDeleteCount = 0;

				for (int i = 0; i < bulletCount; i++)
				{
					_bullets[i] = MksBulletmlInterpreterNative.ReadBullet(_bulletPtrs[i]);

					float bulletX = _bullets[i].position.x; 
					float bulletY = _bullets[i].position.y;
					bool isOutsideWindow = bulletX < -_windowHalfSize.X || bulletX > _windowHalfSize.X || bulletY < -_windowHalfSize.Y || bulletY > _windowHalfSize.Y;

					_bulletObjects[i].Visible = true;
					_bulletObjects[i].Position = new Vector2(
						_windowHalfSize.X + bulletX,
						_windowHalfSize.Y - bulletY
					);

					if (isOutsideWindow)
					{
						_bulletHandlesToDelete[bulletDeleteCount++] = _bullets[i].handle;

						if (_bulletObjects[i] != null)
						{
							_bulletObjects[i].Visible = false;
						}
					}

				}

				// 3. Delete bullets that are outside the window
				if (bulletDeleteCount > 0)
				{
					MksBulletmlInterpreterNative.mksbmli_delete_bullets(_playbackHandle, _bulletHandlesToDelete, bulletDeleteCount);
				}

				for (int i = bulletCount; i < _maxBullets; i++)
				{
					_bulletObjects[i].Visible = false;
				}
			}
			else
			{
				GD.PrintErr("Failed to get bullets from BulletML playback.");
			}
		}
	}
}
