// C# bindings for the MKS BulletML Interpreter
// v0.9.1

using System;
using System.Runtime.InteropServices;

namespace MksBulletmlInterpreter
{
	public enum BulletmlAttributeType
	{
		None = 0,
		Vertical = 1,
		Horizontal = 2,
	}

	public enum MksbmliErrorCode
	{
		NoError = 0,
		UnspecifiedError = -1,
		XmlFileLoadError = -100,
		TooManyXmlFiles = -101,
		XmlFilenameTooLong = -102,
		InvalidXmlFilename = -103,
		XmlParseError = -104,
		TooManyBullets = -105,
		AlreadyPlaying = -120,
		NotPlaying = -121,
		InvalidHandle = -130,
		InvalidParameter = -131,
	}

	[StructLayout(LayoutKind.Sequential)]
	public struct Vector2D
	{
		public float x;
		public float y;
	}

	[StructLayout(LayoutKind.Sequential)]
	public struct VirtualBullet
	{
		public uint handle;
		public int action_id;
		public BulletmlAttributeType bulletml_attribute;
		public Vector2D position;
		public float angle_degrees;
		public float speed;
		public Vector2D velocity;
		public Vector2D acceleration_vector;
		public uint acceleration_frames;
		public float changing_direction_interval;
		public uint changing_direction_frames;
		public float changing_speed_interval;
		public uint changing_speed_frames;
	}

	public static class MksBulletmlInterpreterNative
	{
		public const string LibraryName = "mks-bulletml-interpreter";

		[DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl, ExactSpelling = true)]
		public static extern int mksbmli_init();

		[DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl, ExactSpelling = true)]
		public static extern int mksbmli_shutdown();

		[DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl, ExactSpelling = true)]
		public static extern void mksbmli_get_version(out int major, out int minor, out int patch);

		[DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl, ExactSpelling = true)]
		public static extern int mksbmli_set_random_seed(int seed);

		[DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl, ExactSpelling = true, CharSet = CharSet.Ansi)]
		public static extern int mksbmli_load_xml([MarshalAs(UnmanagedType.LPStr)] string xml_filename, out uint handle);

		[DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl, ExactSpelling = true)]
		public static extern int mksbmli_unload_xml(uint handle);

		[DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl, ExactSpelling = true)]
		public static extern int mksbmli_count_xmls();

		[DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl, ExactSpelling = true)]
		public static extern void mksbmli_start_playback(uint handle);

		[DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl, ExactSpelling = true)]
		public static extern void mksbmli_stop_playback(uint handle);

		[DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl, ExactSpelling = true)]
		public static extern void mksbmli_next_frame(uint handle);

		[DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl, ExactSpelling = true)]
		public static extern int mksbmli_get_bullets(uint handle, int max_bullets, [Out] IntPtr[] bullets, out int nos_bullets);

		[DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl, ExactSpelling = true)]
		public static extern int mksbmli_delete_bullets(uint handle, [In] uint[] bullet_handles, int nos_bullet_handles);

		[DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl, ExactSpelling = true)]
		public static extern int mksbmli_clear_bullets(uint handle);

		[DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl, ExactSpelling = true)]
		public static extern int mksbmli_set_emitter_center(uint handle, float emitter_position_x, float emitter_position_y);

		[DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl, ExactSpelling = true)]
		public static extern int mksbmli_set_player_position(uint handle, float player_position_x, float player_position_y);

		[DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl, ExactSpelling = true)]
		public static extern int mksbmli_set_rank(uint handle, float rank);

		public static VirtualBullet ReadBullet(IntPtr bulletPointer)
		{
			return Marshal.PtrToStructure<VirtualBullet>(bulletPointer);
		}
	}
}
