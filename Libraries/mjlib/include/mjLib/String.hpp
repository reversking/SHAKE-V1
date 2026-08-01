class String {
public:
    static std::wstring StringToWString(const std::string& str) {
        return std::wstring(str.begin(), str.end());
    }

    static std::string WStringToString(const std::wstring& wstr) {
        return std::string(wstr.begin(), wstr.end());
    }

    static std::string Trim(const std::string& str) {
        std::string::const_iterator start = str.begin();
        while (start != str.end() && std::isspace(*start)) {
            ++start;
        }

        std::string::const_iterator end = str.end();
        while (end != start && std::isspace(*(end - 1))) {
            --end;
        }
        return std::string(start, end);
    }

    static std::string Replace(const std::string str, const std::string from, const std::string to) {
        std::string result = str;
        size_t pos = 0;
        while ((pos = result.find(from, pos)) != std::string::npos) {
            result.replace(pos, from.length(), to);
            pos += to.length();
        }
        return result;
    }

    static std::string Random(int length) {
        const std::string chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
        std::string randomStr;
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, chars.size() - 1);
        for (int i = 0; i < length; ++i) {
            randomStr += chars[dis(gen)];
        }
        return randomStr;
    }

    static std::string ToHex(const std::string& input) {
        std::ostringstream oss;
        for (unsigned char c : input) {
            oss << std::setw(2) << std::setfill('0') << std::hex << (int)c;
        }
        return oss.str();
    }

    static std::string FromHex(const std::string& hexInput) {
        std::string output;
        if (hexInput.length() % 2 != 0) {
            return "";
        }
        for (size_t i = 0; i < hexInput.length(); i += 2) {
            std::string byteString = hexInput.substr(i, 2);
            char byte = (char)std::stoi(byteString, nullptr, 16);
            output.push_back(byte);
        }
        return output;
    }

    static std::string ToUpper(const std::string& str) {
        std::string result = str;
        std::transform(result.begin(), result.end(), result.begin(), ::toupper);
        return result;
    }

    static std::string ToLower(const std::string& str) {
        std::string result = str;
        std::transform(result.begin(), result.end(), result.begin(), ::tolower);
        return result;
    }
};