# INTRODUCTION
Santa Claus sleeps in his shop at the North Pole, and can only be wakened by either all nine reindeer being back from their year long vacation on the beaches of some tropical island in the South Pacific, or by some elves who are having some difficulties making toys.
## PROBLEM STATEMENT
To allow Santa to get some sleep, the elves can only wake him when three of them have problems. While three elves are having their problems solved, any other elves wishing to visit Santa must wait for those elves to return.
If Santa wakes up to find three elves waiting at the shop's door, along with the last reindeer having come back vacation, Santa decides that the elves must wait until after Christmas because it is more important to get the sleigh ready. 
# OBJECTIVES
The main objective is to write code for Santa, the reindeer and the elves.
The specific objectives are to:
•	Ensure the events happen in order at the right time. (Message ordering)
•	Ensure that the group of reindeer have higher priority over any elf groups that may be waiting at that time. (Priority)
•	The elf and reindeer groups must organize among themselves without any help from a Santa thread or process. (Self-organization)
•	Synchronization between various processes. The usual freedom from deadlock, livelock and starvation.