<div id="top"></div>
<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
      <ul>
        <li><a href="#built-with">Built With</a></li>
      </ul>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#installation">Installation</a></li>
      </ul>
    </li>
    <li><a href="#license">License</a></li>
    <li><a href="#acknowledgments">Acknowledgments</a></li>
  </ol>
</details>

<!-- ABOUT THE PROJECT -->
## About The Project

This projects implements the Aho-Corasick Algorithm for finding all positions within a text string where any string from a dictionary of patterns matches. The algorithm runs in time O(|L| + |T| + h) where |L| is the sum of all pattern lengths in the dictionary and h is the total number of matches. Preprocessing consists of a matching trie that has space proportional to O(|L|). 

Some interesting implementation choices to uphold some of the time and space requirements above:
* Keeping the space proportional to O(|L|) requires that no state in the trie stores a list of outputting states. Take the example patterns ACGTC, CGTC, GTC. When ACGTC is in the text string, all 3 patterns will match. Each state in the trie will have a pointer to the next output state. If that next output state is null, then there is no more matches. The example above will have the state corresponding to the second C in ACGTC will have a next output state to the second C in CGTC and that will have a next output state to GTC. This keeps the space requirement by not having 3 output items in one node and 2 in the next and so on.

<p align="right">(<a href="#top">back to top</a>)</p>



### Built With

* [CMake](https://cmake.org/)
* [C++](https://www.cplusplus.com/)
* [GoogleTest](https://github.com/google/googletest)

<p align="right">(<a href="#top">back to top</a>)</p>



<!-- GETTING STARTED -->
## Getting Started

### Prerequisites

This project requires at least C++14 because of calls to std::make_unique

### Installation

_A few example installations are provided for compilers clang, gcc, minGW, and msvc. _



<p align="right">(<a href="#top">back to top</a>)</p>



<!-- LICENSE -->
## License

Distributed under the MIT License. See `LICENSE` for more information.

<p align="right">(<a href="#top">back to top</a>)</p>



<!-- ACKNOWLEDGMENTS -->
## Acknowledgments

Used [README-Template](https://github.com/othneildrew/Best-README-Template) to create this readme.