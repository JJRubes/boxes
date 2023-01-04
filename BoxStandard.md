Language names?
 - Boxes
 - Message Box
it's a bit functionally, so function or func something
 - Get Rect

Temporary standard for the language.
Nothing here is set in stone

# Basics
[Language Name] is a 2D language based around drawing boxes to represent functions and connecting them to express the flow of a program. There isn't variable assignment as all data is passed via connections between functions.

# Boxes
Boxes make up the bulk of [Language Name], as they are how functions are called and defined. 
## Edges
A box is primarily defined by its edge. The edge of a box contains all of th pins, as well as the name if it is a function definition. The edge is made up of a '+' in each corner. The top and bottom edges are made up of '|' or '-'. Other characters, not part of a function definition name, are interpretted as pins. For simplicity, ' ', '+', '|', and '-' are not valid pins. 
```
  +-----+
  |     |
  |     |
  +-----+
```
An example box.

# Functions
Every box is either a fucntion call or a function definition. A function call is connected to other function calls by wires between pins. A function definition's pins are only connected to function calls within the definition (should you be able to have an anonymous function that contains a definition but also connects to other calls?).
## Calls
A function call is a box with only pins along its edge, and with a function name within the box. It can run after all of its inputs have been given a value. Default functions are an exception, see below. 
```
  +---i---+
  | print |
  +-------+
```
A call to the "print" function. Note that this won't run as there is no input for the 'i' pin. 
When making a call the name of the call is placed inside of the box and the pins are around the outside, and in any order.

```
  +---+  +-o-y-x-+  +y+
  x + y  |   +   |  o+x
  +-o-+  +-------+  +-+
```
Three calls to the "+" function which will all output 'x' + 'y' on 'o'.
Similarly to Definition names I am unsure how to handle cases when the names are split across lines. Should they be concatenated? Or seperated by a space, newline, or comma? Should it try to match the closest function name?
```
  +----------+
  | function |
  |   call   |
  +----------+
```
A call to a function but I don't know what function it is trying to call.
### Support for Other Directions
I think it would be nice to be able to name functions is other directions, similarly to definition names. I don't know how I want to handle this.
```
  +---+  +-----+
  | n |  | n n |  +---+ 
  | a |  | a a |  | 名 |
  | m |  | m m |  | 前 |
  | e |  | e e |  +---+
  +---+  +-----+
```
3 examples of function calls that are meant to be read top to bottom. 
## Definitions
Function definitions have a name along its edge as well as pins. A definition can contain definitions as well as calls that will be run when the function is called. These calls can connect to the pins on the edge of the definition.
```
  +--- increment ---+
  |                 |
  |  +---+   +---+  |
  x--x + y---o 1 |  |
  |  +-o-+   +---+  |
  |    |            |
  +----o------------+
```
A definition of the "increment" function.
### Names
Spaces are used to define the name of the function a definition is for. The first group of spaces before a corner mark the start of function name, and the last group of spaces before the next corner mark the end.
```
  +- example box -+
  |               |
  |               |
  +---------------+
```
A function definition for "example box".
Names are read left to right for if they are on the top edge, and top to bottom if they are on the left or right edges.
```
   +-+  +-+
   | |  | |
   |      |  +- name -+  +-  name    +
   | n  n |  |        |  |           |
   | a  a |  +--------+  +-----------+
   | m  m |  +--------+
   | e  e |  |        |
   |      |  +- name -+
   | |  | |
   +-+  +-+
```
A collection of possible definitions for "name".
I don't know how I want to handle if there is a name on more than one edge.
Function names can contain spaces, but not '+'. 
```
  +- box with spaces in the name -+
  |                               |
  +-------------------------------+
```
A definition of the function "box with spaces in the name".
A name cannot contain only spaces.
```
  +-     -+
  |       |
  +-------+
```
A definition with an invalid name.
## Pins
Pins are how data is passed to function calls. An output pin of one function is connected to an input pin of another, and when all the inputs to a function have recieved an input the function is run on this data. 
A pin can be any character that is not '+', ' ', '-', or '|'.
a character can be used only once for either input or output
unless it not ambiguous which is input and output or if it is
being overloaded
```
  +--- example ----+
  |                |  example definition with 'a' as both
  |     +---+      |  input and output
  a-----x + y------b  where "+" is the addition function
  |     +-o-+      |  with inputs 'x', 'y' and output 'o'
  |       |        |
  +-------a--------+

  +---+
  | 1 |
  +-o-+
    |
  +-a---------+  +---------+
  |  example  a--i  print  |
  +-b---------+  +---------+
    |
  +-o-+
  | 1 |
  +---+

  +-- increment example --+
  |                       |
  |  +---------+  +---+   |  here it is ambiguous
  x--a example b--o 1 |   |  whether x or o is the
  |  +--a------+  +---+   |  output of increment example
  |     |                 |
  +-----o-----------------+

  +-- increment example --+  but with the use of arrows
  |                       |  (^, >, v, <)
  |  +---------+  +---+   |  the inputs and outputs can
  x->a example b--o 1 |   |  be specified.
  |  +--a------+  +---+   |  it doesn't matter which
  |     |                 |  and both can be specified
  +-----o-----------------+

  +-------- overloaded add ------+
  |                              |  here x and y are overloaded
  |             +---+            |  TODO: better way of describing
  x-----int-----x + y-----int----y  types
  |             +-o-+            |
  |               |              |
  |               >--------------o  here I've used an arrow to
  |                              |  make a turn, I don't know
  |          +--------+          |  if I want to allow '+'
  x--string--x concat y--string--y  as a corner. Currently I
  |          +----o---+          |  won't because it's harder
  |               |              |  to parse.
  +---------------o--------------+
```
if there is a pin that could accept multiple types
and there is a pin that can accept a specific type
always choose the more specific type

# Program Flow
A function only executes when all of its inputs are satisfied. So the first functions to execute is either a function with no inputs, or whose inputs are provided by the input of the function. If two functions have unrelated inputs there is no guaruntee for the order that they will be executed in. Therefore, controlling program becomes controlling when and how functions receive input.
## Splitting Outputs
Currently to split an output I have decided to use a core library function "s" which provides the same output on more than one pin. However, there are many ways that I could allow splitting and I have yet to decide on which.
### Dedicated Function
As above, this is the option I will use for the time being.
```
  +-i-+  +-i-+
  a s b  a s c
  +---+  +-b-+
```
The "s" function with input 'i' and output 'a', 'b' and optionally 'c'.
### Duplicate Pins
This is the most promising alternative. A function may have multiple copies of an output pin and each will provide the same data to the function they connect to. It is unclear for complex objects like lists if they should be copies or references.
```
  +---+   +---+   +---+
  | 1 | v-o 1 | v-o 1 o-v
  +-o-+ | +-o-+ | +-o-+ |
    |   |   |   |       |
  +-x-+ | +-x-+ | +-x-+ |
  | + y-< | + y-< | + y-<
  +-o-+   +-o-+   +-o-+
```
An example of 0, 1, and 2 duplicate pins of the literal function "1".
### Splitting Wires
My original idea, and the most compact. A reserved character can be used to split a wire so that it can go to whatever inputs. Originally I considered '+', but this would severely complicate box parsing. Then I considered 't', but it looks ugly. This might be more viable if prettier boxes (described below) are used.
```
             +-------+
          >--i print |
          |  +-------+
  +---+   |  +-------+
  | 1 o---t--i print |
  +---+   |  +-------+
          |  +-------+
          >--i print |
             +-------+
```
The function "1" whose output is split by a 't' so that it can be the input of 3 different print functions.
### Duplicate Functions
If you want an output more than once, just run the function more than once. This raises the issue of now there might be too many of a given pin. These pins could be omitted or treated as unnecessary pins, which are described below. Another flaw is that a function definition might have a pin that is used by several functions within its definition, and there is no clear way to duplicate the definition.
## Unneccesary Pins
A function with a pin that does nothing can allow for more control over the order that functions are run. This is only significant for functions with side effects, such as "print".
```
  +--- start print ---+  +- end print -+     +-----------+   +---------+
  |                   |  |             |     | "Hello, " |   | "World" |
  |  +-------+  +---+ |  |  +-------+  |     +----o------+   +----o----+
  i--i print |  | 0 o-c  i--i print |  w          |               |
  |  +-------+  +---+ |  |  +-------+  |   +------i------+  +-----i-----+
  |                   |  |             |   | start print c--w end print |
  +-------------------+  +-------------+   +-------------+  +-----------+
```
A Hello, World program which has 2 definitions for alternative print functions, one to begin a chain, and one to end a chain. The are then used to ensure the order of printing.
In the example I have used a "0" to start the chain. I have yet to decide if I have a type which has no data be can be used in this case. I have not decided whether the function must have the pin terminate or if the pin can be left entirely unconnected, as in the example.
```
  +- wait -+   +- wait -+
  |        |   |        |
  i---v    w   i---v  *-w
  |   |    |   |   |    |
  +---o----+   +---o----+
```
Two example definitions of "wait", a function which will only let the input pass to the output when it has recieved a signal on the 'w' pin. In the left definition the 'w' pin is unconnected, while the right definition the 'w' pin is connected to a terminator '\*'.
I could also include a "wait" function in the core library so and not allow arbitrary functions to throw away inputs. There could also be a reserved pin which can be used on any function call to make it wait for an additional input. Or instead of a reserved pin, all of the functions which have side effects could be defined with and without a wait pin.
```
  +-w----+  +--i-w--+
  i wait o  | print |
  +------+  +-------+
```
And example of other possible solutions to waiting. The left is simply a core library implementation, and the right is a reserved pin that can be applied to any call. 
## Conditional Output
This allows functions to run not give an output on every output pin.
```
  +------+  +-------+
  | true |  | false |
  +---o--+  +---o---+
      |         |
   +--c-+    +--c-+
   | if |    | if |
   +--o-+    +--o-+
      |         |
    +-i-+     +-i-+
    | a |     | b |
    +---+     +---+
```
An example of an "if" function, the left "if" will output on pin 'o' and "a" will run, but the right "if" won't output on pin 'o' and so b cannot run.
The "if" function will likely be the simplest core function for this to work. However, the output of the "if" function doesn't have a clear type. The above "if" function can be used to create other conditional constructs such as an if-else. 
```
  +-- if a else b ---+
  |                  |
  |  +---+   +-----+ |
  i--i s a---i not | |
  |  +-b-+   +--o--+ |
  |    |        |    |
  |  +-i--+  +--i-+  |
  |  | if |  | if |  |
  |  +-o--+  +--o-+  |
  |    |        |    |
  +----a--------b----+
```
A definition of "if a else b" that outputs to either 'a' or 'b' but not both.
And can also be modified to send a value 
## Default
This allows a function to have an output on a pin before the function has any inputs. Once the function has all of its inputs it will run like normal and update the value of the pin. This allows for the creation of loops.
```
  +---i---j---+
  |  default  |
  +-----o-----+
```
A possible core library function for "default" which will output the value at 'i' until it receive a value for 'j'.
Default and Conditionals can be used in combination to produce loops.
```
                +-------+
                | print |
                +---i---+
                    |
     +---------+  +-a-+   +---+  +-----+
     | default o--i s b---x < y--o 100 |
     +-i-----j-+  +-c-+   +-o-+  +-----+
       |     |      |       |
     +-o-+ +-o-+    |    +--i-+
     | 0 | | + y----<    | if |
     +---+ +-x-+         +-o--+
             |             |
          +--o---+         |
          | wait w---------<
          +--i---+
             |
           +-o-+
           | 1 |
           +---+
```
A program to print the numbers 0 to 99.
I am realising now that it might be quite hard to determine when a program has ended.

# Built-in Functions
literals
use an 'o' for output
```
          +---+ +------+
base 10:  | 1 | | 8742 |
          +-o-+ +--o---+

          +------------+ +--------+
    hex:  | 0xDEADBEEF | | 0x89ab o
          +-----o------+ +--------+

          +------------+
 binary:  | 0b01000001 |
          +-----o------+

          +-----------------+
strings:  | "Hello, World!" |
          +--------o--------+
```

using x, y for input and o for output
```
            +---+  +---+  +---+  +---+
arithmetic: x + y  x - y  x * y  x / y
            +-o-+  +-o-+  +-o-+  +-o-+
```

some basic io
```
+---i---+  +---------+  +----------+  +----------+
| print |  | get int |  | get char |  | get line |
+-------+  +----o----+  +----o-----+  +----o-----+
```
# Comments
Because all important information is contained within boxes comments can be put almost anywhere and do not require any escaping. Comments cannot be on edges, connections, or in calls. The only characters that are not allowed in comments are '+' so that parsing boxes is easier.
```
  A program that prints "Hello, World"
  +---------------+
  | "Hello, World | The string literal "Hello, World"
  +-------o-------+
  The stri|ng is passed to a "print" function
      +---i---+
      | print |
      +-------+
```
A Hello, World program with some comments.
# Connections
A connection is between 2 pins.
# Additional Features
Features that may or may not be a permanent feature of the language. 
## Prettier Boxes
To allow for a cleaner look I would like to support box drawing characters for the edges of boxes. The corners will be '┌', '┐', '└', and '┘'. The edges will use '─' and '│'. 
```
  ┌─────┐
  │     │  ┌───────────────┐  ┌───────┐
  │     │  │ "Hello World" o──i print │
  │     │  └───────────────┘  └───────┘
  └─────┘
```
An empty box and a small Hello World program using the box drawing character for the edges of the boxes.