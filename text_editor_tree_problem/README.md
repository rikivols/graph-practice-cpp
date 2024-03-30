# Text Editor Tree

This problem is to practice working with AVL trees. You need to create a data structure, that
can insert, delete and get an element on ith index in O(log n) time.

## Description
In ancient times, in a galaxy far away, two absolutely irreconcilable tribes of users lived on the planet Earth. Each tribe worshipped one of two very powerful text editors as their god: Vim and Emacs. One of the most important life missions (and malicious tongues claim that for some, it was the only mission) of these users was to engage in passionate internet discussions about which of the editors was better. Each of them claimed that their editor was the best and that the other should be banned by the authorities for its low efficiency.

However, in one small corner of this planet, a young and promising programmer named Elon was born, whose thoughts were different from the others. He perceived the irreconcilable fight between Vim and Emacs as a waste of time and energy. He was convinced that it would be much better to combine the best features of both editors into one versatile tool.

So, Elon began working on a project that was to bring unity between Vim and Emacs users. He decided to create an amazing text editor that would combine the power of Vim's shortcuts with the flexibility of Emacs's extensions. But that wasn't all. Elon wanted his editor to be so revolutionary that it would excite not only advanced users of text editors but the entire planet, including so-called BFUs. Therefore, he decided to incorporate two important features from each of the editors: from the Emacs environment, the adage "a good operating system, but a bad editor," and from Vim, the inability to exit the editor.

Elon, who delighted in efficiency, decided to name his editor χ. χ was to become a symbol of connection, progress, freedom, and peace on Earth. And from that time on, programmers lived in harmony, everyone could use χ according to their taste, and no one bothered with arguments between Vim and Emacs anymore. They were happy and created amazing programs and projects together because χ allowed them to reach the maximum of their creative power.

Do you really believe something like that could happen? No, that's just a fairy tale. In reality, Elon himself can't program very well; he is better at impulsive decision-making, crisis management, and marketing. Therefore, he asked you to help him program the editor. In exchange for your 120 hours of work per week, he promises that once χ is released into the world, you will be listed as the second author.

## Program Interface
Your task is to implement the **TextEditorBackend** class, which represents the backend of a text editor. 
Required interface:

* **TextEditorBackend(const std::string& text)**: constructor that initializes the editor content with the given text.
* **size_t size() const**: Number of characters in the text, including new lines.
* **size_t lines() const**: Number of lines. An empty text has one line. Each \n character adds a line.
* **char at(size_t i) const**: Returns the i-th character. The allowed range is [0, size()).
* **void edit(size_t i, char c)**: Replaces the character at position i with character c. The allowed range is [0, size()).
* **void insert(size_t i, char c)**: Inserts character c before the character at position i. The allowed range is [0, size()].
* **void erase(size_t i)**: Deletes the character at position i. The allowed range is [0, size()).
* **size_t line_start(size_t r) const**: Returns the start of the r-th line. The \n character is part of the line it ends. The allowed range is [0, lines()).
* **size_t line_length(size_t r) const**: Returns the length of the i-th line, including the possible \n character at the end. The allowed range is [0, lines()).
* **size_t char_to_line(size_t i) const**: Returns the line number on which the i-th character is located. The allowed range is [0, size()).

If a method is called with an argument outside the range, it must throw a std::out_of_range exception. The methods size and lines should have constant complexity. Other methods should have logarithmic complexity. The constructor should not be slower than repeated calls to the insert method.

## Scoring Conditions
* To earn 1 point, the interface must be correctly implemented, but the time complexity of the methods (almost) does not matter. (The time limit is 15 seconds.)
* To earn 5 points, a naive implementation is not sufficient, but operations are still performed randomly. That is, the testing environment does not try to throw your editor off balance. (The time limit is 1 second.)
* To earn 10 points, it is necessary to correctly implement the interface with the mentioned or better complexities. (The time limit is 10 seconds.)