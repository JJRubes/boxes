The majority of the work in the parser is parsing a box. This step starts with a list of strings, effectively forming a 2D character array, which is fitted to a box and includes the box's edge. The edges is checked for name so we are able to tell if it is a definition or a call. 
The parser works in [n] steps
1. Assume the program is a definition and draw a box around it.
2. Given this definition, find all of the boxes within it.
3. Parse each box you find, including finding if it is a call or a definition, what its name is and what pins it has.
4. Find the connections between pins in this definition.
5. Organise this into a useful structure.
