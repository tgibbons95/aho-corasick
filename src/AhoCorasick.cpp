#include "AhoCorasick.h"

#include <fstream>
#include <iostream>
#include <string>
#include <queue>

namespace CSE_584A
{
   enum Alphabet
   {
      FIRST = 0,
      ALPHABET_A = FIRST,
      ALPHABET_C,
      ALPHABET_G,
      ALPHABET_T,
      LAST
   };

   Alphabet ConvertChar(const char& c)
   {
      Alphabet transition = Alphabet::ALPHABET_A;
      switch (c)
      {
         case 'A':
            transition = Alphabet::ALPHABET_A;
            break;
         case 'a':
            transition = Alphabet::ALPHABET_A;
            break;
         case 'C':
            transition = Alphabet::ALPHABET_C;
            break;
         case 'c':
            transition = Alphabet::ALPHABET_C;
            break;
         case 'G':
            transition = Alphabet::ALPHABET_G;
            break;
         case 'g':
            transition = Alphabet::ALPHABET_G;
            break;
         case 'T':
            transition = Alphabet::ALPHABET_T;
            break;
         case 't':
            transition = Alphabet::ALPHABET_T;
            break;
         default:
            transition = Alphabet::ALPHABET_T;
            break;
      }
      return transition;
   }

   char ComputeReverseCompliment(char c)
   {
      char compliment = 'A';
      switch (c)
      {
      case 'A':
         compliment = 'T';
         break;
      case 'a':
         compliment = 't';
         break;
      case 'C':
         compliment = 'G';
         break;
      case 'c':
         compliment = 'g';
         break;
      case 'G':
         compliment = 'C';
         break;
      case 'g':
         compliment = 'c';
         break;
      case 'T':
         compliment = 'A';
         break;
      case 't':
         compliment = 'a';
         break;
      default:
         compliment = 'a';
         break;
      }
      return compliment;
   }

   std::string ComputeReverseCompliment(const std::string& line)
   {
      std::string ret = line;
      for (size_t i = 0; i < line.size(); ++i)
      {
         ret[i] = ComputeReverseCompliment(line[line.size() - i - 1]);
      }
      return ret;
   }

   struct Vertex
   {
      Alphabet charToGetHere = Alphabet::LAST;
      Vertex* nextState[Alphabet::LAST];
      Vertex* parent = nullptr;
      Vertex* failLink = nullptr;
      Vertex* nextOutput = nullptr;
      size_t outputPatternIndex = 0;
      bool outputNode = false;
      Vertex()
      {
         std::fill_n(nextState, Alphabet::LAST, nullptr);
      }
   };

   void BuildTrie(const std::vector<std::string>& patterns, std::vector<std::unique_ptr<Vertex>>& trie)
   {
      trie.clear();
      trie.emplace_back(std::move(std::make_unique<Vertex>()));
      auto currentState = trie[0].get();
      for (size_t patternIndex = 0; patternIndex < patterns.size(); ++patternIndex)
      {
         // Initialize current state root for each new pattern
         currentState = trie[0].get();
         const auto& pattern = patterns[patternIndex];

         for (size_t charIndex = 0; charIndex < pattern.size(); ++charIndex)
         {
            Alphabet transition = ConvertChar(pattern[charIndex]);
            if (currentState->nextState[transition])
            {
               // If the next state already exists, use it.
               currentState = currentState->nextState[transition];
            }
            else
            {
               // If the next state does not already exist, create it.
               trie.emplace_back(std::move(std::make_unique<Vertex>()));
               trie.back()->charToGetHere = transition;
               trie.back()->parent = currentState;
               currentState->nextState[transition] = trie.back().get();
               currentState = trie.back().get();

            }
         }
         currentState->outputNode = true;
         currentState->outputPatternIndex = patternIndex;
      }

      // Initialize breadth first queue
      std::queue<Vertex*> states;
      for (size_t transition = Alphabet::FIRST; transition < Alphabet::LAST; ++transition)
      {
         if (trie[0]->nextState[transition])
         {
            // Initialize failure links for initialized next states to root node.
            trie[0]->nextState[transition]->failLink = trie[0].get();
            states.push(trie[0]->nextState[transition]);
         }
         else
         {
            // Initialize uninitialized next links for root node to itself.
            trie[0]->nextState[transition] = trie[0].get();
         }
      }

      for (Vertex* currentState = nullptr; states.size() > 0; states.pop())
      {
         currentState = states.front();
         for (size_t transition = Alphabet::FIRST; transition < Alphabet::LAST; ++transition)
         {
            if (currentState->nextState[transition])
            {
               // Find appropriate fail link
               auto currentFail = currentState->failLink;
               while (!currentFail->nextState[transition])
               {
                  currentFail = currentFail->failLink;
               }

               // Initialize next state fail link
               currentFail = currentFail->nextState[transition];
               currentState->nextState[transition]->failLink = currentFail;

               // Add link to next output
               if (currentFail->outputNode)
               {
                  currentState->nextState[transition]->nextOutput = currentFail;
               }
               else
               {
                  currentState->nextState[transition]->nextOutput = currentFail->nextOutput;
               }

               states.push(currentState->nextState[transition]);
            }
         }
      }
   }

   void AhoCorasick(const char* patternsFilePath, const char* textFilePath, std::function<void(const TextMatchData&)> outputMethod)
   {
      // Open patterns file
      std::ifstream patternsFile(patternsFilePath);
      if(!patternsFile.is_open())
      {
         std::cout << "Cannot open patterns file";
         return;
      }

      // Open text file
      std::ifstream textFile(textFilePath);
      if (!textFile.is_open())
      {
         std::cout << "Cannot open text file";
         return;
      }

      // Gather Patterns
      std::vector<std::string> patterns;
      std::string line;
      while (std::getline(patternsFile, line))
      {
         patterns.emplace_back(line);
         patterns.emplace_back(ComputeReverseCompliment(line));
      }

      std::vector<std::unique_ptr<Vertex>> trie;

      // Build Trie
      BuildTrie(patterns, trie);

      auto currentState = trie[0].get();
      const int BUFFER_SIZE = 2048;
      char buffer[BUFFER_SIZE];
      int i = 0;

      TextMatchData match;
      std::streamsize total = 0;
      while (textFile)
      {
         textFile.read(buffer, BUFFER_SIZE);
         std::streamsize readcount = textFile.gcount();
         total += readcount;
         for (size_t j = 0; j < readcount; ++j)
         {
            // Follow failure links until we can proceed with current character
            auto transition = ConvertChar(buffer[j]);
            while (!currentState->nextState[transition])
            {
               currentState = currentState->failLink;
            }
            
            currentState = currentState->nextState[transition];


            // Check if current node is output
            if (currentState->outputNode)
            {
               match.p = (currentState->outputPatternIndex) / 2 + 1;
               match.s = (currentState->outputPatternIndex) % 2 + 1;
               match.t = total - readcount + j - patterns[currentState->outputPatternIndex].size() + 2;
               outputMethod(match);
            }

            // Enumerate other outputs that happen on this spot
            Vertex* outputState = currentState;
            while (outputState->nextOutput)
            {
               match.p = (outputState->nextOutput->outputPatternIndex) / 2 + 1;
               match.s = (outputState->nextOutput->outputPatternIndex) % 2 + 1;
               match.t = total - readcount + j - patterns[outputState->nextOutput->outputPatternIndex].size() + 2;
               outputMethod(match);
               outputState = outputState->nextOutput;
            }
         }
      }
   }
}