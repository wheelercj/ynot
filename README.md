# ynot

Add some color, style, advanced cursor movements, autocomplete suggestions, and more to your C++ terminal app! This fully cross-platform library includes a variety of functions and classes to make common challenges easier.

## download

You can download this library into your project with git by using `git submodule add https://github.com/wheelercj/ynot` and then adding `#include "../ynot/ynot/ynot.h"` to the file you want to use this library in. Get future updates with `git submodule update --remote`.

Alternatively, you can manually download a zip file or a tar.gz file of this library at https://github.com/wheelercj/ynot/tags. The library's version number is saved in ynot.h so that you can check whether you have the latest version in the future.

## usage

See the header files for the lists of functions and their descriptions. There are many examples of how you can use this library below, in the test files, and in [the matrix](https://github.com/wheelercj/the-matrix). C++17 or newer is required; in Visual Studio, you can choose the version of C++ with project > Properties > C/C++ > Language > C++ Language Standard.

## examples

The ynot library's `print` function (and other functions that print) allows you to print any emoji or other [Unicode](https://en.wikipedia.org/wiki/Unicode) symbols to a terminal. On Windows, printing emoji with C++ is normally very complicated. With this print function, it couldn't be more simple.

```cpp
#include "../ynot/ynot/ynot.h"
int main() {
    ynot::print("🔥🐊");
    ynot::print("\n I can print anything 😎🤖");
    return 0;
}
```

To print emoji, the file must be [saved with the UTF-8 encoding](https://docs.microsoft.com/en-us/visualstudio/ide/how-to-save-and-open-files-with-encoding?view=vs-2022) (_without_ signature/BOM) and your code must run in a modern terminal (such as [Windows Terminal](https://aka.ms/terminal); see [how to run your C++ app in Windows Terminal](https://wheelercj.github.io/notes/pages/20220506214620.html)).

Here's an example of `dedent` which removes indentation from a multiline string, and `getline_ac` which can give autocomplete suggestions (not autocorrect) and has optional built-in input validation:

```cpp
#include "../ynot/ynot/ynot.h"
using namespace std;
int main() {
    string menu = ynot::dedent(R"(
        Sample menu:
         * Create
         * Read
         * Update
         * Delete
        > )");
    ynot::print(menu);
    string choice = ynot::getline_ac(
        { "Create", "Read", "Update", "Delete" },
        "type an option");
    ynot::print("\n You chose " + choice);
    return 0;
}
```

![autocomplete menu example](https://media.giphy.com/media/Rqoco5DR2a2AjDAqtX/giphy.gif)

Below is another example. With ynot's `Paginator` class, you can cleanly present long pieces of text in a terminal.

```cpp
#include "../ynot/ynot/ynot.h"
using namespace std;
int main() {
    string article_title = "3.6 Git Branching - Rebasing";
    string article_body = ynot::dedent(R"(
        Article body here.
        Indent with tabs or spaces, not both.)");
    string line_prefix = "\n    ";
    ynot::Paginator paginator(article_title, article_body, line_prefix);
    paginator.run();
    return 0;
}
```

![paginator demo](https://media.giphy.com/media/tAn8Pis7lLUfA39MFa/giphy.gif)

The sample text is from [git-scm.com](https://git-scm.com/book/en/v2/Git-Branching-Rebasing).

### multithreading with terminal.h

If your app uses multiple threads that call functions in terminal.h, you can prevent race conditions by combining strings before printing.

Race condition example:

```cpp
string message;
int x, y, green;

...

ynot::set_cursor_coords(x, y);  // set_cursor_coords prints an ANSI code
ynot::print_rgb(0, green, 0, message);
```

![race condition example](https://media.giphy.com/media/Obc0RuoYP7XkHppQ0I/giphy.gif)

Each character is supposed to get steadily darker after it appears on screen, but sometimes the `print_rgb` function in one thread ends up being called between the `set_cursor_coords` and `print_rgb` calls in another thread.

Prevent this output scrambling by combining all strings into one before printing. The terminal.h functions that start with `ret_` return strings instead of printing them so that you can wait to print until after combining strings.

```cpp
string coord_str = ynot::ret_set_cursor_coords(x, y);
ynot::print_rgb(0, green, 0, coord_str + message);
```

![matrix demo](https://media.giphy.com/media/iArQ9LLVS30McyVR3u/giphy.gif)

You can see all of the code for what's shown in the gif [here](https://github.com/wheelercj/the-matrix).
