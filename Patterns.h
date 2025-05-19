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
};