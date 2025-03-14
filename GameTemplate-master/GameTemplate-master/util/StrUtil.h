#pragma once
#include <string>
#include <vector>

// •¶š—ñ•Ö—˜ŠÖ”
namespace StrUtil
{
	// •¶š—ñ‚ğ1•¶š‚¸‚Â•ªŠ„‚µ‚½vector‚É‚·‚é
	// 3ƒoƒCƒgˆÈã‚Ì•¶š‚Í”ñ‘Î‰	ÅŒã‚Ì\0‚Íæ‚èœ‚­
	std::vector<std::string> splitString(const std::string str);

	// input‚µ‚½•¶š—ñ‚ğdelimiter‚Å•ªŠ„‚µ‚Ä
	// •ªŠ„Œã‚Ì•¶š—ñ‚ğ•Ô‚·
	std::vector<std::string> split(std::string& input, char delimiter);
}
