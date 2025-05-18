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
};