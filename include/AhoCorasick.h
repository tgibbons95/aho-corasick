#include <functional>

namespace CSE_584A
{
   struct TextMatchData
   {
      long long t = 0; // Position in text that matches. Starts at 1.
      long long p = 0; // Pattern line that matches. Starts at 1.
      long long s = 0; // Forward or reverse complement
      TextMatchData() = default;
      TextMatchData(long long t, long long p, long long s) : t(t), p(p), s(s) {}
   };

   void AhoCorasick(const char* patternsFile, const char* textFile, std::function<void(const TextMatchData&)> outputMethod);

}