namespace Util {
	class String {
	public:
		static void Trim(string& s) {
			while (s.size() > 0 && isspace(s[0])) {
				s.erase(0, 1);
			}
			while (s.size() > 0 && isspace(s[s.size()-1])) {
				s.pop_back();
			}
		}

		static string TrimCopy(string& s) {
			string ret = string(s);
			Trim(ret);
			return ret;
		}

		static void ToLower(string& s) {
			for (int i = 0; i < s.size(); i++) {
				s[i] = tolower(s[i]);
			}
		}

		static string ToLowerCopy(const string& s) {
			string ret = string(s);
			ToLower(ret);
			return ret;
		}

		static void ToUpper(string& s) {
			for (int i = 0; i < s.size(); i++) {
				s[i] = toupper(s[i]);
			}
		}

		static string ToUpperCopy(const string& s) {
			string ret = string(s);
			ToUpper(ret);
			return ret;
		}

		static bool EqualsIgnoreCase(const string& a, const string& b) {
			if (a.size() != b.size()) {
				return false;
			}
			for (int i = 0; i < a.size(); i++) {
				if (tolower(a[i]) != tolower(b[i])) {
					return false;
				}
			}
			return true;
		}
	};
};