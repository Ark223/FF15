
#include <array>
#include <codecvt>
#include <functional>
#include <locale>
#include <unordered_map>
#include <vector>

namespace MapLibraryV2
{
    using Char = unsigned char;

    const std::string base64_alpha = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";
    const std::string safe_uri_alpha = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+-$";

    const std::array<char, 66> base64_table = { "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=" };
    const std::array<char, 66> safe_uri_table = { "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+-$" };

    class LZ
    {
        private:
            static std::string ToString(const std::wstring& text);
            static std::wstring Compress(const std::string& text, const int bit_count, const std::function<wchar_t(int)> converter);
            static std::string Decompress(const size_t length, const int reset_value, const std::function<int(int)> converter);

        public:
            static std::string CompressToBase64(const std::string& text);
            static std::vector<Char> CompressToBytes(const std::string& text);
            static std::string CompressToEncodedURI(const std::string& text);
            static std::string DecompressFromBase64(const std::string& text);
            static std::string DecompressFromBytes(const std::vector<Char>& bytes);
            static std::string DecompressFromEncodedURI(const std::string& text);
    };
}
