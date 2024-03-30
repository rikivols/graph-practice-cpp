# Christmas gifts

This problem is to practice dynamic programming.

There are many cpp files, attempts at solving this problem. The final and accepted solution is 
`final_and_last_solution.cpp`.

`skeletonToFill.cpp` contains tests and interface to fill your solution to.

## Description
Mr. Ebenezer Scrooge is a hard soul! How he can toil, be stingy, and accumulate wealth! He's sharp as flint, but never will a spark of noble fire be elicited from him. He's also withdrawn and self-satisfied. And as solitary as an oyster. The cold that prevailed within him made his old face stiff, shriveled, and wrinkled, and his pointed nose became ever sharper. When he walks down the street, he resembles an icicle. His eyes are reddened, his lips thin and bluish, and his shrewd voice sounds as if a wheel were squeaking without lubrication.

Mr. Ebenezer Scrooge was, now the sole, partner at the firm of Scrooge & Marley. Mr. Jacob Marley had died seven years ago. Scrooge, and now the late Marley, had been business partners for many years. Scrooge was also the sole executor of his last will, his only manager, his only heir, his only friend, and perhaps his only mourner. But even Scrooge wasn't too devastated by this sad event, as people expected. Even on the day of the funeral, he maintained his cool business mind and celebrated it with a well-concluded deal.

Tonight, Mr. Scrooge had a very restless sleep. And yet, there was no visible reason for it: despite the approaching Christmas Eve, when most freeloaders refuse to work, he managed to negotiate several excellent deals. Could it have been caused by those two nobodies who dared to disturb him today?

Around noon, two men of pleasant appearance, with hats and documents in their hands, entered the office, behaving modestly and politely. "The firm of Scrooge and Marley, is it?" said one of the gentlemen with a bow, looking at the document. "Do I have the pleasure of speaking to Mr. Scrooge, or Mr. Marley?"

"Mr. Marley died seven years ago," Scrooge replied.

"I hope his generosity has passed on to the living partner," said one of the gentlemen, handing Scrooge his business card.

Scrooge frowned and shook his head unkindly.

"In this festive season, Mr. Scrooge," said the gentleman, taking up a pen, "it is more desirable than ever that we should make some provision for the poor and destitute. They suffer most right now. Thousands lack basic necessities and don't even have warmth, sir."

All the more, to give his hard-earned money to some nuisances who can't take care of themselves. Just waiting with their hands out, expecting someone to give them something. As Mr. Scrooge remembered this, he woke up agitated. He lit the oil lamp that stood on the floor beside the bed and went to the kitchen for a glass of water.

"Am I dreaming, or awake?" Mr. Scrooge asked himself as he entered the kitchen. He turned up the flame of the lamp to make sure his eyes weren't deceiving him. Indeed, someone was sitting at the table. What audacity! But wait, wait. This is not just anybody. Why, the figure looked exactly like Mr. Jacob Marley. "Is that you, Jacob?" Mr. Scrooge asked timidly.

"Of course, it's me, old chap," the person said, starting to rise from his place at the table. "I had to come back because you've violated my last will! It was clearly stated that every Christmas you would give each of our shop's employees a gift, just as I did. I know you never agreed with it, but it was my last wish. You call yourself a friend?" With these last words, Mr. Marley was already gripping Mr. Scrooge's shoulder, who stood as if nailed to the spot and listened incredulously. "Tonight, three more spirits will visit you, who will clearly show you what awaits you if you continue to be stingy and do not fulfill the promise you gave me by taking over my inheritance," added Mr. Marley, and suddenly disappeared.

What happened to Mr. Scrooge during the night is not important now. What matters is that the next day he came to the shop and was a changed man. Imagine, he called his assistant Bob to his office and ordered him to procure Christmas gifts for the employees.

It goes without saying that Mr. Scrooge did not give Bob complete freedom. He prepared for him a list of approved gifts along with their prices. And he added that Bob must purchase the gifts as cheaply as possible while adhering to the following rule: no employee must receive the same gift as their superior. And so, Bob now sits dejectedly in his office, trying to figure out which gift to buy for which employee. Can you help him with the selection?

## Program Interface
Your task is to implement the function optimize_gifts, which assigns a Christmas gift to each employee. No employee may receive the same gift as their direct superior or any of their direct subordinates. At the same time, you want to minimize the total cost of the gifts. The function arguments are:

* **std::vector<Employee> boss**: For privacy reasons, employees are identified by numbers from 0 to their count minus one. This array thus indicates the number of employees and at the i-th position is listed the number of the direct superior of employee i. If an employee has no superior, the constant NO_EMPLOYEE is listed. No employee is indirectly their own superior.
* **std::vector<Price> gift_price**: A list of gift prices approved by Mr. Scrooge.

The return value is a pair **std::pair<Price, std::vector<Gift>>**, where the second item is a list that contains the number of the gift (according to the gift_price array) assigned to employee i, and the first item is the sum of the prices of all assigned gifts. You can assume that the input is correct.

## Scoring Conditions
* To earn 3 points, you need to correctly solve small instances of the order of tens of employees and units of gifts. (Time limit is 15 seconds.)
* To earn 6 to 10 points, a naive implementation is not sufficient. (Time limits are 1, 1, and 6 seconds.)
* Bonus: To earn up to 11.5 points, the solution must be not only fast asymptotically but also efficiently programmed. For bonus points, the time limit is 4.5 seconds, for the full score, the limit is 2.5 seconds.
The common time limit for all tests except the first one is 10 seconds.

Note: When using constants, whose incorrect value could affect the correctness of the calculation, it is necessary to justify why the used value is correct. Otherwise, no points may be awarded for the given solution.

Results achieved by modifying compiler settings will not be recognized.