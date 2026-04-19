// App State
let userName = "";
let clickCount = 0;
let notificationsEnabled = true;

let selectedMenuItem = 1;     // 0: Profile, 1: Settings, 2: About
let selectedTheme = 0;        // 0: Dark, 1: Light, 2: System
let selectedDifficulty = 1;   // 0: Easy, 1: Normal, 2: Hard

// Action Handlers
function onInputEnter(text) {
    userName = text;
}

function onButtonClick() {
    clickCount++;
}

function onCheckboxToggle() {
    notificationsEnabled = !notificationsEnabled;
}

function onMenuSelect(index) {
    selectedMenuItem = index;
}

function onThemeChange(index) {
    selectedTheme = index;
}

function onDifficultyChange(index) {
    selectedDifficulty = index;
}

// Render Function 
function render() {
    // Greeting
    const greetingText = userName === "" ? "Guest" : userName;

    return JSON.stringify({
        boxType: "vbox",
        content: [
            // HEADER
            {
                type: "hbox",
                content: [
                    { type: "text", content: " 🚀 My CLI Dashboard" },
                    { type: "separator" }, // In hbox separator becam vertical
                    { type: "text", content: " Welcome, " + greetingText + "!" }
                ]
            },
            
            { type: "separator" }, // In vbox separator became horisontal
            
            // MAIN LAYOUT 
            {
                type: "hbox",
                content: [
                    // Left column: profile menu
                    {
                        type: "vbox",
                        content: [
                            { type: "text", content: "Navigation" },
                            { type: "separator" },
                            {
                                type: "menu",
                                content: ["👤 Profile", "⚙️ Settings", "ℹ️ About"],
                                selected: selectedMenuItem,
                                action: "onMenuSelect"
                            }
                        ]
                    },
                    
                    { type: "separator" }, 
                    
                    // Right column: settings
                    {
                        type: "vbox",
                        content: [
                            // 1. Input
                            {
                                type: "hbox",
                                content: [
                                    { type: "text", content: "Enter your name: " },
                                    { 
                                        type: "input", 
                                        placeholder: "Type here and press Enter...", 
                                        action: "onInputEnter" 
                                    }
                                ]
                            },
                            { type: "separator" },

                            // 2. Button + text
                            {
                                type: "hbox",
                                content: [
                                    { 
                                        type: "button", 
                                        content: "Click Me!", 
                                        action: "onButtonClick" 
                                    },
                                    { type: "text", content: " Total clicks: " + clickCount }
                                ]
                            },
                            { type: "separator" },

                            // 3. Checkbox
                            {
                                type: "checkbox",
                                content: "Enable Push Notifications",
                                checked: notificationsEnabled,
                                action: "onCheckboxToggle"
                            },
                            { type: "separator" },

                            // 4. Radiobox
                            { type: "text", content: "App Theme:" },
                            {
                                type: "radiobox",
                                content: ["Dark Mode", "Light Mode", "System Default"],
                                selected: selectedTheme,
                                action: "onThemeChange"
                            },
                            { type: "separator" },

                            // 5. Toggle
                            { type: "text", content: "Difficulty Level:" },
                            {
                                type: "toggle",
                                content: ["Easy", "Normal", "Hard"],
                                selected: selectedDifficulty,
                                action: "onDifficultyChange"
                            }
                        ]
                    }
                ]
            }
        ],
        IsBorder: true,
        BorderColor: { r: 66, g: 135, b: 245 }
    });
}