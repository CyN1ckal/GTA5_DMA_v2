class Patterns
{
public:
	static inline const std::string WorldPattern = "\x48\x8B\x3D????\x49\x8B\xB6";
	static inline const std::string WorldMask = "xxx????xxx";

	static inline const std::string GodBitsPattern = "\x89\x86????\xEB?\x83\xC8?\x89\x86";
	static inline const std::string GodBitsMask = "xx????x?xx?xx";

	static inline const std::string HealthPattern = "\xF3\x0F\x11\x96????\x80\x7E";
	static inline const std::string HealthMask = "xxxx????xx";
};