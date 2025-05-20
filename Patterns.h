class Patterns
{
public:
	static inline const std::string WorldPattern = "\x48\x8B\x3D????\x49\x8B\xB6";
	static inline const std::string WorldMask = "xxx????xxx";

	static inline const std::string GodBitsPattern = "\x89\x86????\xEB?\x83\xC8?\x89\x86";
	static inline const std::string GodBitsMask = "xx????x?xx?xx";

	static inline const std::string HealthPattern = "\xF3\x0F\x11\x96????\x80\x7E";
	static inline const std::string HealthMask = "xxxx????xx";

	static inline const std::string AmmoModifierPattern = "\xF6\x47??\x75?\x0F\xB7\x77";
	static inline const std::string AmmoModifierMask = "xx??x?xxx";

	static inline const std::string WeaponInventoryPattern = "\x4C\x8B\xB2????\x4D\x85\xF6\x0F\x84????\x45\x8B\x7C\x24";
	static inline const std::string WeaponInventoryMask = "xxx????xxxxx????xxxx";

	static inline const std::string PlayerInfoPattern = "\x48\x83\xB8?????\x75?\x80\x3D";
	static inline const std::string PlayerInfoMask = "xxx?????x?xx";

	static inline const std::string WantedLevelPattern = "\x83\xB8?????\x0F\x8E????\x8B\x05";
	static inline const std::string WantedLevelMask = "xx?????xx????xx";

	static inline const std::string VehicleGodBitsPattern = "\xF6\x80?????\x74?\x8B\x05????\x89\x86????\x48\x8D\x0D";
	static inline const std::string VehicleGodBitsMask = "xx?????x?xx????xx????xxx";

	static inline const std::string NavigationPattern = "\x48\x8B\x48?\x48\x85\xC9\x0F\x84????\x0F\xB7\x71";
	static inline const std::string NavigationMask = "xxx?xxxxx????xxx";

	static inline const std::string PlayerPositionPattern = "\xF2\x0F\x10\x40?\xF2\x0F\x11\x44\x24?\xF3\x0F\x10\x40?\xF3\x0F\x11\x44\x24?\x48\x89\xF1";
	static inline const std::string PlayerPositionMask = "xxxx?xxxxx?xxxx?xxxxx?xxx";

	static inline const std::string BlipPattern = "\x4C\x8D\x3D????\x49\x8B\x34\xC7";
	static inline const std::string BlipMask = "xxx????xxxx";

	static inline const std::string BlipPositionPattern = "\x0F\x29\x78?\xEB?\x0F\x13\x78";
	static inline const std::string BlipPositionMask = "xxx?x?xxx";

	static inline const std::string BlipIDPattern = "\x41\x0F\x28\x44\x24?\x41\x0F\x28\x4C\x24?\x41\x0F\x29\x4E";
	static inline const std::string BlipIDMask = "xxxxx?xxxxx?xxxx";

	static inline const std::string WeaponManagerPattern = "\x49\x83\xBD?????\x74?\x48\x89\xC5";
	static inline const std::string WeaponManagerMask = "xxx?????x?xxx";

	static inline const std::string WeaponInfoPattern = "\x48\x3B\x4F?\x48\x0F\x44\xC1";
	static inline const std::string WeaponInfoMask = "xxx?xxxx";

	static inline const std::string WeaponNamePattern = "\x81\x78?????\x0F\x85????\x4C\x8B\x35";
	static inline const std::string WeaponNameMask = "xx?????xx????xxx";

	static inline const std::string WeaponImpactTypePattern = "\x83\x7D??\x74?\x8B\x05????\x39\x45";
	static inline const std::string WeaponImpactTypeMask = "xx??x?xx????xx";

	static inline const std::string WeaponDamagePattern = "\xF3\x0F\x10\x89????\x0F\x57\xC0\x3D";
	static inline const std::string WeaponDamageMask = "xxxx????xxxx";

	static inline const std::string WeaponPenetrationPattern = "\xF3\x41\x0F\x10\x86????\xF3\x0F\x11\x43";
	static inline const std::string WeaponPenetrationMask = "xxxxx????xxxx";

	static inline const std::string WeaponReloadMultiplerPattern = "\xF3\x0F\x10\x80????\x48\x81\xC6";
	static inline const std::string WeaponReloadMultiplerMask = "xxxx????xxx";

	static inline const std::string WeaponFireRatePattern = "\xF3\x0F\x10\x87????\xEB?\x81\x7F";
	static inline const std::string WeaponFireRateMask = "xxxx????x?xx";

	static inline const std::string WeaponRecoilAmplitudePattern = "\xBD????\x49\x03\x6C\x24";
	static inline const std::string WeaponRecoilAmplitudeMask = "x????xxxx";
};