
--[[

    ,--.   ,-------.      ,--.                    
    |  |   `--.   /,-----.|  |   ,--.,--. ,--,--. 
    |  |     /   / '-----'|  |   |  ||  |' ,-.  | 
    |  '--. /   `--.      |  '--.'  ''  '\ '-'  | 
    `-----'`-------'      `-----' `----'  `--`--' 

    LZ-based compression algorithm for LuaJIT scripting language

    Original creator: pieroxy
    Repository: https://github.com/pieroxy/lz-string
    Ported and optimized by: Uncle Ark & solstice

]]

local bit = require "bit"
local base64_alpha = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/="
local safe_uri_alpha = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+-$"

local base64_table = {
    ['A'] = 0, ['B'] = 1, ['C'] = 2, ['D'] = 3, ['E'] = 4,
    ['F'] = 5, ['G'] = 6, ['H'] = 7, ['I'] = 8, ['J'] = 9,
    ['K'] = 10, ['L'] = 11, ['M'] = 12, ['N'] = 13, ['O'] = 14,
    ['P'] = 15, ['Q'] = 16, ['R'] = 17, ['S'] = 18, ['T'] = 19,
    ['U'] = 20, ['V'] = 21, ['W'] = 22, ['X'] = 23, ['Y'] = 24,
    ['Z'] = 25, ['a'] = 26, ['b'] = 27, ['c'] = 28, ['d'] = 29,
    ['e'] = 30, ['f'] = 31, ['g'] = 32, ['h'] = 33, ['i'] = 34,
    ['j'] = 35, ['k'] = 36, ['l'] = 37, ['m'] = 38, ['n'] = 39,
    ['o'] = 40, ['p'] = 41, ['q'] = 42, ['r'] = 43, ['s'] = 44,
    ['t'] = 45, ['u'] = 46, ['v'] = 47, ['w'] = 48, ['x'] = 49,
    ['y'] = 50, ['z'] = 51, ['0'] = 52, ['1'] = 53, ['2'] = 54,
    ['3'] = 55, ['4'] = 56, ['5'] = 57, ['6'] = 58, ['7'] = 59,
    ['8'] = 60, ['9'] = 61, ['+'] = 62, ['/'] = 63, ['='] = 64
}

local safe_uri_table = {
    ['A'] = 0, ['B'] = 1, ['C'] = 2, ['D'] = 3, ['E'] = 4,
    ['F'] = 5, ['G'] = 6, ['H'] = 7, ['I'] = 8, ['J'] = 9,
    ['K'] = 10, ['L'] = 11, ['M'] = 12, ['N'] = 13, ['O'] = 14,
    ['P'] = 15, ['Q'] = 16, ['R'] = 17, ['S'] = 18, ['T'] = 19,
    ['U'] = 20, ['V'] = 21, ['W'] = 22, ['X'] = 23, ['Y'] = 24,
    ['Z'] = 25, ['a'] = 26, ['b'] = 27, ['c'] = 28, ['d'] = 29,
    ['e'] = 30, ['f'] = 31, ['g'] = 32, ['h'] = 33, ['i'] = 34,
    ['j'] = 35, ['k'] = 36, ['l'] = 37, ['m'] = 38, ['n'] = 39,
    ['o'] = 40, ['p'] = 41, ['q'] = 42, ['r'] = 43, ['s'] = 44,
    ['t'] = 45, ['u'] = 46, ['v'] = 47, ['w'] = 48, ['x'] = 49,
    ['y'] = 50, ['z'] = 51, ['0'] = 52, ['1'] = 53, ['2'] = 54,
    ['3'] = 55, ['4'] = 56, ['5'] = 57, ['6'] = 58, ['7'] = 59,
    ['8'] = 60, ['9'] = 61, ['+'] = 62, ['-'] = 63, ['$'] = 64
}

-- Implementation of bitwise operations

local function LSHIFT(n, b)
    return bit.lshift(n, b)
end

local function RSHIFT(n, b)
    return bit.rshift(n, b)
end

local function AND(a, b)
    return bit.band(a, b)
end

local function OR(a, b)
    return bit.bor(a, b)
end

-- Utility functions for working with Unicode strings

local Unicode = {}

function Unicode.ToArray(str)
    local result, index = {}, 1
    local pattern = ".[\128-\191]*"
    for char in str:gmatch(pattern) do
        result[index] = char
        index = index + 1
    end
    result[index] = " "
    return result
end

function Unicode.ToChar(n)
    if n < 0x80 then
        return string.char(n)
    elseif n < 0x800 then
        local b1 = 0xC0 + RSHIFT(n, 6)
        local b2 = 0x80 + AND(n, 0x3F)
        return string.char(b1, b2)
    elseif n < 0x10000 then
        local b1 = 0xE0 + RSHIFT(n, 12)
        local b2 = 0x80 + AND(RSHIFT(n, 6), 0x3F)
        local b3 = 0x80 + AND(n, 0x3F)
        return string.char(b1, b2, b3)
    elseif n < 0x110000 then
        local b1 = 0xF0 + RSHIFT(n, 18)
        local b2 = 0x80 + AND(RSHIFT(n, 12), 0x3F)
        local b3 = 0x80 + AND(RSHIFT(n, 6), 0x3F)
        local b4 = 0x80 + AND(n, 0x3F)
        return string.char(b1, b2, b3, b4)
    end
    return ""
end

function Unicode.ToInt(c)
    local b1, b2, b3, b4 = c:byte(1, 4)
    if b1 < 0x80 then return b1 end
    if b1 < 0xE0 then
        local d1 = LSHIFT(AND(b1, 0x1F), 6)
        return d1 + AND(b2, 0x3F)
    elseif b1 < 0xF0 then
        local d1 = LSHIFT(AND(b1, 0x1F), 12)
        local d2 = LSHIFT(AND(b2, 0x3F), 6)
        return d1 + d2 + AND(b3, 0x3F)
    elseif b1 < 0xF8 then
        local d1 = LSHIFT(AND(b1, 0x07), 18)
        local d2 = LSHIFT(AND(b2, 0x3F), 12)
        local d3 = LSHIFT(AND(b3, 0x3F), 6)
        return d1 + d2 + d3 + AND(b4, 0x3F)
    end
    return -1
end

-- LZ-based loseless compression algorithm

local function Compress(text, bit_count, converter)
    local dictionary, to_create = {}, {}
    local curr_char, curr_text, prev_text = "", "", ""
    local enlarge_in, dict_size, num_bits = 2, 3, 2
    local data, data_value, data_position = {}, 0, 0

    -- function to write bits to the output data
    local function write_bits(val, num_bits)
        for i = 1, num_bits do
            local temp = LSHIFT(data_value, 1)
            data_value = OR(temp, AND(val, 1))
            if data_position == bit_count - 1 then
                data[#data + 1] = converter(data_value)
                data_value, data_position = 0, 0
            else
                data_position = data_position + 1
            end
            val = RSHIFT(val, 1)
        end
    end

    -- function to enlarge the size of dictionary
    local function balance_size()
        enlarge_in = enlarge_in - 1
        if enlarge_in == 0 then
            enlarge_in = 2 ^ num_bits
            num_bits = num_bits + 1
        end
    end

    -- function to add dictionary entries
    local function add_entries()
        if to_create[prev_text] then
            write_bits(0, num_bits)
            value = string.byte(prev_text, 1)
            write_bits(value, 8)
            balance_size()
            to_create[prev_text] = nil
        else
            value = dictionary[prev_text]
            write_bits(value, num_bits)
        end
    end

    -- loop through the input text and compress it
    for index = 1, #text do
        curr_char = text:sub(index, index)
        if not dictionary[curr_char] then
            dictionary[curr_char] = dict_size
            dict_size = dict_size + 1
            to_create[curr_char] = true
        end
        curr_text = prev_text .. curr_char
        if dictionary[curr_text] then
            prev_text = curr_text
        else
            add_entries(); balance_size()
            dictionary[curr_text] = dict_size
            dict_size = dict_size + 1
            prev_text = curr_char
        end
    end

    -- write the end-of-data marker to the output
    if prev_text ~= "" then add_entries() end
    write_bits(2, num_bits)

    -- write any remaining bits to the output
    while data_position ~= 0 do
        data_value = LSHIFT(data_value, 1)
        if data_position == bit_count - 1 then
            table.insert(data, converter(data_value))
            data_value, data_position = 0, 0
        else
            data_position = data_position + 1
        end
    end

    -- return the compressed data as a string of bits
    return table.concat(data)
end

local function Decompress(length, reset_value, converter)
    local dictionary, result, entry = {}, {}, ""
    local enlarge_in, dict_size, num_bits = 4, 4, 3
    local data_position, data_index = reset_value, 1
    for i = 0, 2 do dictionary[i] = string.char(i) end
    local data_value = converter(0)

    -- function to read binary data from the input stream
    local function read_bits(exponent)
        local bits, power = 0, 1
        local maxpower = 2 ^ exponent
        while power ~= maxpower do
            local resb = AND(data_value, data_position)
            data_position = RSHIFT(data_position, 1)
            if data_position == 0 then
                data_position = reset_value
                data_value = converter(data_index)
                data_index = data_index + 1
            end
            bits = OR(bits, resb > 0 and power or 0)
            power = LSHIFT(power, 1)
        end
        return bits
    end

    -- function to adjust the size of dictionary
    local function balance_size()
        if enlarge_in == 0 then
            enlarge_in = 2 ^ num_bits
            num_bits = num_bits + 1
        end
    end

    -- function to add dictionary entries
    local function add_entries(char)
        dictionary[dict_size] = char
        dict_size = dict_size + 1
        enlarge_in = enlarge_in - 1
    end

    -- initialise the dictionary and output
    local bits = read_bits(2)
    if bits == 2 then return "" end
    bits = read_bits((bits + 1) * 8)
    local char = string.char(bits)
    dictionary[3] = char
    local text = char
    result[#result + 1] = char
    local counter = 0

    -- read input data and build a dictionary 
    while true do
        counter = counter + 1
        bits = read_bits(num_bits)
        local count = bits
        if count == 2 then
            return table.concat(result)
        elseif count == 0 or count == 1 then
            bits = read_bits((count + 1) * 8)
            add_entries(string.char(bits))
            count = dict_size - 1
        end
        balance_size()
        entry = dictionary[count] or
            (text .. text:sub(1, 1))
        result[#result + 1] = entry
        char = entry:sub(1, 1)
        add_entries(text .. char)
        balance_size()
        text = entry
    end
end

-- All public methods provided for use

local function CompressToBase64(text)
    if not text or text == "" then return "" end
    local result = Compress(text, 6, function(i)
        return base64_alpha:sub(i + 1, i + 1) end)
    local ending = string.rep("=", 4 - (#result % 4))
    return result .. (#result % 4 > 0 and ending or "")
end

local function CompressToEncodedURI(text)
    if not text or text == "" then return "" end
    return Compress(text, 6, function(i) return
        safe_uri_alpha:sub(i + 1, i + 1) end)
end

local function CompressToUnicode(text)
    if not text or text == "" then return "" end
    return Compress(text, 15, function(i) return
        Unicode.ToChar(i + 32) end) .. (" ")
end

local function DecompressFromBase64(text)
    if not text or text == "" then return "" end
    return Decompress(#text, 32, function(i) return
        base64_table[text:sub(i + 1, i + 1)] end)
end

local function DecompressFromEncodedURI(text)
    if not text or text == "" then return "" end
    return Decompress(#text, 32, function(i) return
        safe_uri_table[text:sub(i + 1, i + 1)] end)
end

local function DecompressFromUnicode(text)
    if not text or text == "" then return "" end
    local array = text and Unicode.ToArray(text)
    return Decompress(#array, 16384, function(i)
        return Unicode.ToInt(array[i + 1]) - 32 end)
end

return {
    CompressToBase64 = CompressToBase64,
    CompressToEncodedURI = CompressToEncodedURI,
    CompressToUnicode = CompressToUnicode,
    DecompressFromBase64 = DecompressFromBase64,
    DecompressFromEncodedURI = DecompressFromEncodedURI,
    DecompressFromUnicode = DecompressFromUnicode,
}

