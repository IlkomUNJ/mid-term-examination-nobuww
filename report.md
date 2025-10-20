## Correct window size for identifying segment
i think 3x3 window size is already correct for identifying a segment because it already capture horizontal, vertical, and diagonal line. **example:**

**horizontal line:**\
false false false\
true  true  true\
false false false

**vertical line:**\
false true false\
false true false\
false true false

**diagonal line:**\
true false false\
false true false\
false false true

where true = pixel is not white, false = pixel is white (empty)

## Appropriate window pattern for identifying a segment
the pattern for identifying a segment is if the pixel have more than 2 neighbor, there is an intersection on the line. **example:**

**T intersect:**\
false true false\
true true true\
false true false

**X intersect:**\
true false true\
false true false\
true false true

if the pixel only has 2 neighbor, it's a middle of the line, and if the pixel only has 1 neighbor, it's an endpoint of a line.

## AI usage
https://gemini.google.com/share/ce369c5e8ffb
https://www.perplexity.ai/search/what-do-you-think-the-ideal-pi-377E4PlTQdOOItO0ZPEHNg#0