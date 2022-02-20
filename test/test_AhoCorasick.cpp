// Test
#include "gtest/gtest.h"
#include "AhoCorasick.h"
#include <iostream>
#include <fstream>
#include <queue>

namespace CSE_584A
{
   namespace Testing
   {
      void RunTestCase(const char* patternFile, const char* textFile, const char* outputFile)
      {
         std::vector<TextMatchData> matches;
         auto matchesPtr = &matches;
         auto output = [matchesPtr](const TextMatchData& data)
         {
            matchesPtr->push_back(data);
         };

         AhoCorasick(patternFile, textFile, output);

         // Sorted matches
         auto compare = [](const TextMatchData& lhs, const TextMatchData& rhs) 
         { 
            bool lessThan = false;
            if (lhs.t < rhs.t)
            {
               lessThan = true;
            }
            else if (lhs.t == rhs.t)
            {
               if (lhs.p < rhs.p)
               {
                  lessThan = true;
               }
               else if (lhs.p == rhs.p)
               {
                  lessThan = lhs.s == rhs.s;
               }
            }
            return lessThan;
         };
         std::sort(matches.begin(), matches.end(), compare);

         // Enumerate sorted matches to file
         std::ofstream outputF(outputFile);
         for (const auto& match : matches)
         {
            outputF << match.t << " "
                    << match.p << " "
                    << match.s << std::endl;
         }
         outputF.close();
      }

      TEST(AhoCorasickHomeworkTest, Pattern1)
      {
         RunTestCase("pat1.txt", "hg38_part.txt", "case1.out");
      }

      TEST(AhoCorasickHomeworkTest, Pattern2)
      {
         RunTestCase("pat2.txt", "hg38_part.txt", "case2.out");
      }

      TEST(AhoCorasickHomeworkTest, Pattern3)
      {
         RunTestCase("pat3.txt", "hg38_part.txt", "case3.out");
      }

      TEST(AhoCorasickHomeworkTest, Pattern4)
      {
         RunTestCase("pat4.txt", "hg38_part.txt", "case4.out");
      }

      TEST(AhoCorasickHomeworkTest, Pattern5)
      {
         RunTestCase("pat5.txt", "hg38_part.txt", "case5.out");
      }

      TEST(AhoCorasickStressTest, ArbitrarySubstring)
      {
         std::queue<TextMatchData> expectedResults;
         auto expectedResultsPtr = &expectedResults;
         expectedResults.emplace(TextMatchData(2, 3, 1));
         expectedResults.emplace(TextMatchData(1, 2, 1));
         expectedResults.emplace(TextMatchData(5, 3, 1));
         expectedResults.emplace(TextMatchData(4, 2, 1));

         auto output = [expectedResultsPtr](const TextMatchData& data)
         {
            if (expectedResultsPtr->size() > 0)
            {
               auto expectedMatch = expectedResultsPtr->front();
               EXPECT_EQ(expectedMatch.t, data.t) << "Position Failed";
               EXPECT_EQ(expectedMatch.p, data.p) << "Pattern Failed";
               EXPECT_EQ(expectedMatch.s, data.s) << "Compliment Failed";
               expectedResultsPtr->pop();
            }
            else
            {
               FAIL() << "Too many matches";
            }
         };

         std::string textFile = "inputT.txt";
         std::ofstream inputT(textFile);
         inputT << "CGTCGTC";
         inputT.close();

         std::string patternFile = "inputP.txt";
         std::ofstream inputP(patternFile);
         inputP << "ACGTC" << std::endl;
         inputP << "CGTC" << std::endl;
         inputP << "GT" << std::endl;
         inputP.close();

         AhoCorasick(patternFile.c_str(), textFile.c_str(), output);
         EXPECT_EQ(0, expectedResults.size()) << "All Results Not Found";
      }

      TEST(AhoCorasickStressTest, RepeatedCharacter)
      {
         std::queue<TextMatchData> expectedResults;
         auto expectedResultsPtr = &expectedResults;
         expectedResults.emplace(TextMatchData(1, 1, 1));
         expectedResults.emplace(TextMatchData(1, 2, 1));
         expectedResults.emplace(TextMatchData(2, 1, 1));
         expectedResults.emplace(TextMatchData(2, 2, 1));
         expectedResults.emplace(TextMatchData(3, 1, 1));
         expectedResults.emplace(TextMatchData(3, 2, 1));
         expectedResults.emplace(TextMatchData(4, 1, 1));

         auto output = [expectedResultsPtr](const TextMatchData& data)
         {
            if (expectedResultsPtr->size() > 0)
            {
               auto expectedMatch = expectedResultsPtr->front();
               EXPECT_EQ(expectedMatch.t, data.t) << "Position Failed";
               EXPECT_EQ(expectedMatch.p, data.p) << "Pattern Failed";
               EXPECT_EQ(expectedMatch.s, data.s) << "Compliment Failed";
               expectedResultsPtr->pop();
            }
            else
            {
               FAIL() << "Too many matches";
            }
         };

         std::string textFile = "inputT.txt";
         std::ofstream inputT(textFile);
         inputT << "CCCCC";
         inputT.close();

         std::string patternFile = "inputP.txt";
         std::ofstream inputP(patternFile);
         inputP << "CC" << std::endl;
         inputP << "CCC" << std::endl;
         inputP.close();

         AhoCorasick(patternFile.c_str(), textFile.c_str(), output);
         EXPECT_EQ(0, expectedResults.size()) << "All Results Not Found";
      }
   }
}
