# JS-Driven Terminal UI Framework

A lightweight, reactive framework for building Terminal User Interfaces (TUI). This project combines the performance and drawing capabilities of C++ (using [FTXUI](https://github.com/ArthurSonzogni/FTXUI)) with the flexibility of JavaScript (using QuickJS) for state management and UI declaration.

## 🚀 Getting Started

### Prerequisites
Make sure you have the following installed on your system:
* CMake (v3.11 or higher)
* A modern C++ compiler (GCC, Clang, or MSVC)
* Git

### Downloading and Building
1. **Clone the repository:**
```bash
git clone https://github.com/AnnPoshtak/JsTui
cd JsTui```

2. **Generate build files and compile(Warning: It can take to 5 minutes to compile all files):**
```bash
mkdir build
cd build
cmake ..
cmake --build .```

3. **Run application:**
```bash
./JsTui```


## 🧠 How It Works

The core philosophy is similar to React, but for the terminal:
1. **JavaScript** holds the state and describes *what* the UI should look like.
2. **C++** parses this description and handles the actual rendering and hardware events.

The C++ engine expects to load a JavaScript file (e.g., `example.js`) that **must** contain a `render()` function. 

### Writing your UI (The JavaScript File)

Your JavaScript file consists of two main parts: **State/Actions** and the **Render Function**.

#### 1. State and Actions
Define your global variables to hold the current state. When a user interacts with a component (like pressing a button or changing a radio option), the C++ engine will look for the corresponding action function in JS.

```javascript
// State
let clickCount = 0;
let selectedRadio = 0;

// Actions
// Standard actions take no arguments
function onButtonClick() {
    clickCount++;
}

// Actions for selectable lists (radiobox, menu, toggle) receive the new index
function onRadioChange(newIndex) {
    selectedRadio = newIndex;
}

#### 2. The `render()` Function
The engine requires a global function named `render()`. It must return a **JSON string** describing the component tree.

```javascript
function render() {
    const uiLayout = {
        boxType: "vbox", // Main container: "vbox" (vertical) or "hbox" (horizontal)
        content: [
            {
                type: "text",
                content: "Current count: " + clickCount
            },
            {
                type: "button",
                content: "Increment",
                action: "onButtonClick" // Maps to the JS function name
            },
            {
                type: "radiobox",
                content: ["Option 1", "Option 2"],
                selected: selectedRadio, // Bind to JS state
                action: "onRadioChange"
            }
        ],
        IsBorder: true,
        BorderColor: { r: 252, g: 186, b: 3 }
    };
    
    return JSON.stringify(uiLayout);
}

## 🧩 Supported Components

Here is a quick reference for the currently supported components inside the `content` array:

| Component Type | Expected Properties | Description |
| :--- | :--- | :--- |
| `"vbox" / "hbox"` | `content` (array) | Container that stacks children vertically or horizontally. Supports nesting. |
| `"text"` | `content` (string/number) | Displays plain text. |
| `"separator"` | *None* | Draws a dividing line. |
| `"button"` | `content` (string), `action` (string) | A clickable button that triggers a JS function. |
| `"checkbox"` | `content` (string), `checked` (bool), `action` (string) | A toggleable checkbox. |
| `"radiobox"` | `content` (array), `selected` (int), `action` (string) | A vertical list of mutually exclusive options. |
| `"menu"` | `content` (array), `selected` (int), `action` (string) | A navigable vertical menu list. |
| `"toggle"` | `content` (array), `selected` (int), `action` (string) | A horizontal segmented control/toggle. |

*Note: For `radiobox`, `menu`, and `toggle`, the triggered JS action automatically receives the `newIndex` (integer) as its first argument.*

## 💡 Example

Check out `example.js` in the root directory for a complete working example containing all supported components and state management logic.