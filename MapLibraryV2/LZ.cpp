
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING

#include "LZ.h"

namespace MapLibraryV2
{
    std::string LZ::ToString(const std::wstring& str)
    {
        std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
        return converter.to_bytes(str);
    }

    std::wstring LZ::Compress(const std::string& text,
        const int bit_count, const std::function<wchar_t(int)> converter)
    {
        if (text.empty()) return std::wstring();
        std::wstring curr_text, prev_text, curr_char;
        std::unordered_map<std::wstring, int> dictionary;
        std::unordered_map<std::wstring, bool> to_create;
        int enlarge_in = 2, dict_size = 3, num_bits = 2;
        int data_value = 0, data_position = 0;
        std::vector<wchar_t> data;

        // reserve space for output data
        data.reserve(text.size() * 2);

        // function to write bits to the output data
        auto write_bits = [&](int val, int num_bits)
        {
            for (int i = 0; i < num_bits; i++)
            {
                int temp = data_value << 1;
                data_value = temp | (val & 1);
                if (data_position == bit_count - 1)
                {
                    data.push_back(converter(data_value));
                    data_value = data_position = 0;
                }
                else data_position++;
                val = val >> 1;
            }
        };

        // function to enlarge the size of dictionary
        auto balance_size = [&]()
        {
            if (--enlarge_in == 0)
                enlarge_in = 1 << num_bits++;
        };

        // function to add dictionary entries
        auto add_entries = [&]()
        {
            if (to_create.count(prev_text) > 0)
            {
                write_bits(0, num_bits);
                write_bits(prev_text[0], 8);
                balance_size();
                to_create.erase(prev_text);
            }
            else
            {
                int value = dictionary[prev_text];
                write_bits(value, num_bits);
            }
        };

        // loop through the input text and compress it
        for (const wchar_t& ch : text)
        {
            curr_char = ch;
            if (dictionary.count(curr_char) == 0)
            {
                dictionary[curr_char] = dict_size++;
                to_create[curr_char] = true;
            }
            curr_text = prev_text + curr_char;
            if (dictionary.count(curr_text) > 0)
            {
                prev_text = curr_text;
            }
            else
            {
                add_entries(); balance_size();
                dictionary[curr_text] = dict_size++;
                prev_text = curr_char;
            }
        }

        // write the end-of-data marker to the output
        if (!prev_text.empty()) add_entries();
        write_bits(2, num_bits);

        // write any remaining bits to the output
        while (data_position != 0)
        {
            data_value = data_value << 1;
            if (data_position == bit_count - 1)
            {
                data.push_back(converter(data_value));
                data_value = data_position = 0;
            }
            else data_position++;
        }

        // return the compressed data as a string
        return std::wstring(data.begin(), data.end());
    }

    std::string LZ::Decompress(const size_t length,
        const int reset_value, const std::function<int(int)> converter)
    {
        if (length == 0) return "";
        std::string result, entry;
        std::unordered_map<int, std::string> dictionary;
        int enlarge_in = 4, dict_size = 4, num_bits = 3;
        int data_position = reset_value, data_index = 1;
        for (int index = 0; index < 3; ++index)
            dictionary[index] = static_cast<char>(index);
        int data_value = converter(0);

        // function to read binary data from the input stream
        auto read_bits = [&](int exponent)
        {
            int bits = 0, power = 1;
            int maxpower = 1 << exponent;
            while (power != maxpower)
            {
                int resb = data_value & data_position;
                data_position = data_position >> 1;
                if (data_position == 0)
                {
                    data_position = reset_value;
                    data_value = converter(data_index++);
                }
                bits |= (resb > 0 ? power : 0);
                power <<= 1;
            }
            return bits;
        };

        // function to adjust the size of dictionary
        auto balance_size = [&]()
        {
            if (enlarge_in == 0)
                enlarge_in = 1 << num_bits++;
        };

        // function to add dictionary entries
        auto add_entries = [&](const std::string& ch)
        {
            dictionary[dict_size++] = ch;
            enlarge_in--;
        };

        // initialise the dictionary and output
        int bits = read_bits(2);
        if (bits == 2) return "";
        bits = read_bits((bits + 1) * 8);
        char ch = bits;
        dictionary[3] = ch;
        std::string text(1, ch);
        result += ch;

        // read input data and build a dictionary
        while (true)
        {
            bits = read_bits(num_bits);
            int count = bits;
            if (count == 2) return result;
            else if (count == 0 || count == 1)
            {
                bits = read_bits((count + 1) * 8);
                char temp = static_cast<char>(bits);
                add_entries(std::string(1, temp));
                count = dict_size - 1;
            }
            balance_size();
            entry = dictionary[count];
            if (entry.empty())
                entry = text + text[0];
            result += entry;
            ch = entry[0];
            add_entries(text + ch);
            balance_size();
            text = entry;
        }
    }

    // All public functions provided for use

    std::string LZ::CompressToBase64(const std::string& text)
    {
        std::wstring result = Compress(text, 6,
            [](int i) { return base64_alpha[i]; });
        std::string output = ToString(result);
        size_t div = output.size() % 4;
        auto ending = std::string(4 - div, '=');
        return output + (div > 0 ? ending : "");
    }

    std::vector<Char> LZ::CompressToBytes(const std::string& text)
    {
        auto result = std::vector<Char>();
        std::wstring output = Compress(text, 16,
            [](int i) { return (wchar_t)i; });
        size_t size = output.size();
        result.assign(size * 2, 0);
        for (size_t i = 0; i < size; i++)
        {
            int value = (int)output[i];
            result[i * 2] = (Char)(value >> 8);
            result[i * 2 + 1] = (Char)(value % 256);
        }
        return result;
    }

    std::string LZ::CompressToEncodedURI(const std::string& text)
    {
        return ToString(Compress(text, 6, [](int i)
            { return safe_uri_alpha[i]; }));
    }

    std::string LZ::DecompressFromBase64(const std::string& text)
    {
        return Decompress(text.size(), 32, [&](int i) {
            return base64_table.at(text.at(i)); });
    }

    std::string LZ::DecompressFromBytes(const std::vector<Char>& bytes)
    {
        std::wstring output;
        size_t size = bytes.size() / 2;
        for (size_t i = 0; i < size; i++)
        {
            int v1 = bytes[i * 2] * 256;
            int v2 = bytes[i * 2 + 1];
            output += (wchar_t)(v1 + v2);
        }
        return Decompress(output.size(), 32768,
            [&](int i) { return output[i]; });
    }

    std::string LZ::DecompressFromEncodedURI(const std::string& text)
    {
        return Decompress(text.size(), 32, [&](int i) {
            return safe_uri_table.at(text.at(i)); });
    }
}
