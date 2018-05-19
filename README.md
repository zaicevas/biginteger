# BigInteger implementation in C
**BigInteger** (arbitrary-precision integer) implementation in C, made as a lab project in Vilnius University by **Tomas Zaicevas** and **Valdas Rakutis**. 
BigIntegers are stored in a singly-linked list.

![Preview](https://i.imgur.com/FjFH9FL.png)

## Recommendations
* Use string_to_bint to parse values. Example:
```
bint a = string_to_bint("-133713371337");
```
* Always initialize BigIntegers
* Parse only correct integers, because otherwise you will have a BigInteger, which equals to zero
* Always use free_bint to free memory, because every BigInteger is stored on the heap.
